#include "IpApplication.h"

#pragma region IpTimer
int IpTimer::TimerIndex = 1;

IpTimer::IpTimer(unsigned long Interval, IpWForm* Form)
{
	m_Started = false;
	m_Interval = Interval;
	m_Form = Form;
	m_Index = TimerIndex;
	TimerIndex++;
	Form->m_Timers.push_back(this);
}

void IpTimer::Start()
{
	SetTimer(m_Form->GethWnd(), m_Index, m_Interval, (TIMERPROC)NULL);
	//int test = SetTimer(NULL, 50, 500, (TIMERPROC)NULL);
	//SetTimer(NULL, m_Index, m_Interval, NULL);
	m_Started = true;
}

void IpTimer::Stop()
{
	KillTimer(m_Form->GethWnd(), m_Index);
	m_Started = false;
}

int IpTimer::GetIndex()
{
	return m_Index;
}

bool IpTimer::IsStarted()
{
	return m_Started;
}
#pragma endregion

#pragma region IpWindow
unsigned long IpWindow::m_Index = 1;
unsigned long IpWindow::ClassName = 1;

bool IpWindow::CreateIpWindow()
{
	m_hWnd = CreateWindow(m_ClassName.data(),
		(m_WinName != "") ? m_WinName.data() : NULL,
		m_Style,
		m_XPos, m_YPos,
		m_Width, m_Height,
		(m_Parent != NULL) ? m_Parent->GethWnd() : NULL,
		m_hMenu,
		m_hInst,
		NULL);

	if (!m_hWnd)
	{
		DWORD lg = GetLastError();
		std::string val = std::to_string(lg);
		MessageBox(NULL, "Cannot create window", val.data(), MB_OK);
		return false;
	}
	//CloseWindow(hWnd);
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
	return true;
}

IpWindow::IpWindow(const std::string& WindowName,
	const std::string& className,
	int x,
	int y,
	int width,
	int height,
	DWORD windowStyle,
	IpWindow* Parent,
	HMENU HMenu)
{
	//int WS_Style;
	//m_hInst = hInst;
	m_hInst = GetModuleHandle(NULL);
	m_ClassCreated = false;
	if (className != "")
		m_ClassName = className;
	else
	{
		m_ClassName = std::to_string(ClassName);
		ClassName++;
		m_ClassCreated = true;
	}
	if (WindowName != "")
		m_WinName = WindowName;
	else
		m_WinName = "\0";

	m_Style = windowStyle;
	m_XPos = x;
	m_YPos = y;
	m_Width = width;
	m_Height = height;
	m_Parent = Parent;
	if (m_Parent != NULL)
		m_Parent->m_WindowChilds.push_back(this);
	m_hMenu = (HMenu == (HMENU)-1) ? (HMENU)(m_Index++) : HMenu;
	m_Deleted = false;
}


IpWindow::~IpWindow()
{
	Free();
}

void IpWindow::Free()
{
	if (m_Parent != NULL)
		for (int i = 0; i < m_Parent->m_WindowChilds.size(); ++i)
			if (m_Parent->m_WindowChilds[i] == this)
			{
				m_Parent->m_WindowChilds.erase(m_Parent->m_WindowChilds.begin() + i);
				break;
			}
	m_Parent = NULL;
	m_hMenu = NULL;
	while (m_WindowChilds.size() > 0)
	{
		if (!m_WindowChilds[0]->m_Deleted)
			delete m_WindowChilds[0];
	}
	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	if (m_ClassCreated)
		UnregisterClass(m_ClassName.data(), m_hInst);
	m_Deleted = true;
}

HWND IpWindow::GethWnd()
{
	return m_hWnd;
}

void IpWindow::SetHINSTANCE(HINSTANCE hInst)
{
	m_hInst = hInst;
	m_wndclass.hInstance = m_hInst;
}

HINSTANCE IpWindow::GethInst()
{
	return m_hInst;
}

bool IpWindow::SetName(std::string& name)
{
	m_WinName = name;
	SetWindowText(m_hWnd, m_WinName.data());
	UpdateWindow(m_hWnd);
	return true;
}

bool IpWindow::Move(long XPos, long YPos)
{
	m_XPos = XPos;
	m_YPos = YPos;

	if (FAILED(MoveWindow(m_hWnd, m_XPos, m_YPos, m_Width, m_Height, TRUE)))
		return FALSE;

	return TRUE;
}

bool IpWindow::Resize(long Width, long Height)
{
	m_Width = Width;
	m_Height = Height;

	if (FAILED(MoveWindow(m_hWnd, m_XPos, m_YPos, m_Width, m_Height, TRUE)))
		return FALSE;

	return TRUE;
}

