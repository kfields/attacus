#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include "flutter_composite_view.h"
#include "backing_store.h"

namespace attacus
{

class BackingStoreLayer : public Layer {
public:
    BackingStoreLayer(BackingStore& surface) : surface_(&surface) {}
    void Draw() override;
    // Data members
    BackingStore* surface_;
};

FlutterCompositeView::FlutterCompositeView(View& parent, ViewParams params) : FlutterView(parent, params)
{
}

void FlutterCompositeView::Create() {
    FlutterView::Create();

}

void FlutterCompositeView::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    InitCompositor(compositor_);
    args.compositor = &compositor_;
}

void FlutterCompositeView::InitCompositor(FlutterCompositor& compositor) {
    compositor.struct_size = sizeof(FlutterCompositor);
    compositor.user_data = this;
    compositor.create_backing_store_callback =
        [](const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.CreateBackingStore(*config, *backing_store_out);
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.CollectBackingStore(*renderer);
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.PresentLayers(layers, layers_count);
        };
}

bool FlutterCompositeView::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out) {
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;
    BackingStore& surface = *BackingStore::Produce<BackingStore>(SurfaceParams(Size(width, height)));
    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeFramebuffer;
    FlutterOpenGLFramebuffer& fbOut = backing_store_out.open_gl.framebuffer;
    //fbOut.target = GL_TEXTURE_2D;
    //fbOut.target = GL_RENDERBUFFER;
    //fbOut.target = 0;
    fbOut.target = GL_RGBA8;
    fbOut.name = surface.framebuffer_id;
    //
    fbOut.destruction_callback = [](void* userdata){};
    fbOut.user_data = &surface;
    return true;
}

bool FlutterCompositeView::CollectBackingStore(const FlutterBackingStore& renderer) {
    std::lock_guard<std::mutex> guard(guard_mutex_);
    Surface& surface = *static_cast<Surface*>(renderer.user_data);
    surface.Destroy();
    return true;
}

bool FlutterCompositeView::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::lock_guard<std::mutex> guard(guard_mutex_);

    layers_.clear();

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& layer = *layers[i];
        if (layer.type == kFlutterLayerContentTypeBackingStore) {
            BackingStore& surface = *static_cast<BackingStore*>(layer.backing_store->user_data);
            layers_.push_back(new BackingStoreLayer(surface));
        }
    }

    return true;
}

void FlutterCompositeView::Draw() {
    std::lock_guard<std::mutex> guard(guard_mutex_);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (std::vector<Layer*>::iterator it = layers_.begin(); it != layers_.end(); ++it){
        Layer* layer = *it;
        layer->Draw();
    }

    SDL_GL_SwapWindow(sdl_window_);

}

#define GLSL(src) "#version 150 core\n" #src

void BackingStoreLayer::Draw() {
    BackingStore& surface = *surface_;
    int width = surface.width();
    int height = surface.height();

 // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] = {
    //  Position      Color             Texcoords
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an Element Buffer Object and copy the element data to it
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    GLuint elements[] = {
         0, 1, 2,
         2, 3, 0
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
        GL_STATIC_DRAW);

    // Create and compile the vertex shader
    const char* vertexSource = GLSL(
        in vec2 position;
        in vec3 color;
        in vec2 texcoord;

        out vec3 Color;
        out vec2 Texcoord;
        
        void main() {
            Color = color;
            Texcoord = texcoord;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    );

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    const char* fragmentSource = GLSL(
        uniform sampler2D tex0;
        //uniform sampler2D tex1;

        in vec3 Color;
        in vec2 Texcoord;

        out vec4 outColor;
        
        void main() {
            //outColor = mix(texture(tex0, Texcoord), texture(tex1, Texcoord), 0.5) * vec4(Color, 1.0);
            outColor = texture(tex0, Texcoord) * vec4(Color, 1.0);
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
    glBindTexture(GL_TEXTURE_2D, surface.texture_id);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace attacus