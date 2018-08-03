/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// Matrix.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Matrix.h"
#include "IntMatrix.h"
#include "MathAdd.h"
#include "Resource.h"

//#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Remark

// Note: We replaced the string "int sizeX = GetSize().cx, sizeY = GetSize().cy;"
// globally by "const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;".
// This was done to get faster access to the size of the matrix.

/////////////////////////////////////////////////////////////////////////////
// CMatrix
namespace mymathfuncs
{
CMatrix::CMatrix(const KINDOFORDER& nKindOfOrder)
{
	// this is an empty matrix
	SetOrder(nKindOfOrder);
	Empty();
}

CMatrix::CMatrix(const double& inValue, const KINDOFORDER& nKindOfOrder)
{
	// copy inValue
	SetOrder(nKindOfOrder);
	SetSize(1);
	SetAt(0, inValue);
}

CMatrix::CMatrix(const double& firstValue, const double& lastValue, const double& stepSize, const KINDOFORDER& nKindOfOrder)
{
	// setup row matrix
	SetOrder(nKindOfOrder);
	if (stepSize == 0.)
		Empty();
	else if ((lastValue-firstValue)/stepSize < 0.)
		Empty();
	else
	{
		SetSize(CSize(1, int(::ceil((double)(lastValue-firstValue+stepSize)/stepSize))));
		int size = GetTotalSize();
		for (int k = 0; k < size-1; k++)
			SetAt(k, firstValue + k*stepSize);
		SetAt(size-1, lastValue);
	}
}

CMatrix::CMatrix(const CSize& inSize, const double& initValue, const KINDOFORDER& nKindOfOrder)
{
	// initialize with initValue
	SetOrder(nKindOfOrder);
	SetSize(inSize);
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, initValue);
}

CMatrix::CMatrix(const CMatrix& inMatrix)
{
	// copy inMatrix
	SetOrder(inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inMatrix.GetAt(k));
}

CMatrix::CMatrix(const CIntMatrix& inMatrix)
{
	// copy inMatrix
	SetOrder((CMatrix::KINDOFORDER)inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, double(inMatrix.GetAt(k)));
}

CMatrix::CMatrix(const CMatrix& inMatrix, const KINDOFORDER& nKindOfOrder)
{
	// copy inMatrix
	SetOrder(nKindOfOrder);
	CopyContents(inMatrix);
}

CMatrix::CMatrix(const CIntMatrix& inMatrix, const KINDOFORDER& nKindOfOrder)
{
	// copy inMatrix
	SetOrder(nKindOfOrder);
	CopyContents(inMatrix);
}

CMatrix::~CMatrix()
{
}

//{{AFX_MSG_MAP(CMatrix)
	// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// Diagnose CMatrix

#ifdef _DEBUG
void CMatrix::AssertValid() const
{
	CArray<double, double>::AssertValid();

	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);
}

void CMatrix::Dump(CDumpContext& dc) const
{
	CArray<double, double>::Dump(dc);

	dc << "\nm_nSizeX = " << m_nSizeX;
	dc << "\nm_nSizeY = " << m_nSizeY;
	dc << "\nm_nKindOfOrder = " << m_nKindOfOrder;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Überladungen CMatrix

void CMatrix::CopyContents(const CMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());

	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			(*this)(i,j) = inMatrix(i,j);
}

void CMatrix::CopyContents(const CIntMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());

	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			(*this)(i,j) = (double)inMatrix(i,j);
}

void CMatrix::SetSize(int nNewSize, int nGrowBy)
{
	// format it as a column vector
	int sizeX = nNewSize;
	int sizeY = 1;
	if (sizeX <= 0)
		sizeX = sizeY = 0;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	CArray<double, double>::SetSize(sizeX*sizeY, nGrowBy);
}

void CMatrix::SetSize(CSize nNewSize, int nGrowBy)
{
	int sizeX = nNewSize.cx;
	int sizeY = nNewSize.cy;
	if (sizeX <= 0 || sizeY <= 0)
		sizeX = sizeY = 0;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	CArray<double, double>::SetSize(sizeX*sizeY, nGrowBy);
}

inline CSize CMatrix::GetSize() const
{
	return CSize(m_nSizeX, m_nSizeY);
}

inline int CMatrix::GetTotalSize() const
{
	return m_nSizeX * m_nSizeY;
}

inline int CMatrix::GetLength() const
{
	return max(m_nSizeX, m_nSizeY);
}

void CMatrix::SetOrder(KINDOFORDER nKindOfOrder)
{
	ASSERT(nKindOfOrder == UNKNOWN_ORDER || nKindOfOrder == FORTRAN_ORDER || nKindOfOrder == C_ORDER);
	if (nKindOfOrder == UNKNOWN_ORDER)
		nKindOfOrder = FORTRAN_ORDER;

	m_nKindOfOrder = nKindOfOrder;
}

void CMatrix::SetOrder(KINDOFORDER nKindOfOrder, BOOL bReArrange)
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	if (m_nKindOfOrder == nKindOfOrder)
		return;

	if (bReArrange)
	{
		CMatrix tmpMatrix(*this, nKindOfOrder);
		*this = tmpMatrix;
	}
	else
		SetOrder(nKindOfOrder);
}

void CMatrix::transp()
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	int tmpInt, *pKindOfOrder = (int*)&m_nKindOfOrder;

	*pKindOfOrder *= -1;
	*pKindOfOrder += 3;

	tmpInt = m_nSizeX;
	m_nSizeX = m_nSizeY;
	m_nSizeY = tmpInt;
}