void IpWindow::ShowMouse(bool show)
{
	ShowCursor(show);
}

void IpWindow::SetStyle(DWORD style)
{
	m_Style = style;
	SetClassLong(m_hWnd, GCL_STYLE, style);
	UpdateWindow(m_hWnd);
}

void IpWindow::SetHandler(WNDPROC handler)
{
	SetClassLong(m_hWnd, GCL_WNDPROC, (DWORD)handler);
}
void IpWindow::SetCursor(LPCSTR cursor)
{
	SetClassLong(m_hWnd, GCL_HCURSOR, (DWORD)LoadCursor(NULL, cursor));
}
void IpWindow::SetIcon(LPCSTR icon)
{
	SetClassLong(m_hWnd, GCL_HICON, (DWORD)LoadIcon(NULL, icon));
}
void IpWindow::SetIConsm(LPCSTR iconsm)
{
	SetClassLong(m_hWnd, GCL_HICONSM, (DWORD)LoadIcon(NULL, iconsm));
}
void IpWindow::SetMenuName(LPCSTR menu)
{
	SetClassLong(m_hWnd, GCL_MENUNAME, (DWORD)menu);
}
void IpWindow::SetBackColor(unsigned char R, unsigned char G, unsigned char B)
{
	m_BackColor = RGB(R, G, B);
	SetClassLong(m_hWnd, GCL_HBRBACKGROUND, (DWORD)CreateSolidBrush(m_BackColor));
	Redraw();

}

COLORREF IpWindow::GetBackColor()
{
	return m_BackColor;
}

HMENU IpWindow::GetHMenu()
{
	return m_hMenu;
}

const std::string& IpWindow::GetName()
{
	return m_WinName;
}

DWORD IpWindow::GetXPos()
{
	return m_XPos;
}
DWORD IpWindow::GetYPos()
{
	return m_YPos;
}
DWORD IpWindow::GetWidht()
{
	return m_Width;
}
DWORD IpWindow::GetHeight()
{
	return m_Height;
}

IpWindow* IpWindow::GetParent()
{
	return m_Parent;
}

void IpWindow::Update()
{
	UpdateWindow(m_hWnd);
}

void IpWindow::Redraw()
{
	RECT rc;
	rc.left = 0;
	rc.right = m_Width;
	rc.top = 0;
	rc.bottom = m_Height;
	InvalidateRect(m_hWnd, &rc, true);
}

POINT IpWindow::GetCursorPosition()
{
	POINT p = IpApplication::GetCursorPosition();
	p.x -= m_XPos;
	p.y -= m_YPos;
	return p;
}

void IpWindow::Show(bool visible)
{
	if (visible)
	{
		if (m_Enabled)
			return;
		m_Enabled = true;
		ShowWindow(m_hWnd, SW_NORMAL);
		UpdateWindow(m_hWnd);
	}
	else
	{
		if (!m_Enabled)
			return;
		m_Enabled = false;
		ShowWindow(m_hWnd, SW_HIDE);
	}
}

bool IpWindow::IsActive()
{
	return m_Enabled;
}

#pragma endregion

#pragma region IpWForm
IpWForm::IpWForm(const std::string& WindowName,
	int x,
	int y,
	int width,
	int height,
	UINT classStyle,
	DWORD windowStyle,
	IpWForm* Parent,
	WNDPROC handler,
	LPCTSTR menuName) : IpWindow(WindowName, "", x, y, width, height, windowStyle, Parent, NULL)
{
	m_wndclass.cbSize = sizeof(WNDCLASSEX);								// Размер структуры WNDCLASSEX в байтах
	m_wndclass.style = classStyle;										// Стиль окна
	m_wndclass.lpfnWndProc = handler;									// Указатель на обработчик сообщений
	m_wndclass.cbClsExtra = 0;											// Инзачальный размер структуры в байтах (хз для чего, зануляем)
	m_wndclass.cbWndExtra = 0;											// Изначальный размер окна в байтах (зануляем)
	m_wndclass.hInstance = m_hInst;										// Дескриптор окна
	m_wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// Дескриптор класса значка: Первый параметр - Дескриптор на модуль, 
																		// который содержит значок - если NULL, то можем использовать стандартные значки. 
																		// Второй параметр - Указатель на строку, содержащий имя значка
	m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);				// Дескрипток класса указателя (Параметры такие же, что и у иконки)
	m_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// Задний фон // GetStockObject - функция, которая получает кисть
	m_wndclass.lpszMenuName = menuName;								// Имя ресурса меню
	m_wndclass.lpszClassName = m_ClassName.data();								// Имя класса окна
	m_wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// Значок самого окна, идентично hIcon

	if (!RegisterClassEx(&m_wndclass))
	{
		MessageBox(NULL, "Cannot register window", "Error", MB_OK);
		Free();
		return;
	}

	m_ActiveWindows.push_back(this);
	m_Enabled = true;
	CreateIpWindow();
}

