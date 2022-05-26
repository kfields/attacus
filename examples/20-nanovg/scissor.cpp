#include "main.h"

void drawScissor(struct NVGcontext* vg, float x, float y, float t)
{
	nvgSave(vg);

	// Draw first rect and set scissor to it's area.
	nvgTranslate(vg, x, y);
	nvgRotate(vg, nvgDegToRad(5) );
	nvgBeginPath(vg);
	nvgRect(vg, -20,-20,60,40);
	nvgFillColor(vg, nvgRGBA(255,0,0,255) );
	nvgFill(vg);
	nvgScissor(vg, -20,-20,60,40);

	// Draw second rectangle with offset and rotation.
	nvgTranslate(vg, 40,0);
	nvgRotate(vg, t);

	// Draw the intended second rectangle without any scissoring.
	nvgSave(vg);
	nvgResetScissor(vg);
	nvgBeginPath(vg);
	nvgRect(vg, -20,-10,60,30);
	nvgFillColor(vg, nvgRGBA(255,128,0,64) );
	nvgFill(vg);
	nvgRestore(vg);

	// Draw second rectangle with combined scissoring.
	nvgIntersectScissor(vg, -20,-10,60,30);
	nvgBeginPath(vg);
	nvgRect(vg, -20,-10,60,30);
	nvgFillColor(vg, nvgRGBA(255,128,0,255) );
	nvgFill(vg);

	nvgRestore(vg);
}