/////////////////////////////////////////////////////////////////////////////
// CMatrix Serialisierung

void CMatrix::Serialize(CArchive& ar, unsigned char numByte)
{
	ASSERT(numByte == 1 || numByte == 2 || numByte == 4 || numByte == 8);
	int i, j, sizeX, sizeY;
	double tmpDouble, minVal = 0., maxVal = 0.;
	unsigned char tmpChar;
	unsigned short tmpShort;
	float tmpFloat;

	if (ar.IsStoring())
	{
		tmpChar = char((m_nKindOfOrder << 4) | numByte);
		sizeX = GetSize().cx;
		sizeY = GetSize().cy;

		ar << tmpChar;
		ar << sizeX;
		ar << sizeY;
		if (numByte < 4)
		{
			minVal = matMinFinite(*this);
			maxVal = matMaxFinite(*this);
			ar << minVal;
			ar << maxVal;
		}
		for (j = 1; j <= sizeY; j++)
		{
			for (i = 1; i <= sizeX; i++)
			{
				tmpDouble = (*this)(i,j);
				switch (numByte)
				{
				case 1:
					if (::_isnan(tmpDouble))
						tmpChar = 255;
					else if (tmpDouble == HUGE)
						tmpChar = 254;
					else if (tmpDouble == -HUGE)
						tmpChar = 0;
					else
						tmpChar = (unsigned char)(252.*(tmpDouble-minVal)/(maxVal != minVal ? maxVal-minVal : 1.) + 1.);
					ar << tmpChar;
					break;
				case 2:
					if (::_isnan(tmpDouble))
						tmpShort = 65535;
					else if (tmpDouble == HUGE)
						tmpShort = 65534;
					else if (tmpDouble == -HUGE)
						tmpShort = 0;
					else
						tmpShort = (unsigned short)(65532.*(tmpDouble-minVal)/(maxVal != minVal ? maxVal-minVal : 1.) + 1.);
					ar << tmpShort;
					break;
				case 4:
					tmpFloat = (float)tmpDouble;
					ar << tmpFloat;
					break;
				case 8:
					ar << tmpDouble;
					break;
				default:
					{}
				}
			}
		}
	}
	else
	{
		KINDOFORDER nKindOfOrder;
		BOOL bIsUnknownOrder = FALSE;

		ar >> tmpChar;
		numByte = unsigned char(tmpChar & 0x0F);
		nKindOfOrder = (KINDOFORDER)(tmpChar >> 4);
		if (nKindOfOrder == UNKNOWN_ORDER)
			bIsUnknownOrder = TRUE;
		SetOrder(nKindOfOrder);

		ar >> sizeX;
		ar >> sizeY;
		if (numByte < 4)
		{
			ar >> minVal;
			ar >> maxVal;
		}
		SetSize(CSize(sizeX,sizeY));
		for (j = 1; j <= sizeY; j++)
		{
			for (i = 1; i <= sizeX; i++)
			{
				switch (numByte)
				{
				case 1:
					ar >> tmpChar;
					if (bIsUnknownOrder)
					{
						// old style (not able to handle QNAN)
						if (tmpChar == 255)
							(*this)(i,j) = HUGE;
						else if (tmpChar == 0)
							(*this)(i,j) = -HUGE;
						else
							(*this)(i,j) = (tmpChar-1)/253.*(maxVal-minVal) + minVal;
					}
					else
					{
						// new style (able to handle QNAN)
						if (tmpChar == 255)
							(*this)(i,j) = QNAN;
						else if (tmpChar == 254)
							(*this)(i,j) = HUGE;
						else if (tmpChar == 0)
							(*this)(i,j) = -HUGE;
						else
							(*this)(i,j) = (tmpChar-1)/252.*(maxVal-minVal) + minVal;
					}
					break;
				case 2:
					ar >> tmpShort;
					if (bIsUnknownOrder)
					{
						// old style (not able to handle QNAN)
						if (tmpShort == 65535)
							(*this)(i,j) = HUGE;
						else if (tmpShort == 0)
							(*this)(i,j) = -HUGE;
						else
							(*this)(i,j) = (tmpShort-1)/65533.*(maxVal-minVal) + minVal;
					}
					else
					{
						// new style (able to handle QNAN)
						if (tmpShort == 65535)
							(*this)(i,j) = QNAN;
						else if (tmpShort == 65534)
							(*this)(i,j) = HUGE;
						else if (tmpShort == 0)
							(*this)(i,j) = -HUGE;
						else
							(*this)(i,j) = (tmpShort-1)/65532.*(maxVal-minVal) + minVal;
					}
					break;
				case 4:
					ar >> tmpFloat;
					(*this)(i,j) = tmpFloat;
					break;
				case 8:
					ar >> (*this)(i,j);
					break;
				default:
					{}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Befehle CMatrix 
inline void CMatrix::Empty()
{
	SetSize(0);
}

BOOL CMatrix::IsEmpty() const
{
	return (GetTotalSize() == 0);
}

BOOL CMatrix::IsVector() const
{
	return (GetSize().cx == 1 || GetSize().cy == 1);
}

void CMatrix::DimensionErrorMessage(KINDOFDIMERROR k, CSize sizeOne, CSize sizeTwo) const
{
	CString mMsg;
	switch (k)
	{
	case ERROR_MATRIX_DIM_EQUAL:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_EQUAL, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_MULT:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_MULT, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_CONCAT_HOR:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_CONCAT_HOR, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_CONCAT_VERT:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_CONCAT_VERT, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_INDEX_BOTH:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_INDEX_BOTH, sizeTwo.cx, sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_INDEX_ROW:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_INDEX_ROW, sizeTwo.cx, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_INDEX_COL:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_INDEX_COL, sizeTwo.cy, sizeOne.cx, sizeOne.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	case ERROR_MATRIX_DIM_INDEX_RESHAPE:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_INDEX_RESHAPE, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	default:
		{}
	}
}

void CMatrix::DimensionErrorMessage(KINDOFDIMERROR k, CSize sizeOne, CSize sizeTwo, CSize sizeThree) const
{
	CString mMsg;
	switch (k)
	{
	case ERROR_MATRIX_DIM_EQUAL:
		mMsg.Format(IDS_ERROR_MATRIX_DIM_EQUAL_THREE, sizeOne.cx, sizeOne.cy, sizeTwo.cx, sizeTwo.cy, sizeThree.cx, sizeThree.cy);
		//AfxMessageBox(mMsg, MB_ICONERROR);
		TRACE0(mMsg);
		break;
	default:
		{}
	}
}

CArchive& AFXAPI operator<<(CArchive& ar, CMatrix& matrix)	// friend function of CMatrix
{
	matrix.Serialize(ar);
	return ar;
}

CArchive& AFXAPI operator>>(CArchive& ar, CMatrix& matrix)	// friend function of CMatrix
{
	matrix.Serialize(ar);
	return ar;
}

double CMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY) const
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
	if (nSubscriptX >= 1 && nSubscriptX <= sizeX && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
	{
		if (m_nKindOfOrder == FORTRAN_ORDER)
			return (*this)[(nSubscriptY-1)*sizeX + nSubscriptX-1];
		else /*if (m_nKindOfOrder == C_ORDER)*/
			return (*this)[(nSubscriptX-1)*sizeY + nSubscriptY-1];
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_BOTH, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return 0.;
	}
}

double& CMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY)
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	static double ret;
	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;

	if (nSubscriptX >= 1 && nSubscriptX <= sizeX && nSubscriptY >= 1 && nSubscriptY <= GetSize().cy)
	{
		if (m_nKindOfOrder == FORTRAN_ORDER)
			return (*this)[(nSubscriptY-1)*sizeX + nSubscriptX-1];
		else /*if (m_nKindOfOrder == C_ORDER)*/
			return (*this)[(nSubscriptX-1)*sizeY + nSubscriptY-1];
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_BOTH, GetSize(), CSize(nSubscriptX, nSubscriptY));
		return ret = 0.;
	}
}

CMatrix CMatrix::operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const
{
	CMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	if (nSubscriptMatrixX.GetSize() == nSubscriptMatrixY.GetSize())
	{
		outMatrix.SetSize(nSubscriptMatrixX.GetSize());
		int sizeX = outMatrix.GetSize().cx, sizeY = outMatrix.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = (*this)(nSubscriptMatrixX(i,j), nSubscriptMatrixY(i,j));
	}
	else if (nSubscriptMatrixX.GetTotalSize() > 0 && nSubscriptMatrixY.GetTotalSize() == 1)
	{
		outMatrix.SetSize(nSubscriptMatrixX.GetSize());
		int sizeX = outMatrix.GetSize().cx, sizeY = outMatrix.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = (*this)(nSubscriptMatrixX(i,j), nSubscriptMatrixY(1,1));
	}
	else if (nSubscriptMatrixX.GetTotalSize() == 1 && nSubscriptMatrixY.GetTotalSize() > 0)
	{
		outMatrix.SetSize(nSubscriptMatrixY.GetSize());
		int sizeX = outMatrix.GetSize().cx, sizeY = outMatrix.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = (*this)(nSubscriptMatrixX(1,1), nSubscriptMatrixY(i,j));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, nSubscriptMatrixX.GetSize(), nSubscriptMatrixY.GetSize());
		outMatrix.Empty();
	}
	return outMatrix;
}

