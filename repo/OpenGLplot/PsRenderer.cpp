/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2001
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
// adapted from:
//	 Frederic Delhoume
//	 Mark Kilgards 
// Code below is nothing more than Mark J. Kilgard's code
// adapted for C++/MFC
//
/////////////////////////////////////////////////////////////////////////////
// PsRenderer.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "PsRenderer.h"
#include "GlWinApp.h"

/////////////////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////////////////
CPsRenderer::CPsRenderer()
{
	m_pBoundingBox = NULL;
	m_dmOrientation = DMORIENT_UNKNOWN;
}

/////////////////////////////////////////////////////////////////////////////
// outputEPS
/////////////////////////////////////////////////////////////////////////////
void CPsRenderer::Run(LPCTSTR lpszFilename, BOOL bSort, int nValues, GLfloat *pFeedbackBuffer)
{
	ASSERT(lpszFilename);
	FILE *stream = fopen(lpszFilename, "wt");
	if (!stream)
	{
		TRACE("The stream '%s' was not opened.\n", lpszFilename);
		return;
	}

	SpewWireFrameEPS(stream, lpszFilename, bSort, nValues, pFeedbackBuffer);
	fclose(stream);
}

/////////////////////////////////////////////////////////////////////////////
// SpewWireFrameEPS 
/////////////////////////////////////////////////////////////////////////////
void CPsRenderer::SpewWireFrameEPS(FILE* stream, LPCTSTR lpszFilename, BOOL bSort, GLint size, GLfloat* buffer)
{
	GLfloat clearColor[4], viewport[4];
	GLfloat lineWidth;
	CString tmpString;

	// Read back a bunch of OpenGL state to help make the EPS
	// consistent with the OpenGL clear color, line width, point
	// size, and viewport.
	glGetFloatv(GL_VIEWPORT, viewport);
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);
	glGetFloatv(GL_POINT_SIZE, &m_PointSize);

	// Emit EPS header.
	//	Notice %% for a single % in the fprintf calls.

	// Orientation
	fprintf(stream, "%%!PS-Adobe-2.0 EPSF-2.0\n");
	if (m_dmOrientation == DMORIENT_LANDSCAPE)
		fprintf(stream, "%%%%Orientation: Landscape\n");
	else if (m_dmOrientation == DMORIENT_PORTRAIT)
		fprintf(stream, "%%%%Orientation: Portrait\n");

	// Title
	tmpString = lpszFilename;
	tmpString = tmpString.Right(tmpString.GetLength()-tmpString.ReverseFind('\\')-1);
	fprintf(stream, "%%%%Title: %s\n", tmpString);

	// Creator
	GetModuleFileName(AfxGetInstanceHandle(), tmpString.GetBuffer(_MAX_PATH), _MAX_PATH);
	tmpString.ReleaseBuffer();
	tmpString = tmpString.Right(tmpString.GetLength()-tmpString.ReverseFind('\\')-1);
	fprintf(stream, "%%%%Creator: %s\n", tmpString);

	// CeationDate
	tmpString = COleDateTime::GetCurrentTime().Format("%m/%d/%y %H:%M");
	fprintf(stream, "%%%%CreationDate: %s\n", tmpString);

	// BoundingBox/Pages/Page
	if (m_pBoundingBox)
	{
		if (m_dmOrientation == DMORIENT_LANDSCAPE)
			fprintf(stream, "%%%%BoundingBox: %d %d %d %d\n", m_pBoundingBox->top, m_pBoundingBox->left, m_pBoundingBox->bottom, m_pBoundingBox->right);
		else
			fprintf(stream, "%%%%BoundingBox: %d %d %d %d\n", m_pBoundingBox->left, m_pBoundingBox->top, m_pBoundingBox->right, m_pBoundingBox->bottom);
		fprintf(stream, "%%%%Pages: 1\n");
		fprintf(stream, "%%%%Page: 1 1\n");
	}
	else
		fprintf(stream, "%%%%BoundingBox: %g %g %g %g\n", viewport[0], viewport[1], viewport[2], viewport[3]);

	fprintf(stream, "%%%%EndComments\n");
	fprintf(stream, "\n");
	fprintf(stream, "gsave\n");
	fprintf(stream, "\n");

	fprintf(stream, "/threshold %g def\n", EPS_GOURAUD_THRESHOLD);

	fprintf(stream, "\n%% RGB color command - r g b C\n");
	fprintf(stream, "/C { setrgbcolor } bind def\n");

	// This is more compact
	fprintf(stream, "\n%% Point - x_center y_center PointSize/2 0 360 r g b P\n");
	fprintf(stream, "/P { C arc fill } bind def\n");

	fprintf(stream, "\n%% Segment - x2 y2 x1 y1 r g b S\n");
	fprintf(stream, "/S { C moveto lineto stroke } bind def\n");

	fprintf(stream, "\n%% Flat-shaded triangle - x3 y3 x2 y2 x1 y1 r g b T\n");
	fprintf(stream, "/T { C newpath moveto lineto lineto closepath fill } bind def\n");

	// Output Frederic Delhoume's "gouraudtriangle" PostScript fragment.
	fprintf(stream, "%% The gouraudtriangle PostScript fragement below is free\n");
	fprintf(stream, "%% written by Frederic Delhoume (delhoume@ilog.fr)\n");

	for (int i = 0; GouraudTriangleEPS[i]; i++) 
		fprintf(stream, "%s\n", GouraudTriangleEPS[i]);

	if (m_pBoundingBox)
	{
		double dScale = 1.;
		if (m_dmOrientation == DMORIENT_LANDSCAPE)
		{
			if ((viewport[2]-viewport[0])*(m_pBoundingBox->bottom-m_pBoundingBox->top) > (viewport[3]-viewport[1])*(m_pBoundingBox->right-m_pBoundingBox->left))
			{
				// View area is wider (in x) than Printer area
				dScale = double(m_pBoundingBox->right-m_pBoundingBox->left)/double(viewport[2]-viewport[0]);
				fprintf(stream, "\n%d %d translate", ((m_pBoundingBox->bottom-m_pBoundingBox->top) + int((viewport[3]-viewport[1])*dScale))/2, 0);
			}
			else
			{
				// Printer area is wider (in x) than View area
				dScale = double(m_pBoundingBox->bottom-m_pBoundingBox->top)/double(viewport[3]-viewport[1]);
				fprintf(stream, "\n%d %d translate", m_pBoundingBox->bottom-m_pBoundingBox->top, ((m_pBoundingBox->right-m_pBoundingBox->left) - int((viewport[2]-viewport[0])*dScale))/2);
			}
			fprintf(stream, "\n%d rotate", 90);
		}
		else
		{
			if ((viewport[2]-viewport[0])*(m_pBoundingBox->bottom-m_pBoundingBox->top) > (viewport[3]-viewport[1])*(m_pBoundingBox->right-m_pBoundingBox->left))
			{
				// View area is wider (in x) than Printer area
				dScale = double(m_pBoundingBox->right-m_pBoundingBox->left)/double(viewport[2]-viewport[0]);
				fprintf(stream, "\n%d %d translate", 0, ((m_pBoundingBox->bottom-m_pBoundingBox->top) - int((viewport[3]-viewport[1])*dScale))/2);
			}
			else
			{
				// Printer area is wider (in x) than View area
				dScale = double(m_pBoundingBox->bottom-m_pBoundingBox->top)/double(viewport[3]-viewport[1]);
				fprintf(stream, "\n%d %d translate", ((m_pBoundingBox->right-m_pBoundingBox->left) - int((viewport[2]-viewport[0])*dScale))/2, 0);
			}
		}
		fprintf(stream, "\n%g %g scale", dScale, dScale);
	}

	fprintf(stream, "\n%g setlinewidth\n", EPS_LINE_WIDTH);

	// Clear the background like OpenGL had it
	fprintf(stream, "%g %g %g setrgbcolor\n", clearColor[0], clearColor[1], clearColor[2]);
	fprintf(stream, "%g %g %g %g rectfill\n\n", viewport[0], viewport[1], viewport[2], viewport[3]);

	// Main process
	if (bSort)
		SpewSortedFeedback(stream, size, buffer);
	else
		SpewUnsortedFeedback(stream, size, buffer);

	// Emit EPS trailer
	fprintf(stream, "\ngrestore\n");
	fprintf(stream, "showpage\n");
}

