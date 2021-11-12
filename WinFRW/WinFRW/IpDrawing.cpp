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

IpSolidBrush::IpSolidBrush(COLORREF color)
{
	m_R = GetRValue(color);
	m_G = GetGValue(color);
	m_B = GetBValue(color);

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

#pragma region IpFont
IpFont::IpFont(const std::string& fontName, int height, DWORD flags)
{
	m_R = 0;
	m_G = 0;
	m_B = 0;
	m_Height = height;
	m_Flags = flags;
	m_FontName = fontName;

	m_HDC = CreateCompatibleDC(NULL);
	int nHeight = -MulDiv(m_Height, (int)(GetDeviceCaps(m_HDC, LOGPIXELSY)), 72);
	unsigned long nBold = (m_Flags & eFontFlags::BOLD) ? FW_BOLD : FW_NORMAL;
	bool nItalic = (m_Flags & eFontFlags::ITALIC);
	bool nUnderLine = (m_Flags & eFontFlags::UNDERLINE);
	bool nStrikeOut = (m_Flags & eFontFlags::STRIKEOUT);
	m_Font = CreateFont(nHeight, 0, 0, 0, nBold, nItalic, nUnderLine, nStrikeOut,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, m_FontName.data());
}

IpFont::~IpFont()
{
	Free();
}

void IpFont::Free()
{
	DeleteObject(m_Font);
}

void IpFont::SetSize(int size)
{
	m_Height = size;
	DeleteObject(m_Font);
	int nHeight = -MulDiv(m_Height, (int)(GetDeviceCaps(m_HDC, LOGPIXELSY)), 72);
	unsigned long nBold = (m_Flags & eFontFlags::BOLD) ? FW_BOLD : FW_NORMAL;
	bool nItalic = (m_Flags & eFontFlags::ITALIC);
	bool nUnderLine = (m_Flags & eFontFlags::UNDERLINE);
	bool nStrikeOut = (m_Flags & eFontFlags::STRIKEOUT);
	m_Font = CreateFont(nHeight, 0, 0, 0, nBold, nItalic, nUnderLine, nStrikeOut,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, m_FontName.data());
}

void IpFont::SetColor(int r, int g, int b)
{
	m_R = r;
	m_G = g;
	m_B = b;
}

COLORREF IpFont::GetColor()
{
	return RGB(m_R, m_G, m_B);
}

int IpFont::GetFontSize()
{
	return m_Height;
}

HFONT* IpFont::GetFont()
{
	return &m_Font;
}
#pragma endregion


#pragma region IpDrawing

IpDrawing::IpDrawing(HWND hwnd,int x,int y, int width, int height)
{
	m_HWND = hwnd;
	m_HDC = NULL;
	RECT rc;
	GetWindowRect(m_HWND, &rc);

	m_Width =  (width != 0) ? width : rc.right - rc.left;
	m_Height =  (height != 0) ? height : rc.bottom - rc.top;
	m_X = x;
	m_Y = y;
	m_Rect.left = m_X;
	m_Rect.right = m_X + m_Width;
	m_Rect.top = m_Y;
	m_Rect.bottom = m_Y + m_Height;

	m_HDC = GetDC(m_HWND);
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

IpDrawing::IpDrawing(IpBitmap* Bitmap)
{
	m_HDC = *Bitmap->GetHDC();
	m_X = 0;
	m_Y = 0;
	m_Width = Bitmap->GetBitmap()->bmWidth;
	m_Height = Bitmap->GetBitmap()->bmHeight;
}

IpDrawing::~IpDrawing()
{
	Free();
}

void IpDrawing::Free()
{
	ReleaseDC(m_HWND, m_HDC);
}

bool IpDrawing::PrintText(IpFont* Font, const char* text, int x, int y)
{
	//HGDIOBJ obj;
	int lg = strlen(text);
	SelectObject(m_HDC, *Font->GetFont());
	SIZE sz;
	GetTextExtentPoint32A(m_HDC,text, lg, &sz);
	RECT rc;
	rc.left = x;
	rc.right = x + sz.cx;
	rc.top = y;
	rc.bottom = y + sz.cy;

	SetBkMode(m_HDC, TRANSPARENT);
	SetTextColor(m_HDC, Font->GetColor());
	DrawText(m_HDC, text, lg, &rc, DT_LEFT);
	//DrawTextA(m_HDC, text, lg, &rc, DT_LEFT);
	return true;
}

bool IpDrawing::FillRectangle(IpSolidBrush* Brush, int x, int y, int width, int height)
{
	if (Brush == NULL)
		return false;

	SelectObject(m_HDC, *Brush->GetPen());
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Rectangle(m_HDC,x,y,x+width,y+height))
		return false;
	return true;
}

bool IpDrawing::FillEllipse(IpSolidBrush* Brush,int x,int y,int width,int height)
{
	if (Brush == NULL)
		return false;
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Ellipse(m_HDC, x, y, x + width, y + height))
		return false;
	return true;
}