CMatrix& CMatrix::operator=(const double& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inValue);

	return *this;
}

CMatrix& CMatrix::operator=(const CMatrix& inMatrix)
{
	SetOrder(inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inMatrix.GetAt(k));

	return *this;
}

CMatrix& CMatrix::operator=(const CIntMatrix& inMatrix)
{
	SetOrder((CMatrix::KINDOFORDER)inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, double(inMatrix.GetAt(k)));

	return *this;
}

void CMatrix::operator+=(const double& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) + inValue);
}

void CMatrix::operator-=(const double& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) - inValue);
}

void CMatrix::operator*=(const double& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) * inValue);
}

void CMatrix::operator/=(const double& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) / inValue);
}

void CMatrix::operator+=(const CMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) += inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) + tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp + inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::operator-=(const CMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) -= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) - tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp - inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::DotTimesAssign(const CMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) *= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) * tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp * inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::DotOverAssign(const CMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) /= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) / tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp / inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::operator*=(const CMatrix& inMatrix)
{
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		CMatrix tmpMatrix = *this;
		double tmp;

		SetSize(CSize(tmpMatrix.GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = tmpMatrix.GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = tmpMatrix.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
		{
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0.;
				for (int k = 1; k <= size; k++)
					tmp += tmpMatrix(i,k)*inMatrix(k,j);
				(*this)(i,j) = tmp;
			}
		}
	}
	else if ((GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1) || (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0))
		(*this).DotTimesAssign(inMatrix);
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_MULT, GetSize(), inMatrix.GetSize());
}

void CMatrix::operator+=(const CIntMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) += inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) + tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp + inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::operator-=(const CIntMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) -= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) - tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp - inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::DotTimesAssign(const CIntMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) *= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) * tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp * inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