#pragma endregion

#pragma region IpWPanel
IpWPanel::IpWPanel(int x, int y, int width, int height, IpWForm* Parent) :
	//IpWForm((std::string&)"Panel", x, y, width, height,
	IpWForm("Panel", x, y, width, height,
		(UINT)ClassStyles::STANDART,
		(DWORD)WinStyles::CHILD | (DWORD)WinStyles::PARENT | (DWORD)WinStyles::BORDER, Parent, IpApplication::fnStandartProc, NULL)
{
	
}
#pragma endregion

#pragma region IpWOpenFileDialog
IpWOpenFileDialog::IpWOpenFileDialog(const std::string& Name,IpWindow* Parent, DWORD Flags)
{
	m_Filters = "";
	m_Name = "";
	ZeroMemory(&m_Ofn, sizeof(m_Ofn));
	m_Ofn.lStructSize = sizeof(m_Ofn);
	m_Ofn.hwndOwner = (Parent != NULL) ? Parent->GethWnd() : NULL;
	m_Ofn.hInstance = (Parent != NULL) ? Parent->GethInst() : NULL;
	m_Ofn.lpstrFile = m_BuffFile;
	//m_Ofn.lpstrFile = (char*)m_BuffFile.data();
	m_Ofn.lpstrFile[0] = '\0';
	m_Ofn.lpTemplateName = NULL;
	m_Ofn.nMaxFile = sizeof(m_BuffFile);
	m_Ofn.lpstrFilter = NULL;
	m_Ofn.nFilterIndex = 1;
	m_Ofn.lpstrFileTitle = NULL;
	m_Ofn.nMaxFileTitle = 0;
	m_Ofn.lpstrInitialDir = NULL;
	m_Ofn.Flags = Flags;

	m_Name = Name;
	m_Ofn.lpstrTitle = (m_Name == "") ? NULL : m_Name.c_str();
}

void IpWOpenFileDialog::AddFilters(int Numfilters,const char* FilterName, const char* Filter, ...)
{
	if (Numfilters <= 0 || FilterName == "" || Filter == "")
	{
		m_Filters = "";
		return;
	}

	m_Filters = FilterName;
	m_Filters.push_back('\0');
	m_Filters += "*.";
	m_Filters += Filter;
	m_Filters.push_back('\0');

	const char** data = &Filter;

	for (int i = 0; i < Numfilters - 1; ++i)
	{
		data++;
		m_Filters += *data;
		m_Filters.push_back('\0');
		data++;
		m_Filters += "*.";
		m_Filters += *data;
		m_Filters.push_back('\0');
	}
	m_Filters.push_back('\0');
	m_Ofn.lpstrFilter = m_Filters.c_str();
}

void IpWOpenFileDialog::SetName(const std::string& name)
{
	m_Name = name;
	m_Ofn.lpstrTitle = (m_Name == "") ? NULL : m_Name.c_str();
}
void IpWOpenFileDialog::SetFlags(DWORD Flags)
{
	m_Ofn.Flags = Flags;
}

bool IpWOpenFileDialog::ShowDialog()
{
	return GetOpenFileName(&m_Ofn);
	int qqq = 0;
}

const char* IpWOpenFileDialog::GetPath()
{
	return m_BuffFile;
}
#pragma endregion

#pragma region IpWSaveFileDialog
IpWSaveFileDialog::IpWSaveFileDialog(const std::string& Name, IpWindow* Parent, DWORD Flags)
{
	m_Filters = "";
	m_Name = "";
	ZeroMemory(&m_Ofn, sizeof(m_Ofn));
	m_Ofn.lStructSize = sizeof(m_Ofn);
	m_Ofn.hwndOwner = (Parent != NULL) ? Parent->GethWnd() : NULL;
	m_Ofn.hInstance = (Parent != NULL) ? Parent->GethInst() : NULL;
	m_Ofn.lpstrFile = m_BuffFile;
	m_Ofn.lpstrFile[0] = '\0';
	m_Ofn.lpTemplateName = NULL;
	m_Ofn.nMaxFile = sizeof(m_BuffFile);
	m_Ofn.lpstrFilter = NULL;
	m_Ofn.nFilterIndex = 1;
	m_Ofn.lpstrFileTitle = NULL;
	m_Ofn.nMaxFileTitle = 0;
	m_Ofn.lpstrInitialDir = NULL;
	m_Ofn.Flags = Flags;

	m_Name = Name;
	m_Ofn.lpstrTitle = (m_Name == "") ? NULL : m_Name.c_str();
}

