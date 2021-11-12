#ifndef IPDRAWING_H
#define IPDRAWING_H
#include <Windows.h>
#include <string>

class IpDrawing;
class IpPen;
class IpSolidBrush;
class IpBitmap;

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
	IpSolidBrush(COLORREF color);
	~IpSolidBrush();
	void SetColor(unsigned char R, unsigned char G, unsigned char B);
	void GetColor(unsigned char* R, unsigned char* G, unsigned char* B);
	HBRUSH* GetBrush();
	HPEN* GetPen();
};

struct sPixel
{
	int R;
	int G;
	int B;
};

class IpBitmap
{
private:
	HDC m_HDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hBitmap2;
	BITMAP m_Bitmap;
public:
	IpBitmap();
	~IpBitmap();
	bool Load(const std::string& path);
	bool Create(IpDrawing* Graphic);
	void Free();
	HBITMAP* GetHBitmap();
	bool GetBMPFromWindow(HWND window);
	BITMAP* GetBitmap();
	HDC* GetHDC();
	sPixel GetPixel(int x,int y);
};

class IpFont
{
private:
	HFONT m_Font;
	HDC m_HDC;
	std::string m_FontName;

	int m_R;
	int m_G;
	int m_B;

	DWORD m_Flags;
	int m_Height;
public:
	enum eFontFlags
	{
		BOLD = 0x1,	// Жирный
		ITALIC = 0x2,	// Курсив
		UNDERLINE = 0x4, // Подчеркнутый
		STRIKEOUT = 0x8, // Зачеркнутый
	};

	/**
	@brief Конструктор класса
	fontName - название системного шрифта
	height - размер шрифта
	flags - флаги создания (0 или значения IpFont::eFontFlags)
	**/
	IpFont(const std::string& fontName, int height, DWORD flags = 0);
	~IpFont();
	void Free();
	void SetSize(int size);
	void SetColor(int r, int g, int b);
	COLORREF GetColor();
	HFONT* GetFont();
	int GetFontSize();
};

class IpDrawing
{
private:
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
	RECT m_Rect;
	HDC m_HDC;
	HWND m_HWND;
	PAINTSTRUCT m_PS;
	LPPOINT m_LPPoint;
	bool m_IsBegining;
	static const int m_NUMBEZIERPOINTS = 50;
	int type;
public:
	struct ScaleModes
	{
		enum EScaleModes
		{
			AND = BLACKONWHITE,	// Результирующий пиксель при сжатии будет суммой соседних
			OR = WHITEONBLACK,	// Результирующий пиксель при сжатии будет равен самому светлому соседу
			NO = COLORONCOLOR,	// Не учитывает соседей
		};
	};

	IpDrawing(HWND hwnd, int x = 0,int y = 0, int width = 0, int height = 0);
	IpDrawing(IpBitmap* Bitmap);
	void Free();
	~IpDrawing();


	bool Begin();
	bool End();

	bool FillRectangle(IpSolidBrush* Brush, int x, int y, int width, int height);
	bool FillEllipse(IpSolidBrush* Brush, int x, int y, int width, int height);
	bool FillPie(IpSolidBrush* Brush, int x, int y, int width, int height, int x1, int y1, int x2, int y2);
	bool DrawLine(IpPen* Pen, int x1, int y1, int x2, int y2);
	bool DrawBezier(IpPen* Pen,sDrawPoint p1, sDrawPoint p2, sDrawPoint p3,sDrawPoint p4);
	bool DrawBitmap(IpBitmap* bitmap, int x = 0, int y = 0, int width = 0, int height = 0,
		ScaleModes::EScaleModes scaleMode = ScaleModes::NO);
	bool DrawBitmapUnscaled(IpBitmap* bitmap, int xOffset = 0, int yOffset = 0);
	bool PrintText(IpFont* Font, const char* Text, int x, int y);
	HDC* GetHDC();
	HWND* GetHWND();
	int GetWidth();
	int GetHeight();
};
#endif // !IPDRAWING_H
