/////////////////////////////////////////////////////////////////////////////
// Copyright: 
// Uwe Kotyczka <uwe.kotyczka@web.de>
// created: July 1998
//
/////////////////////////////////////////////////////////////////////////////
// IntMatrix.cpp: Implementierungsdatei
//

#include "StdAfx.h"
#include "Matrix.h"
#include "IntMatrix.h"
#include "MathAdd.h"
#include "Resource.h"

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
// CIntMatrix

CIntMatrix::CIntMatrix(const KINDOFORDER& nKindOfOrder)
{
	// this is an empty matrix
	SetOrder(nKindOfOrder);
	Empty();
}

CIntMatrix::CIntMatrix(const int& inValue, const KINDOFORDER& nKindOfOrder)
{
	// copy inValue
	SetOrder(nKindOfOrder);
	SetSize(1);
	SetAt(0, inValue);
}

CIntMatrix::CIntMatrix(const int& firstValue, const int& lastValue, const double& stepSize, const KINDOFORDER& nKindOfOrder)
{
	// setup row matrix
	SetOrder(nKindOfOrder);
	if (stepSize == 0.)
		Empty();
	else if ((lastValue-firstValue)/stepSize < 0.)
		Empty();
	else
	{
		SetSize(CSize(1, int(ceil((lastValue-firstValue+stepSize)/stepSize))));
		int size = GetTotalSize();
		for (int k = 0; k < size-1; k++)
			SetAt(k, int(firstValue + k*stepSize));
		SetAt(size-1, lastValue);
	}
}

CIntMatrix::CIntMatrix(const CSize& inSize, const int& initValue, const KINDOFORDER& nKindOfOrder)
{
	// initialize with initValue
	SetOrder(nKindOfOrder);
	SetSize(inSize);
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, initValue);
}

CIntMatrix::CIntMatrix(const CIntMatrix& inMatrix)
{
	// copy inMatrix
	SetOrder(inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inMatrix.GetAt(k));
}

CIntMatrix::CIntMatrix(const CMatrix& inMatrix)
{
	// copy inMatrix
	SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, int(inMatrix.GetAt(k)));
}

CIntMatrix::CIntMatrix(const CIntMatrix& inMatrix, const KINDOFORDER& nKindOfOrder)
{
	// copy inMatrix
	SetOrder(nKindOfOrder);
	CopyContents(inMatrix);
}

CIntMatrix::CIntMatrix(const CMatrix& inMatrix, const KINDOFORDER& nKindOfOrder)
{
	// copy inMatrix
	SetOrder(nKindOfOrder);
	CopyContents(inMatrix);
}

CIntMatrix::~CIntMatrix()
{
}

//{{AFX_MSG_MAP(CIntMatrix)
	// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// Diagnose CIntMatrix

#ifdef _DEBUG
void CIntMatrix::AssertValid() const
{
	CArray<int, int>::AssertValid();

	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);
}

void CIntMatrix::Dump(CDumpContext& dc) const
{
	CArray<int, int>::Dump(dc);

	dc << "\nm_nSizeX = " << m_nSizeX;
	dc << "\nm_nSizeY = " << m_nSizeY;
	dc << "\nm_nKindOfOrder = " << m_nKindOfOrder;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Überladungen CIntMatrix

void CIntMatrix::CopyContents(const CIntMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());

	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			(*this)(i,j) = inMatrix(i,j);
}

void CIntMatrix::CopyContents(const CMatrix& inMatrix)
{
	SetSize(inMatrix.GetSize());

	const int &sizeX = m_nSizeX, &sizeY = m_nSizeY;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			(*this)(i,j) = (int)inMatrix(i,j);
}

void CIntMatrix::SetSize(int nNewSize, int nGrowBy)
{
	// format it as a column vector
	int sizeX = nNewSize;
	int sizeY = 1;
	if (sizeX <= 0)
		sizeX = sizeY = 0;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	CArray<int, int>::SetSize(sizeX*sizeY, nGrowBy);
}

