#include "WinFRW/IpApplication.h"
#include "WinFRW/IpDrawing.h"

#ifndef IPHIERARCHY_H
#define IPHIERARCHY_H

class IpWHierarchy : public IpWForm
{
private:
	int m_X;
	int m_Y;
	//int m_Width;
	//int m_Height;

	void PaintProc();

	IpDrawing* m_Graphic;
	IpDrawing* m_gBuff;
	IpSolidBrush* m_BackColorBrush;
	IpSolidBrush* m_Brush;
	IpPen* m_Pen;
	IpBitmap* m_BMP;

	void Test();
	void Test2();
public:
	IpWHierarchy(int x, int y, int width, int height, IpWForm* parent);
};

#endif // !IPHIERARHY_H
