#include "IpDrawing.h"

#pragma region IpPen

IpPen::IpPen(unsigned char R, unsigned char G, unsigned char B,int width)
{
	m_R = R;
	m_G = G;
	m_B = B;
	m_Width = width;
	m_Pen = CreatePen(PS_SOLID,m_Width,RGB(m_R,m_G,m_B));
}

IpPen::~IpPen()
{
	DeleteObject(m_Pen);
}

HPEN* IpPen::GetPen()
{
	return &m_Pen;
}

void IpPen::SetColor(unsigned char R, unsigned char G, unsigned char B)
{
	m_R = R;
	m_G = G;
	m_B = B;
	DeleteObject(m_Pen);
	m_Pen = CreatePen(PS_SOLID, m_Width, RGB(m_R, m_G, m_B));
}
void IpPen::SetWidth(int width)
{
	m_Width = width;
	DeleteObject(m_Pen);
	m_Pen = CreatePen(PS_SOLID, m_Width, RGB(m_R, m_G, m_B));
}

void IpPen::GetColor(unsigned char* R, unsigned char* G, unsigned char* B)
{
	if (R != NULL)
		*R = m_R;
	if (G != NULL)
		*G = m_R;
	if (B != NULL)
		*B = m_R;
}
#pragma endregion

#pragma region IpSolidBrush

IpSolidBrush::IpSolidBrush(unsigned char R, unsigned char G, unsigned char B)
{
	m_R = R;
	m_G = G;
	m_B = B;
	m_Brush = CreateSolidBrush(RGB(m_R, m_G, m_B));
	m_Pen = CreatePen(PS_SOLID, 1, RGB(m_R, m_G, m_B));
}

IpSolidBrush::~IpSolidBrush()
{
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
}

HBRUSH* IpSolidBrush::GetBrush()
{
	return &m_Brush;
}

HPEN* IpSolidBrush::GetPen()
{
	return &m_Pen;
}

void IpSolidBrush::SetColor(unsigned char R, unsigned char G, unsigned char B)
{
	m_R = R;
	m_G = G;
	m_B = B;
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
	m_Brush = CreateSolidBrush(RGB(m_R, m_G, m_B));
	m_Pen = CreatePen(PS_SOLID, 1, RGB(m_R, m_G, m_B));
}

void IpSolidBrush::GetColor(unsigned char* R, unsigned char* G, unsigned char* B)
{
	if (R != NULL)
		*R = m_R;
	if (G != NULL)
		*G = m_R;
	if (B != NULL)
		*B = m_R;
}
#pragma endregion

#pragma region IpDrawing

IpDrawing::IpDrawing(HWND hwnd)
{
	m_HWND = hwnd;
	m_HDC = NULL;
	m_IsBegining = false;
}

bool IpDrawing::Begin()
{
	if (m_HWND == NULL)
		return false;
	m_HDC = BeginPaint(m_HWND, &m_PS);
	if (m_HDC == NULL)
		return false;
	m_IsBegining = true;
	return true;
}

bool IpDrawing::End()
{
	if (m_HWND == NULL)
		return false;
	m_IsBegining = false;
	return EndPaint(m_HWND, &m_PS);
}

bool IpDrawing::FillRectangle(IpSolidBrush* Brush, int x, int y, int width, int height)
{
	if (!m_IsBegining || Brush == NULL)
		return false;

	SelectObject(m_HDC, *Brush->GetPen());
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Rectangle(m_HDC,x,y,x+width,y+height))
		return false;
	return true;
}

bool IpDrawing::FillEllipse(IpSolidBrush* Brush,int x,int y,int width,int height)
{
	if (!m_IsBegining || Brush == NULL)
		return false;
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Ellipse(m_HDC, x, y, x + width, y + height))
		return false;
	return true;
}

bool IpDrawing::FillPie(IpSolidBrush* Brush, int x, int y, int width, int height,int x1,int y1,int x2,int y2)
{
	if (!m_IsBegining || Brush == NULL)
		return false;
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Pie(m_HDC, x, y, x + width, y + height, x1,y1,x2,y2))
		return false;
	return true;
}

bool IpDrawing::DrawLine(IpPen* Pen, int x1, int y1,int x2,int y2)
{
	if (!m_IsBegining || Pen == NULL)
		return false;
	SelectObject(m_HDC, *Pen->GetPen());
	MoveToEx(m_HDC, x1, y1, NULL);
	LineTo(m_HDC, x2, y2);
	return true;
}

bool IpDrawing::DrawBezier(IpPen* Pen, sDrawPoint p1,sDrawPoint p2,sDrawPoint p3, sDrawPoint p4)
{
	if (!m_IsBegining || Pen == NULL)
		return false;
	SelectObject(m_HDC, *Pen->GetPen());
	sDrawPoint points[m_NUMBEZIERPOINTS];
	float dT = 1 / (float)(m_NUMBEZIERPOINTS-1);
	float t = 0;
	points[0].x = p1.x;
	points[0].y = p1.y;
	MoveToEx(m_HDC, points[0].x, points[0].y, NULL);
	for (int i = 1; i < m_NUMBEZIERPOINTS; ++i)
	{
		t += dT;
		points[i].x = ((1 - t)*(1 - t)*(1 - t)*p1.x) + (3 * t*(1 - t)*(1 - t)*p2.x) + (3 * t*t*(1 - t)*p3.x) + (t * t*t*p4.x);
		points[i].y = ((1 - t)*(1 - t)*(1 - t)*p1.y) + (3 * t*(1 - t)*(1 - t)*p2.y) + (3 * t*t*(1 - t)*p3.y) + (t * t*t*p4.y);
		LineTo(m_HDC, points[i].x, points[i].y);
	}
}



#pragma endregion