IpWSaveFileDialog::~IpWSaveFileDialog()
{
	
}

void IpWSaveFileDialog::SetName(const std::string& name)
{
	m_Name = name;
	m_Ofn.lpstrTitle = (m_Name == "") ? NULL : m_Name.c_str();
}
void IpWSaveFileDialog::SetFlags(DWORD Flags)
{
	m_Ofn.Flags = Flags;
}


void IpWSaveFileDialog::AddFilters(int Numfilters, const char* FilterName, const char* Filter, ...)
{
	if (Numfilters <= 0 || FilterName == "" || Filter == "")
	{
		m_Filters = "";
		return;
	}

	m_Filters = FilterName;
	m_Filters.push_back('\0');
	m_Filters += "*.";
	m_Filters += Filter;
	m_Filters.push_back('\0');

	m_Ext.push_back(Filter);
	const char** data = &Filter;

	for (int i = 0; i < Numfilters - 1; ++i)
	{
		data++;
		m_Filters += *data;
		m_Filters.push_back('\0');
		data++;
		m_Filters += "*.";
		m_Filters += *data;
		m_Filters.push_back('\0');

		m_Ext.push_back(*data);
	}
	m_Filters.push_back('\0');
	m_Ofn.lpstrFilter = m_Filters.c_str();
}

bool IpWSaveFileDialog::ShowDialog()
{
	return GetSaveFileName(&m_Ofn);
}

std::string IpWSaveFileDialog::GetPath()
{
	return (std::string)m_BuffFile + '.' + m_Ext[m_Ofn.nFilterIndex - 1].data();
}
#pragma endregion

#pragma region IpWButton
IpWButton::IpWButton(const std::string& ButtonName,
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent) : IpWindow(ButtonName, "BUTTON", x, y,
		width, height, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, Parent, (HMENU)-1)
{
	CreateIpWindow();
}

#pragma endregion

#pragma region IpWCheckBox
IpWCheckBox::IpWCheckBox(const std::string& CheckBoxName,
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent) : IpWindow(CheckBoxName, "BUTTON", x, y,
		width, height, WS_VISIBLE | WS_CHILD | BS_CHECKBOX, Parent, (HMENU)-1)
{
	m_Checked = false;
	CreateIpWindow();
}


void IpWCheckBox::SetState(bool checked)
{
	m_Checked = checked;
	SendMessage(m_hWnd, BM_SETCHECK, m_Checked, 0L);
}
bool IpWCheckBox::Checked()
{
	return m_Checked;
}
#pragma endregion

#pragma region IpWRadioButton

IpWRadioButton::IpWRadioButton(const std::string& RadioButtonName,
	int group,
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent) : IpWindow(RadioButtonName, "BUTTON", x, y,
		width, height, WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, Parent, (HMENU)-1)
{
	m_Checked = false;
	ButtonGroup = group;
	CreateIpWindow();
}

void IpWRadioButton::SetState(bool checked)
{
	m_Checked = checked;
	SendMessage(m_hWnd, BM_SETCHECK, m_Checked, 0L);
}

bool IpWRadioButton::Checked()
{
	return m_Checked;
}
#pragma endregion

#pragma region IpWListBox
IpWListBox::IpWListBox(
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent,
	DWORD Flags) : IpWindow("", "listbox", x, y,
		width, height, Flags, Parent, (HMENU)-1)
	//width, height, WS_VISIBLE | WS_CHILD | LBS_STANDARD, Parent, (HMENU)(501 + NumButton))
{
	CreateIpWindow();
}

int IpWListBox::NumItems()
{
	return SendMessage(m_hWnd, LB_GETCOUNT, 0, 0L);
}

bool IpWListBox::AddString(const std::string& string)
{
	SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)(LPSTR)string.data());
	UpdateWindow(m_hWnd);
	return true;
}

bool IpWListBox::DeleteItem(int index)
{
	if (index < 0 || index > NumItems() - 1)
		return false;
	SendMessage(m_hWnd, LB_DELETESTRING, index, 0L);
	UpdateWindow(m_hWnd);
	return true;
}

int IpWListBox::GetString(int index, char* buff)
{
	int lg;
	lg = SendMessage(m_hWnd, LB_GETTEXT, (WPARAM)index, (LPARAM)buff);
	if (lg == -1)
		return -1;
	buff[lg] = '\0';
	return lg;
}

