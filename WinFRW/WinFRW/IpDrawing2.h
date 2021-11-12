#ifndef IPDRAWING_H
#define IPDRAWING_H
#include <Windows.h>

struct sDrawPoint
{
	int x;
	int y;

	sDrawPoint()
	{
		sDrawPoint::x = 0;
		sDrawPoint::y = 0;
	}

	sDrawPoint(int x, int y)
	{
		sDrawPoint::x = x;
		sDrawPoint::y = y;
	}
};

class IpPen
{
private:
	HPEN m_Pen;
	unsigned char m_R, m_G, m_B;
	int m_Width;
public:
	IpPen(unsigned char R, unsigned char G, unsigned char B, int width = 1);
	~IpPen();
	void SetColor(unsigned char R, unsigned char G, unsigned char B);
	void GetColor(unsigned char* R, unsigned char* G, unsigned char* B);
	void SetWidth(int width);
	HPEN* GetPen();
};

class IpSolidBrush
{
private:
	HPEN m_Pen;
	unsigned char m_R, m_G, m_B;
	HBRUSH m_Brush;
	COLORREF Color;
public:
	IpSolidBrush(unsigned char R,unsigned char G, unsigned char B);
	~IpSolidBrush();
	void SetColor(unsigned char R, unsigned char G, unsigned char B);
	void GetColor(unsigned char* R, unsigned char* G, unsigned char* B);
	HBRUSH* GetBrush();
	HPEN* GetPen();
};

class IpDrawing
{
private:
	HDC m_HDC;
	HWND m_HWND;
	PAINTSTRUCT m_PS;
	LPPOINT m_LPPoint;
	bool m_IsBegining;
	static const int m_NUMBEZIERPOINTS = 50;
public:
	IpDrawing(HWND hwnd);
	bool Begin();
	bool End();
	bool FillRectangle(IpSolidBrush* Brush, int x, int y, int width, int height);
	bool FillEllipse(IpSolidBrush* Brush, int x, int y, int width, int height);
	bool FillPie(IpSolidBrush* Brush, int x, int y, int width, int height, int x1, int y1, int x2, int y2);
	bool DrawLine(IpPen* Pen, int x1, int y1, int x2, int y2);
	bool DrawBezier(IpPen* Pen,sDrawPoint p1, sDrawPoint p2, sDrawPoint p3,sDrawPoint p4);
};
#endif // !IPDRAWING_H
