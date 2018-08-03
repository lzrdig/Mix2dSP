/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// IntMatrix.h : Header-Datei
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
#define AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// Matrix CIntMatrix 
namespace mymathfuncs
{
	class CMatrix;

	class AFX_EXT_CLASS_GENERAL CIntMatrix : public CArray<int, int>
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

		CIntMatrix(const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
		CIntMatrix(const int& inValue, const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
		CIntMatrix(const int& firstValue, const int& lastValue, const double& stepSize = 1., const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
		CIntMatrix(const CSize& inSize, const int& initValue = 0, const KINDOFORDER& nKindOfOrder = FORTRAN_ORDER);
		CIntMatrix(const CIntMatrix& inMatrix);
		CIntMatrix(const CMatrix& inMatrix);
		CIntMatrix(const CIntMatrix& inMatrix, const KINDOFORDER& nKindOfOrder);
		CIntMatrix(const CMatrix& inMatrix, const KINDOFORDER& nKindOfOrder);

		void CopyContents(const CIntMatrix& inMatrix);
		void CopyContents(const CMatrix& inMatrix);

		int GetTotalSize() const;
		int GetLength() const;
		void Empty();
		BOOL IsEmpty() const;
		BOOL IsVector() const;

		void DimensionErrorMessage(KINDOFDIMERROR k, CSize sizeOne, CSize sizeTwo) const;

		KINDOFORDER GetOrder() const { ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER); return m_nKindOfOrder; };
		void SetOrder(KINDOFORDER nKindOfOrder);
		void SetOrder(KINDOFORDER nKindOfOrder, BOOL bReArrange);
		void transp();

		friend AFX_EXT_API_GENERAL CArchive& AFXAPI operator<<(CArchive& ar, CIntMatrix& matrix);
		friend AFX_EXT_API_GENERAL CArchive& AFXAPI operator >> (CArchive& ar, CIntMatrix& matrix);

		int operator()(const int& nSubscriptX, const int& nSubscriptY) const;
		int& operator()(const int& nSubscriptX, const int& nSubscriptY);
		CIntMatrix operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const;

		CIntMatrix& operator=(const int& inValue);
		CIntMatrix& operator=(const CIntMatrix& inMatrix);
		CIntMatrix& operator=(const CMatrix& inMatrix);

		void operator+=(const int& inValue);
		void operator-=(const int& inValue);
		void operator*=(const int& inValue);
		void operator/=(const int& inValue);

		void operator+=(const CIntMatrix& inMatrix);
		void operator-=(const CIntMatrix& inMatrix);
		void DotTimesAssign(const CIntMatrix& inMatrix);
		void DotOverAssign(const CIntMatrix& inMatrix);
		void operator*=(const CIntMatrix& inMatrix);

		CIntMatrix operator+(const int& inValue) const;
		CIntMatrix operator-(const int& inValue) const;
		CIntMatrix operator*(const int& inValue) const;
		CIntMatrix operator/(const int& inValue) const;
		CIntMatrix operator<(const int& inValue) const;
		CIntMatrix operator>(const int& inValue) const;
		CIntMatrix operator<=(const int& inValue) const;
		CIntMatrix operator>=(const int& inValue) const;
		CIntMatrix operator==(const int& inValue) const;
		CIntMatrix operator!=(const int& inValue) const;

		friend AFX_EXT_API_GENERAL CIntMatrix operator+(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator-(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator*(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator/(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator<(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator>(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator<=(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator>=(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator==(const int& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator!=(const int& inValue, const CIntMatrix& inMatrix);

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

		friend AFX_EXT_API_GENERAL CMatrix operator+(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CMatrix operator-(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CMatrix operator*(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CMatrix operator/(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator<(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator>(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator<=(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator>=(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator==(const double& inValue, const CIntMatrix& inMatrix);
		friend AFX_EXT_API_GENERAL CIntMatrix operator!=(const double& inValue, const CIntMatrix& inMatrix);

		CIntMatrix operator+(const CIntMatrix& inMatrix) const;
		CIntMatrix operator-(const CIntMatrix& inMatrix) const;
		CIntMatrix DotTimes(const CIntMatrix& inMatrix) const;
		CIntMatrix DotOver(const CIntMatrix& inMatrix) const;
		CIntMatrix operator<(const CIntMatrix& inMatrix) const;
		CIntMatrix operator>(const CIntMatrix& inMatrix) const;
		CIntMatrix operator<=(const CIntMatrix& inMatrix) const;
		CIntMatrix operator>=(const CIntMatrix& inMatrix) const;
		CIntMatrix operator==(const CIntMatrix& inMatrix) const;
		CIntMatrix operator!=(const CIntMatrix& inMatrix) const;

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

		CIntMatrix operator-() const;
		CIntMatrix operator!() const;
		CIntMatrix operator*(const CIntMatrix& inMatrix) const;
		CMatrix operator*(const CMatrix& inMatrix) const;

		BOOL reshape(const int& nNewSizeX, const int& nNewSizeY);
		BOOL reshape(const CSize& nNewSize);
		BOOL reshape();

		CIntMatrix GetRows(const int& nSubscriptX) const;
		CIntMatrix GetColumns(const int& nSubscriptY) const;
		CIntMatrix GetRows(const CIntMatrix& nSubscriptMatrixX) const;
		CIntMatrix GetColumns(const CIntMatrix& nSubscriptMatrixY) const;

	protected:
		int m_nSizeX, m_nSizeY;
		KINDOFORDER m_nKindOfOrder;

		// Überschreibungen
			// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
			//{{AFX_VIRTUAL(CIntMatrix)
	public:
		virtual void SetSize(int nNewSize, int nGrowBy = -1);
		virtual void SetSize(CSize nNewSize, int nGrowBy = -1);
		virtual CSize GetSize() const;
		virtual void Serialize(CArchive& ar, unsigned char numBytes = 4);
		//}}AFX_VIRTUAL

	// Implementierung
	public:
		virtual ~CIntMatrix();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

		// Generierte Nachrichtenzuordnungsfunktionen
	protected:
		//{{AFX_MSG(CIntMatrix)
			// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
		//}}AFX_MSG
	};

	/////////////////////////////////////////////////////////////////////////////
	// Mathematische CIntMatrix Funktionen
	AFX_EXT_API_GENERAL CIntMatrix abs(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL int matMax(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL int matMin(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix matMax(const CIntMatrix& inMatrix, const int& inValue);
	AFX_EXT_API_GENERAL CIntMatrix matMax(const int& inValue, const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix matMax(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
	AFX_EXT_API_GENERAL CIntMatrix matMin(const CIntMatrix& inMatrix, const int& inValue);
	AFX_EXT_API_GENERAL CIntMatrix matMin(const int& inValue, const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix matMin(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);

	/////////////////////////////////////////////////////////////////////////////
	// Andere CIntMatrix Funktionen
	AFX_EXT_API_GENERAL CIntMatrix transp(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix reverse(const CIntMatrix& inMatrix, BOOL bReverseRows, BOOL bReverseColumns);
	AFX_EXT_API_GENERAL CIntMatrix reshape(const CIntMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY);
	AFX_EXT_API_GENERAL CIntMatrix reshape(const CIntMatrix& inMatrix, const CSize& nNewSize);
	AFX_EXT_API_GENERAL CIntMatrix reshape(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix concatHor(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
	AFX_EXT_API_GENERAL CIntMatrix concatVert(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2);
	AFX_EXT_API_GENERAL CIntMatrix rowSum(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix colSum(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix rowProd(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL CIntMatrix colProd(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL int sum(const CIntMatrix& inMatrix);
	AFX_EXT_API_GENERAL int prod(const CIntMatrix& inMatrix);

	AFX_EXT_API_GENERAL void reduceSize(CIntMatrix& inMatrix, const int& maxTotal);
	AFX_EXT_API_GENERAL void reduceSize(CIntMatrix& inMatrix, const int& maxRows, const int& maxColumns);
}
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_INTMATRIX_H__09EDD581_CDA3_11D1_B619_0000C0F55FDF__INCLUDED_)