/////////////////////////////////////////////////////////////////////////////
// SpewUnsortedFeedback 
/////////////////////////////////////////////////////////////////////////////
void CPsRenderer::SpewUnsortedFeedback(FILE* stream, GLint size, GLfloat* buffer)
{
	GLfloat *loc, *end;

	loc = buffer;
	end = buffer + size;
	while (loc < end)
	{
		loc = SpewPrimitiveEPS(stream, loc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// SpewSortedFeedback 
/////////////////////////////////////////////////////////////////////////////
void CPsRenderer::SpewSortedFeedback(FILE* stream, GLint size, GLfloat* buffer)
{
	int token, item, nPrimitives, nVertices, i;
	GLfloat *loc, *end, depthSum;
	Feedback3Dcolor *vertex;
	DepthIndex *prims;

	loc = buffer;
	end = buffer + size;

	// Count how many primitives there are.
	nPrimitives = 0;
	while (loc < end)
	{
		token = (int)*loc++;
		switch (token)
		{
		case GL_LINE_TOKEN:
		case GL_LINE_RESET_TOKEN:
			loc += 14;
			nPrimitives++;
			break;
		case GL_POLYGON_TOKEN:
			nVertices = (int)*loc++;
			loc += (7 * nVertices);
			nPrimitives++;
			break;
		case GL_POINT_TOKEN:
			loc += 7;
			nPrimitives++;
			break;
		case GL_BITMAP_TOKEN:
		case GL_DRAW_PIXEL_TOKEN:
		case GL_COPY_PIXEL_TOKEN:
		case GL_PASS_THROUGH_TOKEN:
			// XXX Cannot be handled (yet).
			loc += 7;
			break;
		default:
			// XXX Left as an excersie to the reader.
			TRACE("Incomplete implementation. Unexpected token (%d).\n", token);
			ASSERT(FALSE);
		}
	}
	if (nPrimitives == 0)
		return;

	// Allocate an array of pointers that will point back at
	//	primitives in the feedback buffer.	There will be one
	//	entry per primitive. This array is also where we keep the
	//	primitive's average depth.	There is one entry per
	//	primitive	in the feedback buffer.
	prims = new DepthIndex[nPrimitives];

	item = 0;
	loc = buffer;
	while (loc < end)
	{
		prims[item].ptr = loc;	// Save this primitive's location.
		token = (int)*loc++;
		switch (token)
		{
		case GL_LINE_TOKEN:
		case GL_LINE_RESET_TOKEN:
			vertex = (Feedback3Dcolor*)loc;
			depthSum = vertex[0].z + vertex[1].z;
			prims[item].depth = (float)depthSum / 2.0f;
			loc += 14;
			break;
		case GL_POLYGON_TOKEN:
			nVertices = (int)*loc++;
			vertex = (Feedback3Dcolor*)loc;
			depthSum = vertex[0].z;
			for (i = 1; i < nVertices; i++)
			{
				depthSum += vertex[i].z;
			}
			prims[item].depth = depthSum / nVertices;
			loc += (7 * nVertices);
			break;
		case GL_POINT_TOKEN:
			vertex = (Feedback3Dcolor*)loc;
			prims[item].depth = vertex[0].z;
			loc += 7;
			break;
		case GL_BITMAP_TOKEN:
		case GL_DRAW_PIXEL_TOKEN:
		case GL_COPY_PIXEL_TOKEN:
		case GL_PASS_THROUGH_TOKEN:
			// XXX Cannot be handled (yet).
			loc += 7;
			item--;
			break;
		default:
			// XXX Left as an excersie to the reader.
			TRACE("Incomplete implementation. Unexpected token (%d).\n", token);
			ASSERT(FALSE);
		}
		item++;
	}
	ASSERT(item == nPrimitives);

	// Sort the primitives back to front.
	qsort(prims, nPrimitives, sizeof(DepthIndex), Compare);

	// XXX Understand that sorting by a primitives average depth
	//	doesn't allow us to disambiguate some cases like self
	//	intersecting polygons.	Handling these cases would require
	//	breaking up the primitives. That's too involved for this
	//	example. Sorting by depth is good enough for lots of
	//	applications.

	// Emit the Encapsulated PostScript for the primitives in
	//	back to front order.
	for (item = 0; item < nPrimitives; item++)
	{
		SpewPrimitiveEPS(stream, prims[item].ptr);
	}

	delete [] prims;
}

/////////////////////////////////////////////////////////////////////////////
// SpewPrimitiveEPS
/////////////////////////////////////////////////////////////////////////////
GLfloat *CPsRenderer::SpewPrimitiveEPS(FILE* stream, GLfloat* buffer)
{
	int token, i, nVertices, nSteps;
	GLfloat *loc;
	BOOL bSmooth;
	GLfloat red, green, blue, 
		dx, dy, dr, dg, db, 
		absR, absG, absB, 
		xStep, yStep, rStep, gStep, bStep, 
		xNext, yNext, rNext, gNext, bNext, 
		colormax, distance;
	Feedback3Dcolor* vertex;

	loc = buffer;

	token = (int)*loc++;
	switch (token) 
	{
	case GL_LINE_RESET_TOKEN:
	case GL_LINE_TOKEN:
		vertex = (Feedback3Dcolor*)loc;

		dr = vertex[1].red - vertex[0].red;
		dg = vertex[1].green - vertex[0].green;
		db = vertex[1].blue - vertex[0].blue;

		if (dr != 0 || dg != 0 || db != 0)
		{
			// Smooth shaded line.
			dx = vertex[1].x - vertex[0].x;
			dy = vertex[1].y - vertex[0].y;
			distance = (float)sqrt(dx * dx + dy * dy);

			absR = (float)fabs(dr);
			absG = (float)fabs(dg);
			absB = (float)fabs(db);

			colormax = max(absR, max(absG, absB));
			nSteps = (int)max(1.0, colormax * distance * EPS_SMOOTH_LINE_FACTOR);

			xStep = dx / nSteps;
			yStep = dy / nSteps;

			rStep = dr / nSteps;
			gStep = dg / nSteps;
			bStep = db / nSteps;

			xNext = vertex[0].x;
			yNext = vertex[0].y;
			rNext = vertex[0].red;
			gNext = vertex[0].green;
			bNext = vertex[0].blue;

			// Back up half a Step; we want the end points to be
			//	exactly the their endpoint colors.
			xNext -= (float)xStep / 2.0f;
			yNext -= (float)yStep / 2.0f;
			rNext -= (float)rStep / 2.0f;
			gNext -= (float)gStep / 2.0f;
			bNext -= (float)bStep / 2.0f;

			fprintf(stream, "%g %g %g setrgbcolor\n", vertex[0].red, vertex[0].green, vertex[0].blue);
			fprintf(stream, "%g %g moveto\n", vertex[0].x, vertex[0].y);

			for (i = 0; i < nSteps; i++) 
			{
				xNext += xStep;
				yNext += yStep;
				rNext += rStep;
				gNext += gStep;
				bNext += bStep;
				fprintf(stream, "%g %g lineto stroke\n", xNext, yNext);
				fprintf(stream, "%g %g %g setrgbcolor\n", rNext, gNext, bNext);
				fprintf(stream, "%g %g moveto\n", xNext, yNext);
			}
			fprintf(stream, "%g %g lineto stroke\n", vertex[1].x, vertex[1].y);
		}
		else // single color segment -> S
		{
			fprintf(stream, "%g %g %g %g %g %g %g S\n", 
				vertex[1].x,vertex[1].y, vertex[0].x,vertex[0].y, 
				vertex[0].red, vertex[0].green, vertex[0].blue);
		}

		loc += 14;	// Each vertex element in the feedback buffer is 7 GLfloats.

		break;
	case GL_POLYGON_TOKEN:
		nVertices = (int)*loc++;

		vertex = (Feedback3Dcolor*)loc;

		if (nVertices > 0)
		{
			red = vertex[0].red;
			green = vertex[0].green;
			blue = vertex[0].blue;
			bSmooth = FALSE;
			for (i = 1; i < nVertices; i++) 
				if (red 	!= vertex[i].red || 
						green != vertex[i].green || 
						blue	!= vertex[i].blue) 
				{
					bSmooth = TRUE;
					break;
				}
			if (bSmooth)
			{
				// Smooth shaded polygon; varying colors at vertices.
				int triOffset;

				// Break polygon into "nVertices-2" triangle fans.
				for (i = 0; i < nVertices - 2; i++) {
					triOffset = i * 7;

					fprintf(stream, "[%g %g %g %g %g %g]",
						vertex[0].x, vertex[i + 1].x, vertex[i + 2].x,
						vertex[0].y, vertex[i + 1].y, vertex[i + 2].y);
					fprintf(stream, " [%g %g %g] [%g %g %g] [%g %g %g] gouraudtriangle\n",
						vertex[0].red, vertex[0].green, vertex[0].blue,
						vertex[i + 1].red, vertex[i + 1].green, vertex[i + 1].blue,
						vertex[i + 2].red, vertex[i + 2].green, vertex[i + 2].blue);
					/*
					// x3 y3 r3 g3 b3 x2 y2 r2 g2 b2 x1 y1 r1 g1 b1 ST
					fprintf(stream, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g ST\n",
						vertex[i+2].x,vertex[i+2].y,vertex[i+2].red,vertex[i+2].green,vertex[i+2].blue,
						vertex[i+1].x,vertex[i+1].y,vertex[i+1].red,vertex[i+1].green,vertex[i+1].blue,
						vertex[0].x,vertex[0].y,vertex[0].red,vertex[0].green,vertex[0].blue);*/
				}
			}
			else
			{
				if (nVertices > 3)	// polygon
				{
					// Draw a filled polygon
					fprintf(stream, "newpath\n");
					fprintf(stream, "%g %g %g setrgbcolor\n", red, green, blue);
					fprintf(stream, "%g %g moveto\n", vertex[0].x, vertex[0].y);
					for (i = 1; i < nVertices; i++)
						fprintf(stream, "%g %g lineto\n", vertex[i].x, vertex[i].y);
					fprintf(stream, "closepath fill\n\n");
				}
				else				// triangle -> T
				{
					fprintf(stream, "%g %g %g %g %g %g %g %g %g T\n",
						vertex[2].x,vertex[2].y,
						vertex[1].x,vertex[1].y,
						vertex[0].x,vertex[0].y,
						red,green,blue);
				}
			}
		}
		loc += nVertices * 7;	// Each vertex element in the feedback buffer is 7 GLfloats.
		break;
	case GL_POINT_TOKEN:
		vertex = (Feedback3Dcolor*)loc;

		fprintf(stream, "%g %g %g 0 360 %g %g %g P\n",vertex[0].x, vertex[0].y,
			m_PointSize / 2.0,vertex[0].red,vertex[0].green,vertex[0].blue);
		loc += 7;				// Each vertex element in the feedback buffer is 7 GLfloats.
		break;
	case GL_BITMAP_TOKEN:
	case GL_DRAW_PIXEL_TOKEN:
	case GL_COPY_PIXEL_TOKEN:
	case GL_PASS_THROUGH_TOKEN:
		// XXX Cannot be handled (yet).
		loc += 7;				// Each vertex element in the feedback buffer is 7 GLfloats.
		break;
	default:
		// XXX Left as an excersie to the reader.
		TRACE("Incomplete implementation. Unexpected token (%d).\n", token);
		ASSERT(FALSE);
	}
	return loc;
}

/////////////////////////////////////////////////////////////////////////////
// Compare 
/////////////////////////////////////////////////////////////////////////////
int CPsRenderer::Compare(const void *a, const void *b)
{
	DepthIndex* p1 = (DepthIndex*)a;
	DepthIndex* p2 = (DepthIndex*)b;
	GLfloat diff = p2->depth - p1->depth;

	if (diff > 0.0)
	{
		return 1;
	}
	else if (diff < 0.0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// SetBoundingBox 
/////////////////////////////////////////////////////////////////////////////
void CPsRenderer::SetBoundingBox(LPRECT pBoundingBox)
{
	ASSERT(pBoundingBox);
	ASSERT(pBoundingBox->left < pBoundingBox->right);
	ASSERT(pBoundingBox->top < pBoundingBox->bottom);

	m_pBoundingBox = pBoundingBox;
}
