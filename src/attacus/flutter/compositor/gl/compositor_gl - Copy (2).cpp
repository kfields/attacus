#include <iostream>

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/components/view_registry.h>
#include "backing_surface_gl.h"

#include "compositor_gl.h"
#include "definitions.h"

namespace attacus {

CompositorGL::CompositorGL(FlutterView& view) : FlutterComponent(view) {
}

void CompositorGL::Create() {
    FlutterComponent::Create();
}

FlutterCompositor* CompositorGL::InitCompositor() {
    FlutterCompositor& compositor = compositor_;
    compositor.struct_size = sizeof(FlutterCompositor);
    compositor.user_data = this;
    compositor.create_backing_store_callback =
        [](const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) -> bool {
            CompositorGL &self = *static_cast<CompositorGL*>(user_data);
            return self.CreateBackingStore(*config, *backing_store_out);
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            CompositorGL &self = *static_cast<CompositorGL*>(user_data);
            return self.CollectBackingStore(*renderer);
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            CompositorGL &self = *static_cast<CompositorGL*>(user_data);
            return self.PresentLayers(layers, layers_count);
        };
    compositor.avoid_backing_store_cache = false;
    return &compositor_;
}

BackingSurfaceGL* CompositorGL::AllocSurface(FlutterSize size) {
    auto width = size.width; auto height = size.height;

    SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);
    BackingSurfaceGL* surface = BackingSurfaceGL::Produce<BackingSurfaceGL>(SurfaceParams(Size(width, height)));
    return surface;
}

void CompositorGL::FreeSurface(BackingSurfaceGL& surface) {
    if (surface.used_)
        return;
    surface.used_ = true;
    free_surfaces_.push_back(&surface);
}

BackingSurfaceGL* CompositorGL::GetCachedSurface() {
    BackingSurfaceGL* surface = nullptr;
    for (std::list<BackingSurfaceGL*>::iterator it = free_surfaces_.begin(); it != free_surfaces_.end(); ++it) {
        surface = *it;
    }
    return surface;
}

/*bool Compositor::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurface& surface) {
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;

    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeTexture;
    FlutterOpenGLTexture& texOut = backing_store_out.open_gl.texture;
    texOut.target = GL_TEXTURE_2D;
    texOut.format = GL_RGBA8;
    texOut.name = surface.GetInternalTexture();
    //
    texOut.width = width;
    texOut.height = height;
    //
    texOut.destruction_callback = [](void* userdata){};
    texOut.user_data = &surface;
    return true;
}*/

bool CompositorGL::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out) {
    SDL_GL_MakeCurrent(view().sdl_window_, view().context_);
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;
    BackingSurfaceGL& surface = *BackingSurfaceGL::Produce<BackingSurfaceGL>(SurfaceParams(Size(width, height)));
    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeFramebuffer;
    FlutterOpenGLFramebuffer& fbOut = backing_store_out.open_gl.framebuffer;
    //fbOut.target = GL_TEXTURE_2D;
    //fbOut.target = GL_RENDERBUFFER;
    //fbOut.target = 0;
    fbOut.target = GL_RGBA8;
    fbOut.name = surface.framebuffer_id_;
    //
    fbOut.destruction_callback = [](void* userdata){};
    fbOut.user_data = &surface;
    return true;
}

bool CompositorGL::CollectBackingStore(const FlutterBackingStore& renderer) {
    Surface& surface = *static_cast<Surface*>(renderer.user_data);
    surface.Destroy();
    return true;
}

bool CompositorGL::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    SDL_GL_MakeCurrent(view().sdl_window_, view().context_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, view().width(), view().height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& layer = *layers[i];
        if (layer.type == kFlutterLayerContentTypeBackingStore) {
            PresentBackingStore(*layer.backing_store, layer.offset, layer.size);
        } else if (layer.type == kFlutterLayerContentTypePlatformView) {
            PresentPlatformView(*layer.platform_view, layer.offset, layer.size);
        }
    }

    SDL_GL_SwapWindow(view().sdl_window_);

    return true;
}

#define GLSL(src) "#version 150 core\n" #src

void CompositorGL::PresentPlatformView(const FlutterPlatformView& pview, FlutterPoint offset, FlutterSize size) {
    auto id = pview.identifier;
    View* view = flutter().viewRegistry().GetView(id);
    auto texId = view->GetInternalTexture();
    PresentTexture(texId, offset, size);
}

void CompositorGL::PresentBackingStore(const FlutterBackingStore& store, FlutterPoint offset, FlutterSize size) {
    BackingSurfaceGL& surface = *static_cast<BackingSurfaceGL*>(store.user_data);
    PresentTexture(surface.texture_id_, offset, size);
}

static PosRgbaTexCoord0Vertex s_vertices[4] = {
    {-1.0f,  1.0f,  1.0f, 0xff000000, 0.0f,  1.0f},
    {1.0f,  1.0f,  1.0f, 0xff000000,  1.0f,  1.0f},
    {1.0f, -1.0f,  1.0f, 0xff000000, 1.0f, 0.0f},
    {-1.0f, -1.0f,  1.0f, 0xff000000,  0.0f, 0.0f}
};

static TexCoord s_coords[4] = {
    {0.0f,  1.0f},
    {1.0f,  1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f}
};

/*
static float vertices[] = {
//  Position      Color             Texcoords
    -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Top-left
    1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Top-right
    1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
    -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f  // Bottom-left
};

*/

static GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
};

void CompositorGL::PresentTexture(uint32_t texId, FlutterPoint offset, FlutterSize size) {
    PosRgbaTexCoord0Vertex vertices[4];

    float x = offset.x;
    float y = offset.y;
    float width = size.width;
    float height = size.height;
    float angle = 0.0f;
    float scale = 1.0f;
    float screen_width = view().width();
    float screen_height = view().height();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    float halfWidth = float(width / 2) / screen_width;
    float halfHeight = float(height / 2) / screen_height;
    for (int i = 0; i < 4; ++i) {
        glm::vec4 vert = model * glm::vec4(s_vertices[i].x * halfWidth, s_vertices[i].y * halfHeight, 0.0f, 1.0f);
        vertices[i].x = vert[0];
        vertices[i].y = vert[1];
        vertices[i].z = vert[2];
        vertices[i].rgba = 0xff000000;
        vertices[i].u = s_coords[i].u;
        vertices[i].v = s_coords[i].v;
    }

// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an Element Buffer Object and copy the element data to it
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
        GL_STATIC_DRAW);

    // Create and compile the vertex shader
    static const char* vertexSource = GLSL(
        in vec3 position;
        in vec4 color;
        in vec2 texcoord;

        out vec4 Color;
        out vec2 Texcoord;
        
        void main() {
            Color = color;
            Texcoord = texcoord;
            //gl_Position = vec4(position, 0.0, 1.0);
            gl_Position = vec4(position, 1.0);
        }
    );

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    static const char* fragmentSource = GLSL(
        uniform sampler2D tex0;

        in vec4 Color;
        in vec2 Texcoord;

        out vec4 outColor;
        
        void main() {
            //outColor = mix(texture(tex0, Texcoord), texture(tex1, Texcoord), 0.5) * vec4(Color, 1.0);
            //outColor = texture(tex0, Texcoord) * vec4(Color, 1.0);
            outColor = texture(tex0, Texcoord) * Color
        }
    );

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace attacus