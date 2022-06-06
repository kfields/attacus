#include <math.h>

#include <bx/bx.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <bx/string.h>

#include <imgui/imgui.h>
#include <nanovg/nanovg.h>

#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

struct DemoData
{
	int fontNormal, fontBold, fontIcons, fontEmoji;
	int images[12];
};

//extern int32_t createImage(struct NVGcontext* _ctx, const char* _filePath, int _imageFlags);
extern int loadDemoData(struct NVGcontext* vg, struct DemoData* data);
extern void freeDemoData(struct NVGcontext* vg, struct DemoData* data);

extern char* cpToUTF8(int cp, char* str);

extern void drawWindow(struct NVGcontext* vg, const char* title, float x, float y, float w, float h);
extern void drawSearchBox(struct NVGcontext* vg, const char* text, float x, float y, float w, float h);
extern void drawEyes(struct NVGcontext* vg, float x, float y, float w, float h, float mx, float my, float t);
extern void drawParagraph(struct NVGcontext* vg, float x, float y, float width, float height, float mx, float my);

extern void drawLines(struct NVGcontext* vg, float x, float y, float w, float h, float t);
extern void drawWidths(struct NVGcontext* vg, float x, float y, float width);
extern void drawCaps(struct NVGcontext* vg, float x, float y, float width);

extern void drawGraph(struct NVGcontext* vg, float x, float y, float w, float h, float t);
extern void drawColorwheel(struct NVGcontext* vg, float x, float y, float w, float h, float t);
extern void drawScissor(struct NVGcontext* vg, float x, float y, float t);