int IpWListBox::FindStringPrefix(int StartIndex, char* prefix)
{
	if (StartIndex > NumItems() - 1 || prefix == NULL)
		return -1;
	return SendMessage(m_hWnd, LB_FINDSTRING, (WPARAM)StartIndex, (LPARAM)(LPSTR)prefix);
}

int IpWListBox::FindString(int StartIndex, char* string)
{
	if (StartIndex > NumItems() - 1 || string == NULL)
		return -1;
	return SendMessage(m_hWnd, LB_FINDSTRINGEXACT, (WPARAM)StartIndex, (LPARAM)(LPSTR)string);
}

int IpWListBox::GetSelectedIndex()
{
	return SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0L);
}

void IpWListBox::SetSelectedIndex(int index)
{
	SendMessage(m_hWnd, LB_SETCURSEL, (WPARAM)index, 0L);
	UpdateWindow(m_hWnd);
}

void IpWListBox::Clear()
{
	SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0L);
	UpdateWindow(m_hWnd);
}
#pragma endregion

#pragma region IpWComboBox
IpWComboBox::IpWComboBox(
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent,
	DWORD Flags) : IpWindow("", "Combobox", x, y, width, height, WS_CHILD | WS_VISIBLE | WS_VSCROLL
		| CBS_AUTOHSCROLL | Flags,Parent,(HMENU)-1)
{
	CreateIpWindow();
}

int IpWComboBox::NumItems()
{
	return SendMessage(m_hWnd, CB_GETCOUNT, 0, 0L);
}

bool IpWComboBox::AddString(const std::string& string)
{
	SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)(LPSTR)string.data());
	UpdateWindow(m_hWnd);
	return true;
}
bool IpWComboBox::DeleteItem(int index)
{
	if (index < 0 || index > NumItems() - 1)
		return false;
	SendMessage(m_hWnd, CB_DELETESTRING, index, 0L);
	UpdateWindow(m_hWnd);
	return true;
}

int IpWComboBox::GetString(int index, char* buff)
{
	int lg;
	lg = SendMessage(m_hWnd, CB_GETLBTEXT, (WPARAM)index, (LPARAM)buff);
	if (lg == -1)
		return -1;
	buff[lg] = '\0';
	return lg;
}

int IpWComboBox::FindStringPrefix(int StartIndex, char* prefix)
{
	if (StartIndex > NumItems() - 1 || prefix == NULL)
		return -1;
	return SendMessage(m_hWnd, CB_FINDSTRING, (WPARAM)StartIndex, (LPARAM)(LPSTR)prefix);
}


int IpWComboBox::FindString(int StartIndex, char* string)
{
	if (StartIndex > NumItems() - 1 || string == NULL)
		return -1;
	return SendMessage(m_hWnd, CB_FINDSTRINGEXACT, (WPARAM)StartIndex, (LPARAM)(LPSTR)string);
}
int IpWComboBox::GetSelectedIndex()
{
	return SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
}
void IpWComboBox::SetSelectedIndex(int index)
{
	SendMessage(m_hWnd, CB_SETCURSEL, (WPARAM)index, 0L);
	UpdateWindow(m_hWnd);
}
void IpWComboBox::Clear()
{
	SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0L);
	UpdateWindow(m_hWnd);
}

bool IpWComboBox::IsExpandedState()
{
	return SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
}

bool IpWComboBox::IsDroppedState()
{
	return SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
}


void IpWComboBox::SetDropDown(bool state)
{
	SendMessage(m_hWnd, CB_SHOWDROPDOWN, (WPARAM)state, 0L);
	UpdateWindow(m_hWnd);
}

void IpWComboBox::SetExpandedState(bool state)
{
	SendMessage(m_hWnd, CB_SETEXTENDEDUI, (WPARAM)state, 0L);
	UpdateWindow(m_hWnd);
}

#pragma endregion

#pragma region IpWLabel
IpWLabel::IpWLabel(const std::string& LabelName,
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent,
	DWORD Style) : IpWindow(LabelName, "static", x, y,
		width, height, WS_CHILD | WS_VISIBLE | Style, Parent, (HMENU)-1)
{
	CreateIpWindow();
}
#pragma endregion

#pragma region IpWTextBox
IpWTextBox::IpWTextBox(
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent) : IpWindow("", "EDIT", x, y, width, height,
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
		Parent, (HMENU)-1)
{
	CreateIpWindow();
}