void CIntMatrix::SetSize(CSize nNewSize, int nGrowBy)
{
	int sizeX = nNewSize.cx;
	int sizeY = nNewSize.cy;
	if (sizeX <= 0 || sizeY <= 0)
		sizeX = sizeY = 0;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	CArray<int, int>::SetSize(sizeX*sizeY, nGrowBy);
}

inline CSize CIntMatrix::GetSize() const
{
	return CSize(m_nSizeX, m_nSizeY);
}

inline int CIntMatrix::GetTotalSize() const
{
	return m_nSizeX * m_nSizeY;
}

inline int CIntMatrix::GetLength() const
{
	return max(m_nSizeX, m_nSizeY);
}

void CIntMatrix::SetOrder(KINDOFORDER nKindOfOrder)
{
	ASSERT(nKindOfOrder == UNKNOWN_ORDER || nKindOfOrder == FORTRAN_ORDER || nKindOfOrder == C_ORDER);
	if (nKindOfOrder == UNKNOWN_ORDER)
		nKindOfOrder = FORTRAN_ORDER;

	m_nKindOfOrder = nKindOfOrder;
}

void CIntMatrix::SetOrder(KINDOFORDER nKindOfOrder, BOOL bReArrange)
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	if (m_nKindOfOrder == nKindOfOrder)
		return;

	if (bReArrange)
	{
		CIntMatrix tmpMatrix(*this, nKindOfOrder);
		*this = tmpMatrix;
	}
	else
		SetOrder(nKindOfOrder);
}

void CIntMatrix::transp()
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
// CIntMatrix Serialisierung

void CIntMatrix::Serialize(CArchive& ar, unsigned char numByte)
{
	ASSERT(numByte == 1 || numByte == 2 || numByte == 4);
	int i, j, sizeX, sizeY;
	int tmpInt, minVal = 0, maxVal = 0, fac = 1;
	UINT tmpUINT;
	char tmpChar;
	short tmpShort;

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
			int maxDiff = (numByte == 1) ? 256 : 65536;

			minVal = matMin(*this);
			maxVal = matMax(*this);
			tmpUINT = maxVal - minVal;
			while (tmpUINT/fac >= (UINT)maxDiff)
				fac *= 2;
			ar << minVal;
			ar << fac;
		}
		for (j = 1; j <= sizeY; j++)
		{
			for (i = 1; i <= sizeX; i++)
			{
				tmpInt = (*this)(i,j);
				switch (numByte)
				{
				case 1:
					tmpUINT = tmpInt - minVal;
					tmpUINT /= fac;
					tmpChar = (char)(int(tmpUINT) - 128);
					ar << tmpChar;
					break;
				case 2:
					tmpUINT = tmpInt - minVal;
					tmpUINT /= fac;
					tmpShort = (short)(int(tmpUINT) - 32768);
					ar << tmpShort;
					break;
				case 4:
					ar << tmpInt;
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

		ar >> tmpChar;
		numByte = unsigned char(tmpChar & 0x0F);
		nKindOfOrder = (KINDOFORDER)(tmpChar >> 4);
		if (nKindOfOrder == UNKNOWN_ORDER)
		{
			nKindOfOrder = FORTRAN_ORDER;
		}
		SetOrder(nKindOfOrder);

		ar >> sizeX;
		ar >> sizeY;
		if (numByte < 4)
		{
			ar >> minVal;
			ar >> fac;
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
					tmpUINT = tmpChar + 128;
					tmpUINT *= fac;
					(*this)(i,j) = int(tmpUINT + minVal);
					break;
				case 2:
					ar >> tmpShort;
					tmpUINT = tmpShort + 32768;
					tmpUINT *= fac;
					(*this)(i,j) = int(tmpUINT + minVal);
					break;
				case 4:
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
// Befehle CIntMatrix 
inline void CIntMatrix::Empty()
{
	SetSize(0);
}

BOOL CIntMatrix::IsEmpty() const
{
	return (GetTotalSize() == 0);
}

BOOL CIntMatrix::IsVector() const
{
	return (GetSize().cx == 1 || GetSize().cy == 1);
}

void CIntMatrix::DimensionErrorMessage(KINDOFDIMERROR k, CSize sizeOne, CSize sizeTwo) const
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

CArchive& AFXAPI operator<<(CArchive& ar, CIntMatrix& matrix)	// friend function of CIntMatrix
{
	matrix.Serialize(ar);
	return ar;
}

CArchive& AFXAPI operator>>(CArchive& ar, CIntMatrix& matrix)	// friend function of CIntMatrix
{
	matrix.Serialize(ar);
	return ar;
}

int CIntMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY) const
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
		return 0;
	}
}

int& CIntMatrix::operator()(const int& nSubscriptX, const int& nSubscriptY)
{
	ASSERT(m_nKindOfOrder == FORTRAN_ORDER || m_nKindOfOrder == C_ORDER);

	static int ret;
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
		return ret = 0;
	}
}

