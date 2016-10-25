/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// Matrix.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Matrix CMatrix 

class CIntMatrix;

class AFX_EXT_CLASS_GENERAL CMatrix : public CArray<double, double>
{
public:
	typedef enum tagKINDOFORDER
	{
		UNKNOWN_ORDER = 0,		// treated like FORTRAN_ORDER, for serialization only
		FORTRAN_ORDER = 1,
		C_ORDER = 2
	}
	KINDOFORDER;

	typedef enum tagKINDOFDIMERROR
	{
		ERROR_MATRIX_DIM_EQUAL = 1,
		ERROR_MATRIX_DIM_MULT = 2,
		ERROR_MATRIX_DIM_CONCAT_HOR = 3,
		ERROR_MATRIX_DIM_CONCAT_VERT = 4,
		ERROR_MATRIX_DIM_INDEX_BOTH = 5,
		ERROR_MATRIX_DIM_INDEX_ROW = 6,
		ERROR_MATRIX_DIM_INDEX_COL = 7,
		ERROR_MATRIX_DIM_INDEX_RESHAPE = 8
	}
	KINDOFDIMERROR;

	CMatrix(const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
	CMatrix(const double& inValue, const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
	CMatrix(const double& firstValue, const double& lastValue, const double& stepSize = 1., const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
	CMatrix(const CSize& inSize, const double& initValue = 0., const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
	CMatrix(const CMatrix& inMatrix);
	CMatrix(const CIntMatrix& inMatrix);
	CMatrix(const CMatrix& inMatrix, const KINDOFORDER& nKindOfOrder);
	CMatrix(const CIntMatrix& inMatrix, const KINDOFORDER& nKindOfOrder);

	void CopyContents(const CMatrix& inMatrix);
	void CopyContents(const CIntMatrix& inMatrix);

	int GetTotalSize() const;
	int GetLength() const;
	void Empty();
	BOOL IsEmpty() const;
	BOOL IsVector() const;

	void DimensionErrorMessage(KINDOFDIMERROR i, CSize sizeOne, CSize sizeTwo) const;
	void DimensionErrorMessage(KINDOFDIMERROR i, CSize sizeOne, CSize sizeTwo, CSize sizeThree) const;

	KINDOFORDER GetOrder() const { ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER); return m_nKindOfOrder; };
	void SetOrder(KINDOFORDER nKindOfOrder);
	void SetOrder(KINDOFORDER nKindOfOrder, BOOL bReArrange);
	void transp();

 	friend AFX_EXT_API_GENERAL CArchive& AFXAPI operator<<(CArchive& ar, CMatrix& matrix);
 	friend AFX_EXT_API_GENERAL CArchive& AFXAPI operator>>(CArchive& ar, CMatrix& matrix);

	double operator()(const int& nSubscriptX, const int& nSubscriptY) const;
	double& operator()(const int& nSubscriptX, const int& nSubscriptY);
	CMatrix operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const;

	CMatrix& operator=(const double& inValue);
	CMatrix& operator=(const CMatrix& inMatrix);
	CMatrix& operator=(const CIntMatrix& inMatrix);

	void operator+=(const double& inValue);
	void operator-=(const double& inValue);
	void operator*=(const double& inValue);
	void operator/=(const double& inValue);

	void operator+=(const CMatrix& inMatrix);
	void operator-=(const CMatrix& inMatrix);
	void DotTimesAssign(const CMatrix& inMatrix);
	void DotOverAssign(const CMatrix& inMatrix);
	void operator*=(const CMatrix& inMatrix);

	void operator+=(const CIntMatrix& inMatrix);
	void operator-=(const CIntMatrix& inMatrix);
	void DotTimesAssign(const CIntMatrix& inMatrix);
	void DotOverAssign(const CIntMatrix& inMatrix);

	CMatrix operator+(const int& inValue) const;
	CMatrix operator-(const int& inValue) const;
	CMatrix operator*(const int& inValue) const;
	CMatrix operator/(const int& inValue) const;
	CIntMatrix operator<(const int& inValue) const;
	CIntMatrix operator>(const int& inValue) const;
	CIntMatrix operator<=(const int& inValue) const;
	CIntMatrix operator>=(const int& inValue) const;
	CIntMatrix operator==(const int& inValue) const;
	CIntMatrix operator!=(const int& inValue) const;

	friend AFX_EXT_API_GENERAL CMatrix operator+(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator-(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator*(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator/(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator<(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator>(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator<=(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator>=(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator==(const int& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator!=(const int& inValue, const CMatrix& inMatrix);

	CMatrix operator+(const double& inValue) const;
	CMatrix operator-(const double& inValue) const;
	CMatrix operator*(const double& inValue) const;
	CMatrix operator/(const double& inValue) const;
	CIntMatrix operator<(const double& inValue) const;
	CIntMatrix operator>(const double& inValue) const;
	CIntMatrix operator<=(const double& inValue) const;
	CIntMatrix operator>=(const double& inValue) const;
	CIntMatrix operator==(const double& inValue) const;
	CIntMatrix operator!=(const double& inValue) const;

	friend AFX_EXT_API_GENERAL CMatrix operator+(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator-(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator*(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CMatrix operator/(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator<(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator>(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator<=(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator>=(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator==(const double& inValue, const CMatrix& inMatrix);
	friend AFX_EXT_API_GENERAL CIntMatrix operator!=(const double& inValue, const CMatrix& inMatrix);

	CMatrix operator+(const CMatrix& inMatrix) const;
	CMatrix operator-(const CMatrix& inMatrix) const;
	CMatrix DotTimes(const CMatrix& inMatrix) const;
	CMatrix DotOver(const CMatrix& inMatrix) const;
	CIntMatrix operator<(const CMatrix& inMatrix) const;
	CIntMatrix operator>(const CMatrix& inMatrix) const;
	CIntMatrix operator<=(const CMatrix& inMatrix) const;
	CIntMatrix operator>=(const CMatrix& inMatrix) const;
	CIntMatrix operator==(const CMatrix& inMatrix) const;
	CIntMatrix operator!=(const CMatrix& inMatrix) const;

	CMatrix operator+(const CIntMatrix& inMatrix) const;
	CMatrix operator-(const CIntMatrix& inMatrix) const;
	CMatrix DotTimes(const CIntMatrix& inMatrix) const;
	CMatrix DotOver(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>(const CIntMatrix& inMatrix) const;
	CIntMatrix operator<=(const CIntMatrix& inMatrix) const;
	CIntMatrix operator>=(const CIntMatrix& inMatrix) const;
	CIntMatrix operator==(const CIntMatrix& inMatrix) const;
	CIntMatrix operator!=(const CIntMatrix& inMatrix) const;

	CMatrix operator-() const;
	CMatrix operator*(const CMatrix& inMatrix) const;
	CMatrix operator*(const CIntMatrix& inMatrix) const;

	BOOL reshape(const int& nNewSizeX, const int& nNewSizeY);
	BOOL reshape(const CSize& nNewSize);
	BOOL reshape();

	CMatrix GetRows(const int& nSubscriptX) const;
	CMatrix GetColumns(const int& nSubscriptY) const;
	CMatrix GetRows(const CIntMatrix& nSubscriptMatrixX) const;
	CMatrix GetColumns(const CIntMatrix& nSubscriptMatrixY) const;

protected:
	int m_nSizeX, m_nSizeY;
	KINDOFORDER m_nKindOfOrder;

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CMatrix)
	public:
	virtual void SetSize(int nNewSize, int nGrowBy = -1);
	virtual void SetSize(CSize nNewSize, int nGrowBy = -1);
	virtual CSize GetSize() const;
	virtual void Serialize(CArchive& ar, unsigned char numBytes = 8);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CMatrix();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CMatrix)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////
// Mathematische CMatrix Funktionen
AFX_EXT_API_GENERAL CMatrix acos(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix asin(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix atan(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix atan2(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix atan2(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix atan2(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix ceil(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix cos(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix cosh(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix exp(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix fabs(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix fix(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix floor(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix fmod(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix fmod(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix fmod(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix frem(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix frem(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix frem(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix log(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix log10(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix pow(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix pow(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix pow(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix round(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix sign(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix sin(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix sinh(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix sqrt(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix tan(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix tanh(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix arcreset(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix arcreset(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix arcreset(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL double matMaxFinite(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double matMinFinite(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double matMax(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double matMin(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix matMax(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix matMax(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix matMax(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix matMin(const CMatrix& inMatrix, const double& inValue);
AFX_EXT_API_GENERAL CMatrix matMin(const double& inValue, const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix matMin(const CMatrix& inMatrix1, const CMatrix& inMatrix2);

AFX_EXT_API_GENERAL CIntMatrix _finite(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CIntMatrix _isnan(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CIntMatrix _isinf(const CMatrix& inMatrix);

/////////////////////////////////////////////////////////////////////////////
// Andere CMatrix Funktionen
AFX_EXT_API_GENERAL CMatrix transp(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix reverse(const CMatrix& inMatrix, BOOL bReverseRows, BOOL bReverseColumns);
AFX_EXT_API_GENERAL CMatrix reshape(const CMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY);
AFX_EXT_API_GENERAL CMatrix reshape(const CMatrix& inMatrix, const CSize& nNewSize);
AFX_EXT_API_GENERAL CMatrix reshape(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix concatHor(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix concatVert(const CMatrix& inMatrix1, const CMatrix& inMatrix2);
AFX_EXT_API_GENERAL CMatrix rowSum(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix colSum(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix rowProd(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL CMatrix colProd(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double sum(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double prod(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double mean(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double meanFinite(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double rms(const CMatrix& inMatrix);
AFX_EXT_API_GENERAL double rmsFinite(const CMatrix& inMatrix);

AFX_EXT_API_GENERAL void cart2pol(const CMatrix& X, const CMatrix& Y, CMatrix& Alpha, CMatrix& Radius);
AFX_EXT_API_GENERAL void cart2sph(const CMatrix& X, const CMatrix& Y, const CMatrix& Z, CMatrix& Alpha, CMatrix& Theta, CMatrix& Radius);
AFX_EXT_API_GENERAL void pol2cart(const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y);
AFX_EXT_API_GENERAL void sph2cart(const CMatrix& Alpha, const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y, CMatrix& Z);

AFX_EXT_API_GENERAL void huge2value(CMatrix& inMatrix, double value);

AFX_EXT_API_GENERAL void reduceSize(CMatrix& inMatrix, const int& maxTotal);
AFX_EXT_API_GENERAL void reduceSize(CMatrix& inMatrix, const int& maxRows, const int& maxColumns);

AFX_EXT_API_GENERAL BOOL LinSolveGaussSimple(const CMatrix& A, const CMatrix& b, CMatrix& x);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_MATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