void IpWTextBox::Clear()
{
	SendMessage(m_hWnd, EM_SETSEL, 0, MAKELPARAM(0, -1));
	SendMessage(m_hWnd, EM_REPLACESEL, 0, (LPARAM)(LPSTR)"");
}

int IpWTextBox::GetLine(char* buff)
{
	int lg = SendMessage(m_hWnd, EM_GETLINE, (WPARAM)0, (LPARAM)(LPSTR)buff);
	buff[lg] = '\0';
	return lg;
}

void IpWTextBox::ReadOnly(bool value)
{
	m_ReadOnly = value;
	SendMessage(m_hWnd, EM_SETREADONLY, (WPARAM)(UINT)value, 0L);
}

int IpWTextBox::GetLenght()
{
	return SendMessage(m_hWnd, EM_LINELENGTH, (WPARAM)-1, 0L);
}
#pragma endregion

#pragma region IpWRichTextBox
IpWRichTextBox::IpWRichTextBox(
	int x,
	int y,
	int width,
	int height,
	IpWForm* Parent) : IpWindow("", "EDIT", x, y, width, height,
		WS_BORDER | WS_CHILD | WS_VISIBLE| ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
		Parent, (HMENU)-1)
{
	CreateIpWindow();
	m_ReadOnly = false;
}

int IpWRichTextBox::GetNumLines()
{
	return SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0L);
}

void IpWRichTextBox::Clear()
{
	SendMessage(m_hWnd, EM_SETSEL, 0, MAKELPARAM(0, -1));
	SendMessage(m_hWnd, EM_REPLACESEL, 0, (LPARAM)(LPSTR)"");
}

int IpWRichTextBox::GetLine(int line,char* buff)
{
	int lg = SendMessage(m_hWnd, EM_GETLINE, (WPARAM)line, (LPARAM)(LPSTR)buff);
	buff[lg] = '\0';
	return lg;
}

void IpWRichTextBox::ReadOnly(bool value)
{
	m_ReadOnly = value;
	SendMessage(m_hWnd, EM_SETREADONLY, (WPARAM)(UINT)value, 0L);
}

int IpWRichTextBox::GetLenght(int line)
{
	return (line < 0) ? GetWindowTextLength(m_hWnd) : SendMessage(m_hWnd, EM_LINELENGTH, (WPARAM)line, 0L);
}

bool IpWRichTextBox::SetText(char* buff)
{
	if (buff == NULL)
		return false;
	return SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)buff);;
}
bool IpWRichTextBox::AddText(char* buff,bool newLine)
{
	if (buff == NULL)
		return false;
	int lg = GetLenght();
	std::string s;
	if (newLine)
		s += "\r\n";
	s += buff;
	SendMessage(m_hWnd, EM_SETSEL, (WPARAM)lg, (LPARAM)lg);
	SendMessage(m_hWnd, EM_REPLACESEL, 0, (LPARAM)&s[0]);
	return true;
}
#pragma endregion

#pragma region IpWScroll
IpWScroll::IpWScroll(int x, int y, int width, int height, ScrollStyles Styleflag, IpWForm* m_Parent) :
	IpWindow("","scrollbar",x,y,width,height, WS_CHILD | WS_VISIBLE | (DWORD)Styleflag,m_Parent,(HMENU)-1)
{
	CreateIpWindow();
	m_Show = true;
	SetRangeValues(0, 100);
	SetStepsValues(1, 10);
}

void IpWScroll::SetRangeValues(int min, int max)
{
	SetScrollRange(m_hWnd, SB_CTL, min, max, true);
}

int IpWScroll::GetMinValue()
{
	GetScrollRange(m_hWnd, SB_CTL, &m_Min, NULL);
	return m_Min;
}

int IpWScroll::GetMaxValue()
{
	GetScrollRange(m_hWnd, SB_CTL, NULL, &m_Max);
	return m_Max;
}

bool IpWScroll::SetValue(int value)
{
	return SetScrollPos(m_hWnd, SB_CTL, value, true);
}

int IpWScroll::GetCurrentValue()
{
	return GetScrollPos(m_hWnd, SB_CTL);
}

void IpWScroll::Show(bool show)
{
	m_Show = show;
	ShowScrollBar(m_hWnd, SB_CTL, m_Show);
}

bool IpWScroll::IsActive()
{
	return m_Show;
}

void IpWScroll::Enable(bool enable)
{
	m_Enable = enable;
	EnableScrollBar(m_hWnd, SB_CTL, ESB_DISABLE_BOTH);
}

bool IpWScroll::IsEnabled()
{
	return m_Enable;
}