bool IpDrawing::FillPie(IpSolidBrush* Brush, int x, int y, int width, int height,int x1,int y1,int x2,int y2)
{
	if (Brush == NULL)
		return false;
	SelectObject(m_HDC, *Brush->GetBrush());
	if (!Pie(m_HDC, x, y, x + width, y + height, x1,y1,x2,y2))
		return false;
	return true;
}

bool IpDrawing::DrawLine(IpPen* Pen, int x1, int y1,int x2,int y2)
{
	if (Pen == NULL)
		return false;
	SelectObject(m_HDC, *Pen->GetPen());
	MoveToEx(m_HDC, x1, y1, NULL);
	LineTo(m_HDC, x2, y2);
	return true;
}

bool IpDrawing::DrawBezier(IpPen* Pen, sDrawPoint p1,sDrawPoint p2,sDrawPoint p3, sDrawPoint p4)
{
	if (Pen == NULL)
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
	return true;
}

bool IpDrawing::DrawBitmap(IpBitmap* bitmap, int x, int y, int width,int height, ScaleModes::EScaleModes scaleMode)
{
	if (bitmap == NULL)
		return false;
	BITMAP* bmp = bitmap->GetBitmap();
	SelectObject(*bitmap->GetHDC(), *bitmap->GetHBitmap());
	//if (width == 0 || height == 0)
	if (width < 1 || height < 1)
		return false;
		//BitBlt(m_HDC, m_X, m_Y, bmp->bmWidth, bmp->bmHeight, *bitmap->GetHDC(), m_X, m_Y, SRCCOPY);
	else
	{
		SetStretchBltMode(m_HDC, scaleMode);
		StretchBlt(m_HDC, m_X+x, m_Y+y, width, height, *bitmap->GetHDC(), m_X, m_Y, bmp->bmWidth, bmp->bmHeight, SRCCOPY);
	}
	return true;
}

bool IpDrawing::DrawBitmapUnscaled(IpBitmap* bitmap, int xOffset, int yOffset)
{
	if (bitmap == NULL)
		return false;
	BITMAP* bmp = bitmap->GetBitmap();
	SelectObject(*bitmap->GetHDC(), *bitmap->GetHBitmap());
	BitBlt(m_HDC, m_X, m_Y, bmp->bmWidth, bmp->bmHeight, *bitmap->GetHDC(), xOffset, yOffset, SRCCOPY);
	return true;
}

HDC* IpDrawing::GetHDC()
{
	return &m_HDC;
}

HWND* IpDrawing::GetHWND()
{
	return &m_HWND;
}

int IpDrawing::GetWidth()
{
	return m_Width;
}
int IpDrawing::GetHeight()
{
	return m_Height;
}

#pragma endregion

#pragma region IpBitmap
IpBitmap::IpBitmap()
{
	m_hBitmap = NULL;
	m_HDC = NULL;
	
}

IpBitmap::~IpBitmap()
{
	Free();
}

bool IpBitmap::Load(const std::string& path)
{
	m_HDC = CreateCompatibleDC(NULL);
	
	m_hBitmap = (HBITMAP)LoadImage(NULL, path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_hBitmap == NULL)
		return false;
	GetObject(m_hBitmap, sizeof(BITMAP), &m_Bitmap);
	return true;
}

bool IpBitmap::Create(IpDrawing* Graphic)
{
	Free();
	m_HDC = CreateCompatibleDC(*Graphic->GetHDC());
	m_hBitmap2 = CreateCompatibleBitmap(*Graphic->GetHDC(), Graphic->GetWidth(), Graphic->GetHeight());
	GetObject(m_hBitmap2, sizeof(BITMAP), &m_Bitmap);
	m_hBitmap = CreateBitmapIndirect(&m_Bitmap);
	//GetObject(m_hBitmap, sizeof(BITMAP), &m_Bitmap);

	DeleteObject(m_hBitmap2);
	//FillRect(m_HDC,&rc,)
	return true;
}

void IpBitmap::Free()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	if (m_HDC != NULL)
	{
		DeleteDC(m_HDC);
		m_HDC = NULL;
	}
}

bool IpBitmap::GetBMPFromWindow(HWND window)
{
	Free();
	m_HDC = GetWindowDC(window);
	RECT rc;
	GetWindowRect(window, &rc);
	m_hBitmap = CreateCompatibleBitmap(m_HDC, rc.right - rc.left, rc.bottom - rc.top);

	if (m_hBitmap == NULL)
		return false;
	GetObject(m_hBitmap, sizeof(BITMAP), &m_Bitmap);
}

HBITMAP* IpBitmap::GetHBitmap()
{
	return &m_hBitmap;
}
BITMAP* IpBitmap::GetBitmap()
{
	return &m_Bitmap;
}

HDC* IpBitmap::GetHDC()
{
	return &m_HDC;
}

sPixel IpBitmap::GetPixel(int x, int y)
{
	sPixel pixe2l;
	pixe2l.R = 0;
	pixe2l.G = 0;
	pixe2l.B = 0;
	//if (m_hBitmap == NULL)
	//	return pixel;
	//HDC dc = CreateCompatibleDC(m_HDC);
	//HDC dc = CreateCompatibleDC(NULL);
	//SelectObject(dc, m_hBitmap);
	return pixe2l;
}
#pragma endregion