void CMatrix::DotOverAssign(const CIntMatrix& inMatrix)
{
	if (GetSize() == inMatrix.GetSize())
	{
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				(*this)(i,j) /= inMatrix(i,j);
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) / tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		SetSize(inMatrix.GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, tmp / inMatrix.GetAt(k));
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());
		Empty();
	}
}

CMatrix CMatrix::operator+(const int& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix += double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator-(const int& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix -= double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator*(const int& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix *= double(inValue);

	return outMatrix;
}

CMatrix CMatrix::operator/(const int& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix /= double(inValue);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) < inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) > inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) <= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) >= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator==(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) == inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator!=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) != inValue));

	return outMatrix;
}

CMatrix operator+(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix += double(inValue);

	return outMatrix;
}

CMatrix operator-(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix -= double(inValue);

	return -outMatrix;
}

CMatrix operator*(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix *= double(inValue);

	return outMatrix;
}

CMatrix operator/(const int& inValue, const CMatrix& inMatrix)			// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix.GetSize(), double(inValue));

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue < inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue > inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator<=(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue <= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>=(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue >= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator==(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue == inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator!=(const int& inValue, const CMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue != inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix CMatrix::operator+(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix += inValue;

	return outMatrix;
}

CMatrix CMatrix::operator-(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix -= inValue;

	return outMatrix;
}

CMatrix CMatrix::operator*(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix CMatrix::operator/(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) < inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) > inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) <= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) >= inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator==(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) == inValue));

	return outMatrix;
}

CIntMatrix CMatrix::operator!=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) != inValue));

	return outMatrix;
}

CMatrix operator+(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix += inValue;

	return outMatrix;
}

CMatrix operator-(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix -= inValue;

	return -outMatrix;
}

CMatrix operator*(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix operator/(const double& inValue, const CMatrix& inMatrix)		// friend function of CMatrix
{
	CMatrix outMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue < inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue > inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator<=(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue <= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>=(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue >= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator==(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue == inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator!=(const double& inValue, const CMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue != inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix CMatrix::operator+(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CMatrix::operator-(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix -= inMatrix;

	return outMatrix;
}

CMatrix CMatrix::DotTimes(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CMatrix::DotOver(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) < inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) < tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp < inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) > inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) > tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp > inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) <= inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) <= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp <= inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) >= inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) >= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp >= inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator==(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) == inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) == tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp == inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator!=(const CMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) != inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) != tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp != inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator+(const CIntMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CMatrix::operator-(const CIntMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix -= inMatrix;

	return outMatrix;
}

CMatrix CMatrix::DotTimes(const CIntMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CMatrix::DotOver(const CIntMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CMatrix::operator<(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) < inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) < tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp < inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) > inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) > tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp > inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator<=(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) <= inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) <= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp <= inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator>=(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) >= inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) >= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp >= inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator==(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) == inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) == tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp == inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CIntMatrix CMatrix::operator!=(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	if (GetSize() == inMatrix.GetSize())
	{
		outMatrix.SetSize(GetSize());
		const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = int((*this)(i,j) != inMatrix(i,j));
	}
	else if (GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1)
	{
		double tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder((CIntMatrix::KINDOFORDER)GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) != tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		double tmp = GetAt(0);
		outMatrix.SetOrder(inMatrix.GetOrder());
		outMatrix.SetSize(inMatrix.GetSize());
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(tmp != inMatrix.GetAt(k)));
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_EQUAL, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator-() const
{
	CMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, -GetAt(k));

	return outMatrix;
}

CMatrix CMatrix::operator*(const CMatrix& inMatrix) const
{
	CMatrix outMatrix;
	double tmp;
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		outMatrix.SetSize(CSize(GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
		{
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0.;
				for (int k = 1; k <= size; k++)
					tmp += (*this)(i,k)*inMatrix(k,j);
				outMatrix(i,j) = tmp;
			}
		}
	}
	else if ((GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1) || (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0))
		outMatrix = (*this).DotTimes(inMatrix);
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_MULT, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

CMatrix CMatrix::operator*(const CIntMatrix& inMatrix) const
{
	CMatrix outMatrix;
	double tmp;
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		outMatrix.SetSize(CSize(GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
		{
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0.;
				for (int k = 1; k <= size; k++)
					tmp += (*this)(i,k)*inMatrix(k,j);
				outMatrix(i,j) = tmp;
			}
		}
	}
	else if ((GetTotalSize() > 0 && inMatrix.GetTotalSize() == 1) || (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0))
		outMatrix = (*this).DotTimes(inMatrix);
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_MULT, GetSize(), inMatrix.GetSize());

	return outMatrix;
}

BOOL CMatrix::reshape(const int& nNewSizeX, const int& nNewSizeY)
{
	BOOL bRet;

	if (GetTotalSize() == nNewSizeX*nNewSizeY)
	{
		SetSize(CSize(nNewSizeX, nNewSizeY));
		bRet = TRUE;
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_RESHAPE, GetSize(), CSize(nNewSizeX, nNewSizeY));
		bRet = FALSE;
	}

	return bRet;
}

BOOL CMatrix::reshape(const CSize& nNewSize)
{
	BOOL bRet;

	if (GetTotalSize() == nNewSize.cx*nNewSize.cy)
	{
		SetSize(nNewSize);
		bRet = TRUE;
	}
	else
	{
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_RESHAPE, GetSize(), nNewSize);
		bRet = FALSE;
	}

	return bRet;
}

