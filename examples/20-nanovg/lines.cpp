#include "main.h"

void drawLines(struct NVGcontext* vg, float x, float y, float w, float h, float t)
{
	int i, j;
	float pad = 5.0f, s = w/9.0f - pad*2;
	float pts[4*2], fx, fy;
	int joins[3] = {NVG_MITER, NVG_ROUND, NVG_BEVEL};
	int caps[3] = {NVG_BUTT, NVG_ROUND, NVG_SQUARE};
	NVG_NOTUSED(h);

	nvgSave(vg);
	pts[0] = -s*0.25f + cosf(t*0.3f) * s*0.5f;
	pts[1] = sinf(t*0.3f) * s*0.5f;
	pts[2] = -s*0.25f;
	pts[3] = 0;
	pts[4] = s*0.25f;
	pts[5] = 0;
	pts[6] = s*0.25f + cosf(-t*0.3f) * s*0.5f;
	pts[7] = sinf(-t*0.3f) * s*0.5f;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fx = x + s*0.5f + (i*3+j)/9.0f*w + pad;
			fy = y - s*0.5f + pad;

			nvgLineCap(vg, caps[i]);
			nvgLineJoin(vg, joins[j]);

			nvgStrokeWidth(vg, s*0.3f);
			nvgStrokeColor(vg, nvgRGBA(0,0,0,160) );
			nvgBeginPath(vg);
			nvgMoveTo(vg, fx+pts[0], fy+pts[1]);
			nvgLineTo(vg, fx+pts[2], fy+pts[3]);
			nvgLineTo(vg, fx+pts[4], fy+pts[5]);
			nvgLineTo(vg, fx+pts[6], fy+pts[7]);
			nvgStroke(vg);

			nvgLineCap(vg, NVG_BUTT);
			nvgLineJoin(vg, NVG_BEVEL);

			nvgStrokeWidth(vg, 1.0f);
			nvgStrokeColor(vg, nvgRGBA(0,192,255,255) );
			nvgBeginPath(vg);
			nvgMoveTo(vg, fx+pts[0], fy+pts[1]);
			nvgLineTo(vg, fx+pts[2], fy+pts[3]);
			nvgLineTo(vg, fx+pts[4], fy+pts[5]);
			nvgLineTo(vg, fx+pts[6], fy+pts[7]);
			nvgStroke(vg);
		}
	}


	nvgRestore(vg);
}

void drawWidths(struct NVGcontext* vg, float x, float y, float width)
{
	int i;

	nvgSave(vg);

	nvgStrokeColor(vg, nvgRGBA(0,0,0,255) );

	for (i = 0; i < 20; i++)
	{
		float w = (i+0.5f)*0.1f;
		nvgStrokeWidth(vg, w);
		nvgBeginPath(vg);
		nvgMoveTo(vg, x,y);
		nvgLineTo(vg, x+width,y+width*0.3f);
		nvgStroke(vg);
		y += 10;
	}

	nvgRestore(vg);
}

void drawCaps(struct NVGcontext* vg, float x, float y, float width)
{
	int i;
	int caps[3] = {NVG_BUTT, NVG_ROUND, NVG_SQUARE};
	float lineWidth = 8.0f;

	nvgSave(vg);

	nvgBeginPath(vg);
	nvgRect(vg, x-lineWidth/2, y, width+lineWidth, 40);
	nvgFillColor(vg, nvgRGBA(255,255,255,32) );
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRect(vg, x, y, width, 40);
	nvgFillColor(vg, nvgRGBA(255,255,255,32) );
	nvgFill(vg);

	nvgStrokeWidth(vg, lineWidth);
	for (i = 0; i < 3; i++) {
		nvgLineCap(vg, caps[i]);
		nvgStrokeColor(vg, nvgRGBA(0,0,0,255) );
		nvgBeginPath(vg);
		nvgMoveTo(vg, x, y + i*10 + 5);
		nvgLineTo(vg, x+width, y + i*10 + 5);
		nvgStroke(vg);
	}

	nvgRestore(vg);
}