CIntMatrix CIntMatrix::operator()(const CIntMatrix& nSubscriptMatrixX, const CIntMatrix& nSubscriptMatrixY) const
{
	CIntMatrix outMatrix;
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

CIntMatrix& CIntMatrix::operator=(const int& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inValue);

	return *this;
}

CIntMatrix& CIntMatrix::operator=(const CIntMatrix& inMatrix)
{
	SetOrder(inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, inMatrix.GetAt(k));

	return *this;
}

CIntMatrix& CIntMatrix::operator=(const CMatrix& inMatrix)
{
	SetOrder((CIntMatrix::KINDOFORDER)inMatrix.GetOrder());
	SetSize(inMatrix.GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, int(inMatrix.GetAt(k)));

	return *this;
}

void CIntMatrix::operator+=(const int& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) + inValue);
}

void CIntMatrix::operator-=(const int& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) - inValue);
}

void CIntMatrix::operator*=(const int& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) * inValue);
}

void CIntMatrix::operator/=(const int& inValue)
{
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		SetAt(k, GetAt(k) / inValue);
}

void CIntMatrix::operator+=(const CIntMatrix& inMatrix)
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
		int tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) + tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

void CIntMatrix::operator-=(const CIntMatrix& inMatrix)
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
		int tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) - tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

void CIntMatrix::DotTimesAssign(const CIntMatrix& inMatrix)
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
		int tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) * tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

void CIntMatrix::DotOverAssign(const CIntMatrix& inMatrix)
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
		int tmp = inMatrix.GetAt(0);
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			SetAt(k, GetAt(k) / tmp);
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

void CIntMatrix::operator*=(const CIntMatrix& inMatrix)
{
	if (GetSize().cy == inMatrix.GetSize().cx)
	{
		CIntMatrix tmpMatrix = *this;
		int tmp;

		SetSize(CSize(tmpMatrix.GetSize().cx, inMatrix.GetSize().cy));
		int sizeX = tmpMatrix.GetSize().cx;
		int sizeY = inMatrix.GetSize().cy;
		int size = tmpMatrix.GetSize().cy;
		for (int i = 1; i <= sizeX; i++)
		{
			for (int j = 1; j <= sizeY; j++)
			{
				tmp = 0;
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

CIntMatrix CIntMatrix::operator+(const int& inValue) const
{
	CIntMatrix outMatrix(*this);
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const int& inValue) const
{
	CIntMatrix outMatrix(*this);
	outMatrix -= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const int& inValue) const
{
	CIntMatrix outMatrix(*this);
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator/(const int& inValue) const
{
	CIntMatrix outMatrix(*this);
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) >= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator==(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) == inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const int& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) != inValue));

	return outMatrix;
}

CIntMatrix operator+(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix(inMatrix);
	outMatrix += inValue;

	return outMatrix;
}

CIntMatrix operator-(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix(inMatrix);
	outMatrix -= inValue;

	return -outMatrix;
}

CIntMatrix operator*(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix(inMatrix);
	outMatrix *= inValue;

	return outMatrix;
}

CIntMatrix operator/(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue < inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue > inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator<=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue <= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue >= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator==(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue == inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator!=(const int& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue != inMatrix.GetAt(k)));

	return outMatrix;
}

CMatrix CIntMatrix::operator+(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix += inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix -= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator*(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix CIntMatrix::operator/(const double& inValue) const
{
	CMatrix outMatrix(*this);
	outMatrix /= inValue;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) < inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) > inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) <= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator>=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) >= inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator==(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) == inValue));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!=(const double& inValue) const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(GetAt(k) != inValue));

	return outMatrix;
}