void IpWScroll::SetStepsValues(int smallStep, int bigStep)
{
	m_SStep = smallStep;
	m_BStep = bigStep;
}
int IpWScroll::GetSmallStep()
{
	return m_SStep;
}
int IpWScroll::GetBigStep()
{
	return m_BStep;
}
#pragma endregion

IpApplication::IpApplication()
{
	m_hInst = GetModuleHandle(NULL);
}

bool IpApplication::Run()
{
	MSG Msg;
	
	if (Init())	// Если инициализация прошла
	{
		ZeroMemory(&Msg, sizeof(MSG));
		while (1)
		{
			if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
			//if (GetMessage(&Msg, 0, 0, 0))
			{
				if (Msg.message == WM_QUIT)
					break;
				else
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
			else
			{
				if (!Frame())
					break;
				WaitMessage();
			}
		}
		/*while (Msg.message != WM_QUIT)
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			//if (GetMessage(&Msg, NULL, 0, 0))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			if (!Frame())
				break;
		}*/
	}

	Shutdown();

	while (m_ActiveWindows.size() > 0)
	{
		if (m_ActiveWindows[0]->GetParent() == NULL)
			delete m_ActiveWindows[0];
		m_ActiveWindows.erase(m_ActiveWindows.begin());
	}

	// Чистим ресурсы
	return TRUE;
}

LRESULT IpApplication::fnStandartProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_COMMAND:
	{
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				if (!m_ActiveWindows[i]->IsActive())
					return(0);
				for (int j = 0; j < m_ActiveWindows[i]->m_WindowChilds.size(); ++j)
				{
					if ((WPARAM)(m_ActiveWindows[i]->m_WindowChilds[j]->GetHMenu()) == LOWORD(wParam))
					{
						IpWindow* wnd = (IpWButton*)m_ActiveWindows[i]->m_WindowChilds[j];
						int command = HIWORD(wParam);
						if (dynamic_cast<IpWButton*>(wnd))
						{
							switch (command)
							{
							case BN_CLICKED:
								((IpWButton*)wnd)->ClickedEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWListBox*>(wnd))
						{
							switch (command)
							{
							case LBN_DBLCLK:
								((IpWListBox*)wnd)->DoubleClickedEvent.Exec();
								return(0);
							case LBN_SELCHANGE:
								((IpWListBox*)wnd)->ChangeChoiseEvent.Exec();
								return(0);
							case LBN_SETFOCUS:
								((IpWListBox*)wnd)->SetFocusEvent.Exec();
								return(0);
							case LBN_KILLFOCUS:
								((IpWListBox*)wnd)->KillFocusEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWTextBox*>(wnd))
						{
							switch (command)
							{
							case EN_CHANGE:
								((IpWTextBox*)wnd)->ChangedEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWCheckBox*>(wnd))
						{
							switch (command)
							{
							case BN_CLICKED:
								((IpWCheckBox*)wnd)->SetState(!((IpWCheckBox*)wnd)->Checked());
								((IpWCheckBox*)wnd)->ClickedEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWRadioButton*>(wnd))
						{
							switch (command)
							{
							case BN_CLICKED:
								IpWRadioButton* Rb = ((IpWRadioButton*)wnd);
								Rb->SetState(true);
								for (int k = 0; k < m_ActiveWindows[i]->m_WindowChilds.size(); ++k)
								{
									if (j == k)
										continue;
									//if (m_ActiveWindows[i]->m_WindowChilds[k]->GetType() == 6)
									IpWindow* lwnd = m_ActiveWindows[i]->m_WindowChilds[k];
									if (dynamic_cast<IpWRadioButton*>(lwnd))
									{
										if (((IpWRadioButton*)wnd)->ButtonGroup == ((IpWRadioButton*)lwnd)->ButtonGroup)
											((IpWRadioButton*)lwnd)->SetState(false);
									}
								}
								((IpWRadioButton*)wnd)->ClickedEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWComboBox*>(wnd))
						{
							switch (command)
							{
							case CBN_CLOSEUP:
								((IpWComboBox*)wnd)->CloseUpEvent.Exec();
								return(0);
							case CBN_DBLCLK:
								((IpWComboBox*)wnd)->DoubleClickedEvent.Exec();
								return(0);
							case CBN_EDITCHANGE:
								((IpWComboBox*)wnd)->ChangedTextEvent.Exec();
								return(0);
							case CBN_SETFOCUS:
								((IpWComboBox*)wnd)->SetFocusEvent.Exec();
								return(0);
							case CBN_KILLFOCUS:
								((IpWComboBox*)wnd)->KillFocusEvent.Exec();
								return(0);
							case CBN_SELENDOK:
								((IpWComboBox*)wnd)->ChangeChoiseEvent.Exec();
								return(0);
							}
							return(0);
						}
						else if (dynamic_cast<IpWRichTextBox*>(wnd))
						{
							switch (command)
							{
							case EN_CHANGE:
								((IpWRichTextBox*)wnd)->ChangedEvent.Exec();
								return(0);
							}
						}
						return(0);
					}
				}
			}
		}
		return 0;
	}
	case WM_CREATE:	// Создание окна
		return (0);
	case WM_PAINT: // Отрисовка окна
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				if (!m_ActiveWindows[i]->Paint.IsEmpty())
				{
					PAINTSTRUCT ps;
					BeginPaint(hWnd, &ps);
					m_ActiveWindows[i]->Paint.Exec();
					//InvalidateRect(hWnd, NULL, true);
					//RedrawWindow(hWnd,NULL,)
					//UpdateWindow(hWnd);
					EndPaint(hWnd, &ps);
					return 0;
					//return DefWindowProc(hWnd, iMsg, wParam, lParam);
				}
			}
		}
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	case WM_LBUTTONDOWN:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->LMouseDown.Exec();
				return(0);
			}
		}
		return(0);
	case WM_RBUTTONDOWN:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->RMouseDown.Exec();
				return(0);
			}
		}
		return(0);
	case WM_LBUTTONUP:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->LMouseUp.Exec();
				return(0);
			}
		}
		return(0);
	case WM_LBUTTONDBLCLK:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->LMouseDClick.Exec();
				return(0);
			}
		}
		return(0);
	case WM_RBUTTONDBLCLK:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->RMouseDClick.Exec();
				return(0);
			}
		}
		return(0);
	case WM_MOVE:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->m_XPos = (int)(short)LOWORD(lParam);
				m_ActiveWindows[i]->m_YPos = (int)(short)HIWORD(lParam);
				return(0);
			}
		}
		return(0);
	case WM_MOUSEMOVE:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				m_ActiveWindows[i]->MouseMove.Exec();
				return(0);
			}
		}
		return(0);
	case WM_MOUSEWHEEL:
		return(0);
	/*case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		return (0);*/
	case WM_TIMER:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				for (int j = 0; j < m_ActiveWindows[i]->m_Timers.size(); ++j)
				{
					if (m_ActiveWindows[i]->m_Timers[j]->GetIndex() == wParam)
					{
						m_ActiveWindows[i]->m_Timers[j]->Tick.Exec();
						return(0);
					}
				}
			}
		}
		return(0);
	case WM_VSCROLL:
	case WM_HSCROLL:
		for (int i = 0; i < m_ActiveWindows.size(); ++i)
		{
			if (m_ActiveWindows[i]->GethWnd() == hWnd)
			{
				for (int j = 0; j < m_ActiveWindows[i]->m_WindowChilds.size(); ++j)
				{
					IpWScroll* scrl = ((IpWScroll*)m_ActiveWindows[i]->m_WindowChilds[j]);
					if (scrl->GethWnd() == (HWND)lParam)
					{
						int command = LOWORD(wParam);
						switch (command)
						{
						case SB_THUMBTRACK:
							scrl->SetValue(HIWORD(wParam));
							break;
						case SB_ENDSCROLL:
							scrl->EndScroll.Exec();
							return(0);
						case SB_LINEUP:
							scrl->SetValue(scrl->GetCurrentValue() - scrl->GetSmallStep());
							break;
						case SB_LINEDOWN:
							scrl->SetValue(scrl->GetCurrentValue() + scrl->GetSmallStep());
							break;
						case SB_PAGEUP:
							scrl->SetValue(scrl->GetCurrentValue() - scrl->GetBigStep());
							break;
						case SB_PAGEDOWN:
							scrl->SetValue(scrl->GetCurrentValue() + scrl->GetBigStep());
							break;
						}
							scrl->Scroll.Exec();
						return(0);
					}
				}
			}
		}
		return(0);
	case WM_DESTROY: // Закрытие окна
		PostQuitMessage(0);	// Функция завершения программы Windows
		return(0);
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam); // Если сообщение нет в списке, возвращаемся к функции WinMain с помощью этой функции
	}
}

POINT IpApplication::GetCursorPosition()
{
	POINT p;
	GetCursorPos(&p);
	return p;
}

POINT IpApplication::GetScreenResolution()
{
	POINT p;
	HDC m_hd = GetDC(NULL);
	p.x = GetDeviceCaps(m_hd, HORZRES);
	p.y = GetDeviceCaps(m_hd, VERTRES);
	ReleaseDC(NULL, m_hd);
	return p;
}