BOOL CMatrix::reshape()
{
	// make it a column vector
	SetSize(GetTotalSize());

	return TRUE;
}

CMatrix CMatrix::GetRows(const int& nSubscriptX) const
{
	CMatrix outMatrix;

	if (nSubscriptX >=1 && nSubscriptX <= GetSize().cx)
	{
		outMatrix.SetSize(CSize(1, GetSize().cy));
		int sizeY = GetSize().cy;
		for (int j = 1; j <= sizeY; j++)
			outMatrix(1,j) = (*this)(nSubscriptX,j);
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_ROW, GetSize(), CSize(nSubscriptX, 1));

	return outMatrix;
}

CMatrix CMatrix::GetColumns(const int& nSubscriptY) const
{
	CMatrix outMatrix;
	if (nSubscriptY >=1 && nSubscriptY <= GetSize().cy)
	{
		outMatrix.SetSize(CSize(GetSize().cx, 1));
		int sizeX = GetSize().cx;
		for (int i = 1; i <= sizeX; i++)
			outMatrix(i,1) = (*this)(i,nSubscriptY);
	}
	else
		DimensionErrorMessage(ERROR_MATRIX_DIM_INDEX_COL, GetSize(), CSize(1, nSubscriptY));

	return outMatrix;
}

CMatrix CMatrix::GetRows(const CIntMatrix& nSubscriptMatrixX) const
{
	CMatrix outMatrix;
	int size = nSubscriptMatrixX.GetTotalSize();
	if (size > 0)
		outMatrix = GetRows(nSubscriptMatrixX[0]);
	for (int k = 1; k < size; k++)
		outMatrix = concatVert(outMatrix, GetRows(nSubscriptMatrixX[k]));

	return outMatrix;
}

