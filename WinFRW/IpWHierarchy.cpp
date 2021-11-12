#include "IpWHierarchy.h"

void IpWHierarchy::Test()
{
	m_X += 10;
	//InvalidateRect(m_hWnd, NULL, true);
	Test2();
}

void IpWHierarchy::Test2()
{
	//m_Graphic->Begin();
	m_gBuff->FillRectangle(m_BackColorBrush, 0, 0, m_Width, m_Height);
	m_gBuff->FillRectangle(m_Brush, m_X, 0, 50, 50);
	//m_Graphic->FillRectangle(m_Brush, m_X, 0, 50, 50);
	m_Graphic->DrawBitmapUnscaled(m_BMP);
	//m_Graphic->End();
}

IpWHierarchy::IpWHierarchy(int x, int y, int width, int height, IpWForm* parent) : IpWForm("Hierarchy", x, y, width, height,
	(UINT)ClassStyles::STANDART,
	(DWORD)WinStyles::CHILD | (DWORD)WinStyles::PARENT | (DWORD)WinStyles::BORDER, parent, IpApplication::fnStandartProc, NULL)
{

	m_Graphic = new IpDrawing(m_hWnd,0,0,width,height);
	//m_Graphic->GetWidht()
	m_BMP = new IpBitmap();
	m_BMP->Create(m_Graphic);
	m_gBuff = new IpDrawing(m_BMP);
	m_Pen = new IpPen(255, 0, 0);
	m_Brush = new IpSolidBrush(255, 0, 0);
	m_BackColorBrush = new IpSolidBrush(255,255,255);

	m_X = 0;

	IpWButton* btn = new IpWButton("t", 100, 100, 80, 30, this);
	btn->ClickedEvent = CreateIpEventHandler(IpWHierarchy, Test);

	//InvalidateRect(m_hWnd, NULL, true);
	Test2();
	Paint += CreateIpEventHandler(IpWHierarchy, PaintProc);
	UpdateWindow(m_hWnd);
}

void IpWHierarchy::PaintProc()
{
	Test2();
	//m_gBuff->FillRectangle(m_BackColorBrush, 0, 0, m_Width, m_Height);
	//m_gBuff->FillRectangle(m_Brush, m_X, 0, 50, 50);
	//m_Graphic->DrawBitmapUnscaled(m_BMP);
}