CMatrix operator+(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix += inValue;

	return outMatrix;
}

CMatrix operator-(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix -= inValue;

	return -outMatrix;
}

CMatrix operator*(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix(inMatrix);
	outMatrix *= inValue;

	return outMatrix;
}

CMatrix operator/(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CMatrix outMatrix(inMatrix.GetSize(), inValue);

	return outMatrix.DotOver(inMatrix);
}

CIntMatrix operator<(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue < inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>(const double& inValue, const CIntMatrix& inMatrix)		// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue > inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator<=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue <= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator>=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue >= inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator==(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue == inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix operator!=(const double& inValue, const CIntMatrix& inMatrix)	// friend function of CIntMatrix
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, int(inValue != inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator+(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix(*this);
	outMatrix += inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::operator-(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix(*this);
	outMatrix -= inMatrix;

	return outMatrix;
}

CIntMatrix CIntMatrix::DotTimes(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix(*this);
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::DotOver(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix(*this);
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) < tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CIntMatrix CIntMatrix::operator>(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) > tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CIntMatrix CIntMatrix::operator<=(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) <= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CIntMatrix CIntMatrix::operator>=(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) >= tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CIntMatrix CIntMatrix::operator==(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) == tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CIntMatrix CIntMatrix::operator!=(const CIntMatrix& inMatrix) const
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
		int tmp = inMatrix.GetAt(0);
		outMatrix.SetOrder(GetOrder());
		outMatrix.SetSize(GetSize());
		int size = GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, int(GetAt(k) != tmp));
	}
	else if (GetTotalSize() == 1 && inMatrix.GetTotalSize() > 0)
	{
		int tmp = GetAt(0);
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

CMatrix CIntMatrix::operator+(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix += inMatrix;

	return outMatrix;
}

CMatrix CIntMatrix::operator-(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix -= inMatrix;

	return -outMatrix;
}

CMatrix CIntMatrix::DotTimes(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotTimesAssign(inMatrix);

	return outMatrix;
}

CMatrix CIntMatrix::DotOver(const CMatrix& inMatrix) const
{
	CMatrix outMatrix(*this);
	outMatrix.DotOverAssign(inMatrix);

	return outMatrix;
}

CIntMatrix CIntMatrix::operator<(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator>(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator<=(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator>=(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator==(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator!=(const CMatrix& inMatrix) const
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
		outMatrix.SetOrder(GetOrder());
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

CIntMatrix CIntMatrix::operator-() const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, -GetAt(k));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator!() const
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(GetOrder());
	outMatrix.SetSize(GetSize());
	int size = GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, !GetAt(k));

	return outMatrix;
}

CIntMatrix CIntMatrix::operator*(const CIntMatrix& inMatrix) const
{
	CIntMatrix outMatrix;
	int tmp;
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
				tmp = 0;
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

CMatrix CIntMatrix::operator*(const CMatrix& inMatrix) const
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

BOOL CIntMatrix::reshape(const int& nNewSizeX, const int& nNewSizeY)
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

BOOL CIntMatrix::reshape(const CSize& nNewSize)
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

BOOL CIntMatrix::reshape()
{
	// make it a column vector
	SetSize(GetTotalSize());

	return TRUE;
}

CIntMatrix CIntMatrix::GetRows(const int& nSubscriptX) const
{
	CIntMatrix outMatrix;

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

CIntMatrix CIntMatrix::GetColumns(const int& nSubscriptY) const
{
	CIntMatrix outMatrix;
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

CIntMatrix CIntMatrix::GetRows(const CIntMatrix& nSubscriptMatrixX) const
{
	CIntMatrix outMatrix;
	int size = nSubscriptMatrixX.GetTotalSize();
	if (size > 0)
		outMatrix = GetRows(nSubscriptMatrixX[0]);
	for (int k = 1; k < size; k++)
		outMatrix = concatVert(outMatrix, GetRows(nSubscriptMatrixX[k]));

	return outMatrix;
}

CIntMatrix CIntMatrix::GetColumns(const CIntMatrix& nSubscriptMatrixY) const
{
	CIntMatrix outMatrix;
	int size = nSubscriptMatrixY.GetTotalSize();
	if (size > 0)
		outMatrix = GetColumns(nSubscriptMatrixY[0]);
	for (int k = 1; k < size; k++)
		outMatrix = concatHor(outMatrix, GetColumns(nSubscriptMatrixY[k]));

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Mathematische CIntMatrix Funktionen
CIntMatrix abs(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, abs(inMatrix.GetAt(k)));

	return outMatrix;
}

int matMax(const CIntMatrix& inMatrix)
{
	int outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int k = 1; k < size; k++)
			outValue = max(outValue, inMatrix.GetAt(k));
	}
	else
		outValue = 0;

	return outValue;
}

int matMin(const CIntMatrix& inMatrix)
{
	int outValue;
	if (inMatrix.GetTotalSize() > 0)
	{
		outValue = inMatrix.GetAt(0);
		int size = inMatrix.GetTotalSize();
		for (int k = 1; k < size; k++)
			outValue = min(outValue, inMatrix.GetAt(k));
	}
	else
		outValue = 0;

	return outValue;
}

CIntMatrix matMax(const CIntMatrix& inMatrix, const int& inValue)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, max(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix matMax(const int& inValue, const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, max(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix matMax(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
		int tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, max(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		int tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, max(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CIntMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix matMin(const CIntMatrix& inMatrix, const int& inValue)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, min(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix matMin(const int& inValue, const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		outMatrix.SetAt(k, min(inValue, inMatrix.GetAt(k)));

	return outMatrix;
}

CIntMatrix matMin(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
		int tmp = inMatrix2.GetAt(0);
		outMatrix.SetOrder(inMatrix1.GetOrder());
		outMatrix.SetSize(inMatrix1.GetSize());
		int size = inMatrix1.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, min(inMatrix1.GetAt(k), tmp));
	}
	else if (inMatrix1.GetTotalSize() == 1 && inMatrix2.GetTotalSize() > 0)
	{
		int tmp = inMatrix1.GetAt(0);
		outMatrix.SetOrder(inMatrix2.GetOrder());
		outMatrix.SetSize(inMatrix2.GetSize());
		int size = inMatrix2.GetTotalSize();
		for (int k = 0; k < size; k++)
			outMatrix.SetAt(k, min(tmp, inMatrix2.GetAt(k)));
	}
	else
		inMatrix1.DimensionErrorMessage(CIntMatrix::ERROR_MATRIX_DIM_EQUAL, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

/////////////////////////////////////////////////////////////////////////////
// Andere CIntMatrix Funktionen
CIntMatrix transp(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix(inMatrix);
	outMatrix.transp();

	return outMatrix;
}

CIntMatrix reverse(const CIntMatrix& inMatrix, BOOL bReverseRows, BOOL bReverseColumns)
{
	if (!bReverseRows && !bReverseColumns)
		return inMatrix;


	CIntMatrix outMatrix;
	outMatrix.SetOrder(inMatrix.GetOrder());
	outMatrix.SetSize(inMatrix.GetSize());
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	for (int i = 1; i <= sizeX; i++)
		for (int j = 1; j <= sizeY; j++)
			outMatrix(i,j) = inMatrix((bReverseRows ? sizeX - i + 1 : i),(bReverseColumns ? sizeY - j + 1 : j));

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix, const int& nNewSizeX, const int& nNewSizeY)
{
	CIntMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape(nNewSizeX, nNewSizeY))
		outMatrix.Empty();

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix, const CSize& nNewSize)
{
	CIntMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape(nNewSize))
		outMatrix.Empty();

	return outMatrix;
}

CIntMatrix reshape(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix(inMatrix);
	if (!outMatrix.reshape())
		outMatrix.Empty();

	return outMatrix;
}

CIntMatrix concatHor(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
		inMatrix1.DimensionErrorMessage(CIntMatrix::ERROR_MATRIX_DIM_CONCAT_HOR, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix concatVert(const CIntMatrix& inMatrix1, const CIntMatrix& inMatrix2)
{
	CIntMatrix outMatrix;
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
		inMatrix1.DimensionErrorMessage(CIntMatrix::ERROR_MATRIX_DIM_CONCAT_VERT, inMatrix1.GetSize(), inMatrix2.GetSize());

	return outMatrix;
}

CIntMatrix rowSum(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	int tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(sizeX, 1));
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 0;
		for (int j = 1; j <= sizeY; j++)
			tmp += inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CIntMatrix colSum(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	int tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(1, sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 0;
		for (int i = 1; i <= sizeX; i++)
			tmp += inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

CIntMatrix rowProd(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	int tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(sizeX, 1));
	for (int i = 1; i <= sizeX; i++)
	{
		tmp = 1;
		for (int j = 1; j <= sizeY; j++)
			tmp *= inMatrix(i,j);
		outMatrix(i,1) = tmp;
	}

	return outMatrix;
}

CIntMatrix colProd(const CIntMatrix& inMatrix)
{
	CIntMatrix outMatrix;
	int tmp;
	int sizeX = inMatrix.GetSize().cx, sizeY = inMatrix.GetSize().cy;
	outMatrix.SetSize(CSize(1, sizeY));
	for (int j = 1; j <= sizeY; j++)
	{
		tmp = 1;
		for (int i = 1; i <= sizeX; i++)
			tmp *= inMatrix(i,j);
		outMatrix(1,j) = tmp;
	}

	return outMatrix;
}

int sum(const CIntMatrix& inMatrix)
{
	int tmp = 0;
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp += inMatrix[k];

	return tmp;
}

int prod(const CIntMatrix& inMatrix)
{
	int tmp = 1;
	int size = inMatrix.GetTotalSize();
	for (int k = 0; k < size; k++)
		tmp *= inMatrix[k];

	return tmp;
}

void reduceSize(CIntMatrix& inMatrix, const int& maxTotal)
{
	int maxRows, maxColumns;

	if (inMatrix.IsEmpty())
		return;

	maxRows = (int)ceil(sqrt(maxTotal * double(inMatrix.GetSize().cx)/double(inMatrix.GetSize().cy)));
	maxColumns = (int)ceil(sqrt(maxTotal * double(inMatrix.GetSize().cy)/double(inMatrix.GetSize().cx)));
	reduceSize(inMatrix, maxRows, maxColumns);
}

void reduceSize(CIntMatrix& inMatrix, const int& maxRows, const int& maxColumns)
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
		CIntMatrix tmpMatrix(inMatrix);
		int indexX, indexY;
		double value;
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
						value += double(tmpMatrix(indexX,indexY));
					}
				}
				value /= double(rowFac*columnFac);
				inMatrix(i,j) = (int)value;
			}
		}
	}
}
