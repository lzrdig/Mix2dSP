/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: February 2001
//
// based on a sample of
// Pierre Alliez <pierre.alliez@cnet.francetelecom.fr>
// adapted from:
//   Frederic Delhoume
//   Mark Kilgards 
// Code below is nothing more than Mark J. Kilgard's code
// adapted for C++/MFC
//
/////////////////////////////////////////////////////////////////////////////
// PsRenderer.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PS_RENDERER_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_)
#define AFX_PS_RENDERER_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_

#define EPS_SMOOTH_LINE_FACTOR	0.06
#define EPS_GOURAUD_THRESHOLD	0.05
#define EPS_LINE_WIDTH			0.1

class CPsRenderer
{
public:
	CPsRenderer();
	virtual ~CPsRenderer() {}

	void Run(LPCTSTR lpszFilename, BOOL bSort, int nValues, GLfloat *pFeedbackBuffer);
	void SetBoundingBox(LPRECT pBoundingBox);
	void SetOrientation(short dmOrientation) { m_dmOrientation = dmOrientation; }

private:
	void SpewWireFrameEPS(FILE* stream, LPCTSTR lpszFilename, BOOL bSort,GLint size, GLfloat* buffer);
	void SpewUnsortedFeedback(FILE* stream, GLint size, GLfloat* buffer);
	void SpewSortedFeedback(FILE* stream, GLint size, GLfloat* buffer);
	GLfloat* SpewPrimitiveEPS(FILE* stream, GLfloat* buffer);
	static int Compare(const void* a, const void* b);

	GLfloat m_PointSize;
	LPRECT m_pBoundingBox;
	short m_dmOrientation;

	typedef struct _DepthIndex
	{
		GLfloat *ptr;
		GLfloat depth;
	}
	DepthIndex;

	// OpenGL's GL_3D_COLOR feedback vertex format.
	typedef struct _Feedback3Dcolor
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;
	}
	Feedback3Dcolor;
};

static char *NiceGouraudTriangleEPS[] =
{
	"\n% Smooth-shaded triangle - x3 y3 r3 g3 b3 x2 y2 r2 g2 b2 x1 y1 r1 g1 b1 ST",
	"/ST {",
	"	/b1 exch def",
	"	/g1 exch def",
	"	/r1 exch def",
	"	/y1 exch def",
	"	/x1 exch def",
	"	/b2 exch def",
	"	/g2 exch def",
	"	/r2 exch def",
	"	/y2 exch def",
	"	/x2 exch def",
	"	/b3 exch def",
	"	/g3 exch def",
	"	/r3 exch def",
	"	/y3 exch def",
	"	/x3 exch def",
	"	b2 b1 sub abs 0.05 gt",
	"	g2 g1 sub abs 0.017 gt",
	"	r2 r1 sub abs 0.032 gt",
	"	b3 b1 sub abs 0.05 gt",
	"	g3 g1 sub abs 0.017 gt",
	"	r3 r1 sub abs 0.032 gt",
	"	b2 b3 sub abs 0.05 gt",
	"	g2 g3 sub abs 0.017 gt",
	"	r2 r3 sub abs 0.032 gt",
	"	or or or or or or or or {",
	"		/b12 b1 b2 add 0.5 mul def",
	"		/g12 g1 g2 add 0.5 mul def",
	"		/r12 r1 r2 add 0.5 mul def",
	"		/y12 y1 y2 add 0.5 mul def",
	"		/x12 x1 x2 add 0.5 mul def",
	"		/b13 b1 b3 add 0.5 mul def",
	"		/g13 g1 g3 add 0.5 mul def",
	"		/r13 r1 r3 add 0.5 mul def",
	"		/y13 y1 y3 add 0.5 mul def",
	"		/x13 x1 x3 add 0.5 mul def",
	"		/b32 b3 b2 add 0.5 mul def",
	"		/g32 g3 g2 add 0.5 mul def",
	"		/r32 r3 r2 add 0.5 mul def",
	"		/y32 y3 y2 add 0.5 mul def",
	"		/x32 x3 x2 add 0.5 mul def",
	"		x1 y1 r1 g1 b1 x12 y12 r12 g12 b12 x13 y13 r13 g13 b13",
	"		x2 y2 r2 g2 b2 x12 y12 r12 g12 b12 x32 y32 r32 g32 b32",
	"		x3 y3 r3 g3 b3 x32 y32 r32 g32 b32 x13 y13 r13 g13 b13",
	"		x32 y32 r32 g32 b32 x12 y12 r12 g12 b12 x13 y13 r13 g13 b13",
	"		ST ST ST ST",
	"	} {",
	"		x1 y1 x2 y2 x3 y3 r1 g1 b1 T",
	"	} ifelse",
	"} bind def",
	"\n",
	NULL
};

static char *GouraudTriangleEPS[] =
{
	"/bd{bind def}bind def /triangle { aload pop   setrgbcolor  aload pop 5 3",
	"roll 4 2 roll 3 2 roll exch moveto lineto lineto closepath fill } bd",
	"/computediff1 { 2 copy sub abs threshold ge {pop pop pop true} { exch 2",
	"index sub abs threshold ge { pop pop true} { sub abs threshold ge } ifelse",
	"} ifelse } bd /computediff3 { 3 copy 0 get 3 1 roll 0 get 3 1 roll 0 get",
	"computediff1 {true} { 3 copy 1 get 3 1 roll 1 get 3 1 roll 1 get",
	"computediff1 {true} { 3 copy 2 get 3 1 roll  2 get 3 1 roll 2 get",
	"computediff1 } ifelse } ifelse } bd /middlecolor { aload pop 4 -1 roll",
	"aload pop 4 -1 roll add 2 div 5 1 roll 3 -1 roll add 2 div 3 1 roll add 2",
	"div 3 1 roll exch 3 array astore } bd /gouraudtriangle { computediff3 { 4",
	"-1 roll aload 7 1 roll 6 -1 roll pop 3 -1 roll pop add 2 div 3 1 roll add",
	"2 div exch 3 -1 roll aload 7 1 roll exch pop 4 -1 roll pop add 2 div 3 1",
	"roll add 2 div exch 3 -1 roll aload 7 1 roll pop 3 -1 roll pop add 2 div 3",
	"1 roll add 2 div exch 7 3 roll 10 -3 roll dup 3 index middlecolor 4 1 roll",
	"2 copy middlecolor 4 1 roll 3 copy pop middlecolor 4 1 roll 13 -1 roll",
	"aload pop 17 index 6 index 15 index 19 index 6 index 17 index 6 array",
	"astore 10 index 10 index 14 index gouraudtriangle 17 index 5 index 17",
	"index 19 index 5 index 19 index 6 array astore 10 index 9 index 13 index",
	"gouraudtriangle 13 index 16 index 5 index 15 index 18 index 5 index 6",
	"array astore 12 index 12 index 9 index gouraudtriangle 17 index 16 index",
	"15 index 19 index 18 index 17 index 6 array astore 10 index 12 index 14",
	"index gouraudtriangle 18 {pop} repeat } { aload pop 5 3 roll aload pop 7 3",
	"roll aload pop 9 3 roll 8 index 6 index 4 index add add 3 div 10 1 roll 7",
	"index 5 index 3 index add add 3 div 10 1 roll 6 index 4 index 2 index add",
	"add 3 div 10 1 roll 9 {pop} repeat 3 array astore triangle } ifelse } bd",
	NULL
};

#endif // AFX_PS_RENDERER_H__A15D9040_F98E_11D3_8B6F_00001CD5E4D1__INCLUDED_