CMatrix CMatrix::GetColumns(const CIntMatrix& nSubscriptMatrixY) const
{
	CMatrix outMatrix;
	int size = nSubscriptMatrixY.GetTotalSize();
	if (size > 0)
		outMatrix = GetColumns(nSubscriptMatrixY[0]);
	for (int k = 1; k < size; k++)
		outMatrix = concatHor(outMatrix, GetColumns(nSubscriptMatrixY[k]));

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Mathematische CMatrix Funktionen
CMatrix acos(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::acos(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix asin(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::asin(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix atan(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::atan(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix atan2(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::atan2(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix atan2(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::atan2(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix atan2(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = ::atan2(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::atan2(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::atan2(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix ceil(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::ceil(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix cos(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::cos(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix cosh(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::cosh(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix exp(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::exp(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix fabs(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::fabs(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix fix(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, fix(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix floor(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::floor(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix fmod(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::fmod(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix fmod(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::fmod(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix fmod(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = ::fmod(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::fmod(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::fmod(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix frem(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, frem(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix frem(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, frem(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix frem(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = frem(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, frem(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, frem(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix log(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::log(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix log10(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::log10(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix pow(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::pow(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix pow(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::pow(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix pow(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = ::pow(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::pow(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, ::pow(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix round(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, round(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix sign(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, sign(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix sin(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::sin(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix sinh(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::sinh(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix sqrt(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::sqrt(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix tan(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::tan(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix tanh(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::tanh(inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix arcreset(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, arcreset(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix arcreset(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, arcreset(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix arcreset(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = arcreset(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, arcreset(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, arcreset(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

double matMaxFinite(const CMatrix& inMatrix)
{
	double tmpValue, outValue = -HUGE;
	if (inMatrix.GetTotalSize() > 0)
	{
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			if (::_finite(tmpValue = inMatrix.GetAt(k)))
				outValue = max(outValue, tmpValue);
	}
	else
		outValue = 0.;

	return outValue;
}

double matMinFinite(const CMatrix& inMatrix)
{
	double tmpValue, outValue = HUGE;
	if (inMatrix.GetTotalSize() > 0)
	{
		int size = inMatrix.GetTotalSize();
		for (int k = 0; k < size; k++)
			if (::_finite(tmpValue = inMatrix.GetAt(k)))
				outValue = min(outValue, tmpValue);
	}
	else
		outValue = 0.;

	return outValue;
}

double matMax(const CMatrix& inMatrix)
{
	double outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int k = 1; k < size; k++)
			outValue = max(outValue, inMatrix.GetAt(k));
	}
	else
		outValue = 0.;

	return outValue;
}

double matMin(const CMatrix& inMatrix)
{
	double outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int k = 1; k < size; k++)
			outValue = min(outValue, inMatrix.GetAt(k));
	}
	else
		outValue = 0.;

	return outValue;
}

CMatrix matMax(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, max(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix matMax(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, max(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix matMax(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = max(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, max(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, max(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix matMin(const CMatrix& inMatrix, const double& inValue)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, min(inMatrix.GetAt(k), inValue));

	return outMatrix;
}

CMatrix matMin(const double& inValue, const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, min(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix matMin(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	if (inMatrix1.GetSize() == inMatrix2.GetSize())
	{
		outMatrix.SetSize(inMatrix1.GetSize());
		int sizeX = inMatrix1.GetSize().cx, sizeY = inMatrix1.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
			for (int j = 1; j <= sizeY; j++)
				outMatrix(i,j) = min(inMatrix1(i,j), inMatrix2(i,j));
	}
	else if (inMatrix1.GetTotalSize() > 0 && inMatrix2.GetTotalSize() == 1)
	{
		double tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, min(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		double tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, min(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix _finite(const CMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::_finite(inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix _isnan(const CMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, ::_isnan(inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix _isinf(const CMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, _isinf(inMatrix.GetAt(k)));

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Andere CMatrix Funktionen
CMatrix transp(const CMatrix& inMatrix)
{
	CMatrix outMatrix(inMatrix);
	outMatrix.transp();

	return outMatrix;
}

CMatrix reverse(const CMatrix& inMatrix, BOOL bReverseRows, BOOL bReverseColumns)
{
	if (!bReverseRows && !bReverseColumns)
		return inMatrix;

	CMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			outMatrix(i,j) = inMatrix((bReverseRows ? sizeX - i + 1 : i),(bReverseColumns ? sizeY - j + 1 : j));

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY)
{
	CIntMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape(nNewSizeX, nNewSizeY))
		outMatrix.Empty();

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix, const CSize& nNewSize)
{
	CMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape(nNewSize))
		outMatrix.Empty();

	return outMatrix;
}

CMatrix reshape(const CMatrix& inMatrix)
{
	CMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape())
		outMatrix.Empty();

	return outMatrix;
}

CMatrix concatHor(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	int i, j, sizeX1, sizeY1, sizeX2, sizeY2;

	sizeX1 = inMatrix1.GetSize().cx;
	sizeY1 = inMatrix1.GetSize().cy;
	sizeX2 = inMatrix2.GetSize().cx;
	sizeY2 = inMatrix2.GetSize().cy;
	if (sizeX1 == sizeX2)
	{
		outMatrix.SetSize(CSize(sizeX1, sizeY1 + sizeY2));
		for (i = 1; i <= sizeX1; i++)
			for (j = 1; j <= sizeY1; j++)
				outMatrix(i,j) = inMatrix1(i,j);
		for (i = 1; i <= sizeX2; i++)
			for (j = 1; j <= sizeY2; j++)
				outMatrix(i,sizeY1+j) = inMatrix2(i,j);
	}
	else if (inMatrix1.IsEmpty())
		return inMatrix2;
	else if (inMatrix2.IsEmpty())
		return inMatrix1;
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_CONCAT_HOR, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix concatVert(const CMatrix& inMatrix1, const CMatrix& inMatrix2)
{
	CMatrix outMatrix;
	int i, j, sizeX1, sizeY1, sizeX2, sizeY2;

	sizeX1 = inMatrix1.GetSize().cx;
	sizeY1 = inMatrix1.GetSize().cy;
	sizeX2 = inMatrix2.GetSize().cx;
	sizeY2 = inMatrix2.GetSize().cy;
	if (sizeY1 == sizeY2)
	{
		outMatrix.SetSize(CSize(sizeX1 + sizeX2, sizeY1));
		for (i = 1; i <= sizeX1; i++)
			for (j = 1; j <= sizeY1; j++)
				outMatrix(i,j) = inMatrix1(i,j);
		for (i = 1; i <= sizeX2; i++)
			for (j = 1; j <= sizeY2; j++)
				outMatrix(sizeX1+i,j) = inMatrix2(i,j);
	}
	else if (inMatrix1.IsEmpty())
		return inMatrix2;
	else if (inMatrix2.IsEmpty())
		return inMatrix1;
	else
		inMatrix1.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_CONCAT_VERT, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CMatrix rowSum(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	double tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(sizeX, 1));
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 0.;
		for (int j = 1; j <= sizeY; j++)
			tmp += inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CMatrix colSum(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	double tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(1, sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 0.;
		for (int i = 1; i <= sizeX; i++)
			tmp += inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

CMatrix rowProd(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	double tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(sizeX, 1));
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 1.;
		for (int j = 1; j <= sizeY; j++)
			tmp *= inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CMatrix colProd(const CMatrix& inMatrix)
{
	CMatrix outMatrix;
	double tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(1, sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 1.;
		for (int i = 1; i <= sizeX; i++)
			tmp *= inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

double sum(const CMatrix& inMatrix)
{
	double tmp = 0.;
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp += inMatrix[k];

	return tmp;
}

double prod(const CMatrix& inMatrix)
{
	double tmp = 1.;
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp *= inMatrix[k];

	return tmp;
}

double mean(const CMatrix& inMatrix)
{
	double tmp = 0.;
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp += inMatrix[k];
	if (size > 0)
		tmp /= (double)size;

	return tmp;
}

double meanFinite(const CMatrix& inMatrix)
{
	double tmp = 0.;
	int size = inMatrix.GetTotalSize(), counter = 0;
	for (int k = 0; k < size; k++)
		if (::_finite(inMatrix[k]))
		{
			counter++;
			tmp += inMatrix[k];
		}
	if (counter > 0)
		tmp /= (double)counter;

	return tmp;
}

double rms(const CMatrix& inMatrix)
{
	double tmp = 0., average = mean(inMatrix);
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp += (inMatrix[k]-average)*(inMatrix[k]-average);
	if (size > 1)
		tmp /= (double)size - 1.;
	tmp = ::sqrt(tmp);

	return tmp;
}

double rmsFinite(const CMatrix& inMatrix)
{
	double tmp = 0., average = meanFinite(inMatrix);
	int size = inMatrix.GetTotalSize(), counter = 0;
	for (int k = 0; k < size; k++)
		if (::_finite(inMatrix[k]))
		{
			counter++;
			tmp += (inMatrix[k]-average)*(inMatrix[k]-average);
		}
	if (counter > 1)
		tmp /= (double)counter - 1.;
	tmp = ::sqrt(tmp);

	return tmp;
}

void cart2pol(const CMatrix& X, const CMatrix& Y, CMatrix& Alpha, CMatrix& Radius)
{
	if (X.GetSize() == Y.GetSize())
	{
		Alpha = atan2(Y,X);
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y));
	}
	else if (X.GetTotalSize() > 0 && Y.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Radius = sqrt(X.DotTimes(X) + Y[0]*Y[0]);
	}
	else if (X.GetTotalSize() == 1 && Y.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,X);
		Radius = sqrt(X[0]*X[0] + Y.DotTimes(Y));
	}
	else
		X.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, X.GetSize(), Y.GetSize());
}

void cart2sph(const CMatrix& X, const CMatrix& Y, const CMatrix& Z, CMatrix& Alpha, CMatrix& Theta, CMatrix& Radius)
{
	if (X.GetSize() == Y.GetSize() && X.GetSize() == Z.GetSize())
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (X.GetSize() == Z.GetSize() && X.GetTotalSize() > 0 && Y.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y[0]*Y[0]));
		Radius = sqrt(X.DotTimes(X) + Y[0]*Y[0] + Z.DotTimes(Z));
	}
	else if (Y.GetSize() == X.GetSize() && Y.GetTotalSize() > 0 && Z.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y.DotTimes(Y)));
		Radius = sqrt(X.DotTimes(X) + Y.DotTimes(Y) + Z[0]*Z[0]);
	}
	else if (Z.GetSize() == Y.GetSize() && Z.GetTotalSize() > 0 && X.GetTotalSize() == 1)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X[0]*X[0] + Y.DotTimes(Y)));
		Radius = sqrt(X[0]*X[0] + Y.DotTimes(Y) + Z.DotTimes(Z));
	}
	else if (X.GetSize() == Z.GetSize() && X.GetTotalSize() == 1 && Y.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X[0]*X[0] + Y.DotTimes(Y)));
		Radius = sqrt(X[0]*X[0] + Y.DotTimes(Y) + Z[0]*Z[0]);
	}
	else if (Y.GetSize() == X.GetSize() && Y.GetTotalSize() == 1 && Z.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,CMatrix(Z.GetSize(), X[0]));
		Theta = atan2(Z, sqrt(X[0]*X[0] + Y[0]*Y[0]));
		Radius = sqrt(X[0]*X[0] + Y[0]*Y[0] + Z.DotTimes(Z));
	}
	else if (Z.GetSize() == Y.GetSize() && Z.GetTotalSize() == 1 && X.GetTotalSize() > 0)
	{
		Alpha = atan2(Y,X);
		Theta = atan2(Z, sqrt(X.DotTimes(X) + Y[0]*Y[0]));
		Radius = sqrt(X.DotTimes(X) + Y[0]*Y[0] + Z[0]*Z[0]);
	}
	else
		X.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, X.GetSize(), Y.GetSize(), Z.GetSize());
}

void pol2cart(const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y)
{
	if (Theta.GetSize() == Radius.GetSize())
	{
		X = Radius.DotTimes(cos(Theta));
		Y = Radius.DotTimes(sin(Theta));
	}
	else if (Theta.GetTotalSize() > 0 && Radius.GetTotalSize() == 1)
	{
		X = cos(Theta)*Radius[0];
		Y = sin(Theta)*Radius[0];
	}
	else if (Theta.GetTotalSize() == 1 && Radius.GetTotalSize() > 0)
	{
		X = Radius*cos(Theta[0]);
		Y = Radius*sin(Theta[0]);
	}
	else
		X.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, Theta.GetSize(), Radius.GetSize());
}

void sph2cart(const CMatrix& Alpha, const CMatrix& Theta, const CMatrix& Radius, CMatrix& X, CMatrix& Y, CMatrix& Z)
{
	if (Alpha.GetSize() == Theta.GetSize() && Alpha.GetSize() == Radius.GetSize())
	{
		X = Radius.DotTimes(cos(Theta)).DotTimes(cos(Alpha));
		Y = Radius.DotTimes(cos(Theta)).DotTimes(sin(Alpha));
		Z = Radius.DotTimes(sin(Theta));
	}
	else if (Alpha.GetSize() == Radius.GetSize() && Alpha.GetTotalSize() > 0 && Theta.GetTotalSize() == 1)
	{
		X = Radius.DotTimes(cos(Alpha))*cos(Theta[0]);
		Y = Radius.DotTimes(sin(Alpha))*cos(Theta[0]);
		Z = Radius*sin(Theta[0]);
	}
	else if (Theta.GetSize() == Alpha.GetSize() && Theta.GetTotalSize() > 0 && Radius.GetTotalSize() == 1)
	{
		X = cos(Theta).DotTimes(cos(Alpha))*Radius[0];
		Y = cos(Theta).DotTimes(sin(Alpha))*Radius[0];
		Z = sin(Theta)*Radius[0];
	}
	else if (Radius.GetSize() == Theta.GetSize() && Radius.GetTotalSize() > 0 && Alpha.GetTotalSize() == 1)
	{
		X = Radius.DotTimes(cos(Theta))*cos(Alpha[0]);
		Y = Radius.DotTimes(cos(Theta))*sin(Alpha[0]);
		Z = Radius.DotTimes(sin(Theta));
	}
	else if (Alpha.GetSize() == Radius.GetSize() && Alpha.GetTotalSize() == 1 && Theta.GetTotalSize() > 0)
	{
		X = cos(Theta)*Radius[0]*cos(Alpha[0]);
		Y = cos(Theta)*Radius[0]*sin(Alpha[0]);
		Z = sin(Theta)*Radius[0];
	}
	else if (Theta.GetSize() == Alpha.GetSize() && Theta.GetTotalSize() == 1 && Radius.GetTotalSize() > 0)
	{
		X = Radius*cos(Theta[0])*cos(Alpha[0]);
		Y = Radius*cos(Theta[0])*sin(Alpha[0]);
		Z = Radius*sin(Theta[0]);
	}
	else if (Radius.GetSize() == Theta.GetSize() && Radius.GetTotalSize() == 1 && Alpha.GetTotalSize() > 0)
	{
		X = cos(Alpha)*Radius[0]*cos(Theta[0]);
		Y = sin(Alpha)*Radius[0]*cos(Theta[0]);
		Z = CMatrix(Alpha.GetSize(),Radius[0]*::sin(Theta[0]));
	}
	else
		X.DimensionErrorMessage(CMatrix::ERROR_MATRIX_DIM_EQUAL, Alpha.GetSize(), Theta.GetSize(), Radius.GetSize());
}

void huge2value(CMatrix& inMatrix, double value)
{
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		if (inMatrix[k] == HUGE)
			inMatrix[k] = value;
}

void reduceSize(CMatrix& inMatrix, const int& maxTotal)
{
	int maxRows, maxColumns;

	if (inMatrix.IsEmpty())
		return;

	maxRows = (int)::ceil(::sqrt(maxTotal * double(inMatrix.GetSize().cx)/double(inMatrix.GetSize().cy)));
	maxColumns = (int)::ceil(::sqrt(maxTotal * double(inMatrix.GetSize().cy)/double(inMatrix.GetSize().cx)));
	reduceSize(inMatrix, maxRows, maxColumns);
}

void reduceSize(CMatrix& inMatrix, const int& maxRows, const int& maxColumns)
{
	ASSERT(maxRows > 0);
	ASSERT(maxColumns > 0);

	int rows, columns, rowFac = 1, columnFac = 1;

	rows = inMatrix.GetSize().cx;
	columns = inMatrix.GetSize().cy;

	while (rows > maxRows)
	{
		rows = ((rows+1)&~1)/2;			// rounding up
		rowFac *= 2;
	}
	while (columns > maxColumns)
	{
		columns = ((columns+1)&~1)/2;	// rounding up
		columnFac *= 2;
	}
	if (rowFac > 1 || columnFac > 1)
	{
		CMatrix tmpMatrix(inMatrix);
		int indexX, indexY;
		double value, entry;
		inMatrix.SetSize(CSize(rows, columns));
		int sizeX = tmpMatrix.GetSize().cx, sizeY = tmpMatrix.GetSize().cy;

		for (int i = 1; i <= rows; i++)
		{
			for (int j = 1; j <= columns; j++)
			{
				value = 0.;
				for (int k = 1; k <= rowFac; k++)
				{
					indexX = min(sizeX, (i-1)*rowFac + k);
					for (int l = 1; l <= columnFac; l++)
					{
						indexY = min(sizeY, (j-1)*columnFac + l);
						entry = tmpMatrix(indexX,indexY);
						if (::_finite(value))
						{
							if (::_finite(entry))
								value += entry;
							else
								value = entry;
						}
					}
				}
				if (::_finite(value))
					value /= double(rowFac*columnFac);
				inMatrix(i,j) = value;
			}
		}
	}
}

BOOL LinSolveGaussSimple(const CMatrix& A, const CMatrix& b, CMatrix& x)
{
	int i, j, k;
	double diagEl, factor;

	CMatrix AWork(A), bWork(b);

	int nRows = AWork.GetSize().cx;
	int nCols = AWork.GetSize().cy;
	int nRowsRhs = bWork.GetSize().cx;
	int nColsRhs = bWork.GetSize().cy;
	ASSERT(nRows == nCols);
	ASSERT(nRows == nRowsRhs);

	// set solvable flag
	BOOL bIsSolvable = TRUE;
	for (i = 1; i < nRows; i++)
	{
		// test if solvable (diagonal element nonzero)
		if ((diagEl = AWork(i,i)) != 0.)
		{
			// modify equations/rhs below i-th equation
			for (k = i+1; k <= nCols; k++)
			{
				// factor for k-th equation
				factor = AWork(k,i)/diagEl;

				if (factor != 0.)
				{
					// modify k-th equation
					for (j = i; j <= nRows; j++)
					{
						AWork(k,j) -= factor*AWork(i,j);
					}
					// modify k-th row of right hand side
					for (j = 1; j <= nColsRhs; j++)
					{
						bWork(k,j) -= factor*bWork(i,j);
					}
				}
			}
		}
		else
		{
			bIsSolvable = FALSE;
			break;
		}
	}

	if (!bIsSolvable)
	{
		x.Empty();
	}
	else
	{
		// set size of x
		x.SetSize(bWork.GetSize());

		// Recalculation for all right hand sides
		for (j = 1; j <= nColsRhs; j++)
		{
			for (i = nRows; i >= 1; i--)
			{
				// x{i} = (bWork{i} - sum_k=i+1..N (AWork{i,k}*bWork{k})) / AWork{i,i}
				x(i,j) = bWork(i,j);
				for (k = nCols; k >= i+1; k--)
				{
					x(i,j) -= AWork(i,k)*x(k,j);
				}
				x(i,j) /= AWork(i,i);
			}
		}
	}

	return bIsSolvable;
}
}
