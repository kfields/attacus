#include "main.h"

void drawParagraph(struct NVGcontext* vg, float x, float y, float width, float height, float mx, float my)
{
	struct NVGtextRow rows[3];
	struct NVGglyphPosition glyphs[100];
	const char* text = "This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.🎉";
	const char* start;
	const char* end;
	int nrows, i, nglyphs, j, lnum = 0;
	float lineh;
	float caretx, px;
	float bounds[4];
	float a;
	float gx = 0.0f, gy = 0.0f;
	int gutter = 0;
	NVG_NOTUSED(height);

	nvgSave(vg);

	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
	nvgTextMetrics(vg, NULL, NULL, &lineh);

	// The text break API can be used to fill a large buffer of rows,
	// or to iterate over the text just few lines (or just one) at a time.
	// The "next" variable of the last returned item tells where to continue.
	start = text;
	end = text + strlen(text);
	for (nrows = nvgTextBreakLines(vg, start, end, width, rows, 3); 0 != nrows; nrows = nvgTextBreakLines(vg, start, end, width, rows, 3) )
	{
		for (i = 0; i < nrows; i++) {
			struct NVGtextRow* row = &rows[i];
			int hit = mx > x && mx < (x+width) && my >= y && my < (y+lineh);

			nvgBeginPath(vg);
			nvgFillColor(vg, nvgRGBA(255,255,255,hit?64:16) );
			nvgRect(vg, x, y, row->width, lineh);
			nvgFill(vg);

			nvgFillColor(vg, nvgRGBA(255,255,255,255) );
			nvgText(vg, x, y, row->start, row->end);

			if (hit) {
				caretx = (mx < x+row->width/2) ? x : x+row->width;
				px = x;
				nglyphs = nvgTextGlyphPositions(vg, x, y, row->start, row->end, glyphs, 100);
				for (j = 0; j < nglyphs; j++) {
					float x0 = glyphs[j].x;
					float x1 = (j+1 < nglyphs) ? glyphs[j+1].x : x+row->width;
					float tgx = x0 * 0.3f + x1 * 0.7f;
					if (mx >= px && mx < tgx)
						caretx = glyphs[j].x;
					px = tgx;
				}
				nvgBeginPath(vg);
				nvgFillColor(vg, nvgRGBA(255,192,0,255) );
				nvgRect(vg, caretx, y, 1, lineh);
				nvgFill(vg);

				gutter = lnum+1;
				gx = x - 10;
				gy = y + lineh/2;
			}
			lnum++;
			y += lineh;
		}
		// Keep going...
		start = rows[nrows-1].next;
	}

	if (gutter)
	{
		char txt[16];
		bx::snprintf(txt, sizeof(txt), "%d", gutter);
		nvgFontSize(vg, 13.0f);
		nvgTextAlign(vg, NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);

		nvgTextBounds(vg, gx,gy, txt, NULL, bounds);

		nvgBeginPath(vg);
		nvgFillColor(vg, nvgRGBA(255,192,0,255) );
		nvgRoundedRect(vg
			,  bx::round(bounds[0])-4.0f
			,  bx::round(bounds[1])-2.0f
			,  bx::round(bounds[2]-bounds[0])+8.0f
			,  bx::round(bounds[3]-bounds[1])+4.0f
			, (bx::round(bounds[3]-bounds[1])+4.0f)/2.0f-1.0f
			);
		nvgFill(vg);

		nvgFillColor(vg, nvgRGBA(32,32,32,255) );
		nvgText(vg, gx,gy, txt, NULL);
	}

	y += 20.0f;

	nvgFontSize(vg, 13.0f);
	nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
	nvgTextLineHeight(vg, 1.2f);

	nvgTextBoxBounds(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL, bounds);

	// Fade the tooltip out when close to it.
	gx = bx::abs((mx - (bounds[0]+bounds[2])*0.5f) / (bounds[0] - bounds[2]) );
	gy = bx::abs((my - (bounds[1]+bounds[3])*0.5f) / (bounds[1] - bounds[3]) );
	a = bx::max(gx, gy) - 0.5f;
	a = bx::clamp(a, 0.0f, 1.0f);
	nvgGlobalAlpha(vg, a);

	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(220,220,220,255) );
	nvgRoundedRect(vg
		, bx::round(bounds[0]-2.0f)
		, bx::round(bounds[1]-2.0f)
		, bx::round(bounds[2]-bounds[0])+4.0f
		, bx::round(bounds[3]-bounds[1])+4.0f
		, 3.0f
		);
	px = float( (int)( (bounds[2]+bounds[0])/2) );
	nvgMoveTo(vg, px,bounds[1] - 10);
	nvgLineTo(vg, px+7,bounds[1]+1);
	nvgLineTo(vg, px-7,bounds[1]+1);
	nvgFill(vg);

	nvgFillColor(vg, nvgRGBA(0,0,0,220) );
	nvgTextBox(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL);

	nvgRestore(vg);
}
