#ifndef MAIN_H
#define MAIN_H

#include <Windows.h>
#include <string>
#include <vector>
//#include <winusb.h>

//class IpApplication;
class IpTimer;
class IpDrawing;
class IpPen;
class IpFont;
class IpSolidBrush;
class IpBitmap;

#pragma region IPDELEGATE

#define CreateIpEventHandler(class,func) new IpEventMethod<class>(this,&class::func)
#define CreateIpEventHandlerFunction(func) new IpEventFunction(func)

class IpEventFunc
{
public:
	virtual void exec() = 0;
	virtual int Equal(IpEventFunc* fn) = 0;
	virtual ~IpEventFunc() { }
};

template <typename T, typename... args>
class IpEventMethodDynamic : public IpEventFunc
{
private:
	T(*func)(...);
public:
	IpEventMethodDynamic(T(*function)(...))
	{
		func = function;
	}

	~IpEventMethodDynamic() 
	{
		func = NULL;
	}

	T exec()
	{
		return func();
	}
};

class IpEventFunction : public IpEventFunc
{
private:
	void(*func)();
public:
	IpEventFunction(void(*function)())
	{
		func = function;
	}

	~IpEventFunction()
	{
		func = NULL;
	}

	void exec()
	{
		func();
	}

	int Equal(IpEventFunc* f1)
	{
		if (this == f1)
			return 1;
		IpEventFunction* val = dynamic_cast<IpEventFunction*>(f1);
		if (val != NULL && ((IpEventFunction*)f1)->func == func)
			return 2;
		return 0;
	}
};

template<typename T>
class IpEventMethod : public IpEventFunc
{
private:
	T* classObj;
	void (T::*method)();
public:
	IpEventMethod(T* t, void (T::*method)())
	{
		classObj = t;
		this->method = method;
	}
	~IpEventMethod()
	{
		classObj = NULL;
		method = NULL;
	}

	void exec()
	{
		(classObj->*method)();
	}

	int Equal(IpEventFunc* f1)
	{
		if (this == f1)
			return 1;
		IpEventMethod* m = dynamic_cast<IpEventMethod*>(f1);
		if ((m != NULL && ((IpEventMethod*)f1)->classObj == classObj
			&& ((IpEventMethod*)f1)->method == method))
			return 2;
		return 0;
	}
};

class IpDelegate
{
private:
	std::vector<IpEventFunc*> m_Functions;
public:
	IpDelegate()
	{

	}

	IpDelegate(IpEventFunc* func)
	{
		if (func != NULL)
			m_Functions.push_back(func);
	}

	~IpDelegate()
	{
		Free();
	}

	void Free()
	{
		while (m_Functions.size() > 0)
		{
			delete m_Functions[0];
			m_Functions.erase(m_Functions.begin());
		}
	}

	void Exec()
	{
		for (int i = 0; i < m_Functions.size(); ++i)
			m_Functions[i]->exec();
	}

	bool IsEmpty()
	{
		return (m_Functions.size() == 0);
	}

	IpDelegate& operator= (IpEventFunc* func)
	{
		Free();
		if (func != NULL)
			m_Functions.push_back(func);
		return *this;
	}

	IpDelegate& operator+= (IpEventFunc* func)
	{
		if (func != NULL)
			m_Functions.push_back(func);
		return *this;
	}

	IpDelegate& operator-= (IpEventFunc* func)
	{
		for (int i = 0; i < m_Functions.size(); ++i)
		{
			int v = m_Functions[i]->Equal(func);
			if (v)
			{
				delete func;
				if (v == 2)
					delete m_Functions[i];
				m_Functions.erase(m_Functions.begin() + i);
				break;
			}
		}
		return *this;
	}
};

#pragma endregion

#pragma region WINSTYLES

enum class ClassStyles
{
	NOCLOSE = CS_NOCLOSE, // ������ �������
	HREDRAW = CS_HREDRAW, // ���������������� ��� ��������� ������
	VREDRAW = CS_VREDRAW, // ���������������� ��� ��������� ������
	DOUBLECLICK = CS_DBLCLKS, // ������������ ������� �������� �����
	STANDART = HREDRAW | VREDRAW, // ����������� ����� ������
};

enum class WinStyles
{
	BORDER = WS_BORDER, // ���� � ������
	CHILD = WS_CHILD, // �������� ����
	HASHSCROLL = WS_HSCROLL, // ����� ������������ ������ ���������
	HASVSCROLL = WS_VSCROLL, // ����� �������������� ������ ���������
	MAXIMAZE = WS_MAXIMIZE,	// ��������� ���� ������������� �������
	MINIMAZE = WS_MINIMIZE, // ��������� ���� ������������ �������
	OVERLAPPED = WS_OVERLAPPED, // ���� ����������� ��� ��������� ���� ����������
	SIZABLE = WS_SIZEBOX,	// ����� ����������� �������� �������
	PARENT = WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // �����������, ��� �������� ���� ��������
	POPUP = WS_POPUPWINDOW,	// ����������� ����
	STANDART = WS_OVERLAPPEDWINDOW | PARENT, // ����������� ����� ����
};

enum class ListBoxStyles
{
	Standart = WS_VISIBLE | WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | LBS_WANTKEYBOARDINPUT,	// ����������� ���
	Sort = LBS_SORT,		// ��� ������ ������ ����� �������������
	AlwaysScroll = LBS_DISABLENOSCROLL, // ������ ������������ ������ ���������
	UseKeyboad = LBS_WANTKEYBOARDINPUT, // ������������� ����������
	MainStyle = WS_VISIBLE | WS_CHILD, // ������������, ���� �� ������������ Standart
};

enum class ComboBoxStyles
{
	DROPDOWN = CBS_DROPDOWN, // ������������ ����� combobox
	DROPDOWNLIST = CBS_DROPDOWNLIST, // ������������ ����� (ReadOnly)
	FULLLIST = CBS_SIMPLE,	// ������ ����� ������
	SORT = CBS_SORT,	// ���������� ���������
};

enum class LabelStyles
{
	None = 0,
	Left = SS_LEFT,				// ����� ������ �����
	Right = SS_RIGHT,			// ����� ������ ������
	Center = SS_CENTER,			// ����� ������ �� ������	
	BlackFrame = SS_BLACKFRAME,	// ������ �����
	BlackRect = SS_BLACKRECT,	// ������ ��������
};
enum class ScrollStyles
{
	Vertical = SBS_VERT,
	Horizontal = SBS_HORZ,
};

enum class OFDStyles
{
	FileMustExits = OFN_FILEMUSTEXIST, // ���� ����������� ������ ����
	ShowHiddenFiles = OFN_FORCESHOWHIDDEN, // ���������� ������� �����
	PathMustExits = OFN_PATHMUSTEXIST,	// ���� ������ ����������� ������������
	Standart = FileMustExits | PathMustExits, // ������ ������������ ���� � ���� � ����
};

enum class SFDStyles
{
	PathMustExits = OFN_PATHMUSTEXIST,	// ���� ������ ����������� ������������
	OverridePrompt = OFN_OVERWRITEPROMPT, // �������, ������������ �� ������������ ����
	CannotCreateNone = OFN_NOTESTFILECREATE, // ������, ��� �� ����� ��������� ����
	Standart = PathMustExits | OverridePrompt, // ������ ���� ���� � ������� � ����������
};
#pragma endregion

class IpApplication
{
protected:
	HINSTANCE     m_hInst;					// ���������� ����������
public:
	IpApplication();
	bool Run();								// ������

	virtual bool Init() = 0;				// �������������
	virtual bool Frame() { return true; }	// ��������� �����
	virtual bool Shutdown() = 0;			// �������� ������

	static POINT GetCursorPosition();
	static POINT GetScreenResolution();

	static LRESULT CALLBACK fnStandartProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

static class IpWindow
{
	static unsigned long m_Index;
	static unsigned long ClassName;
protected:

	HWND          m_hWnd;  // ���������� ����
	HINSTANCE     m_hInst; // ���������� ����������
	std::string m_ClassName; // ��� ������ ����
	std::string m_WinName;	// ���������

	WNDCLASSEX m_wndclass;			// ��������� ����

	DWORD m_Style;					// �����

	DWORD m_Width;					// ������
	DWORD m_Height;					// ������

	IpWindow* m_Parent;
	HMENU m_hMenu;
	bool CreateIpWindow();

	bool m_Deleted;
	bool m_Enabled;
	bool m_ClassCreated;

	COLORREF m_BackColor;
public:
	DWORD m_XPos;					// X
	DWORD m_YPos;					// Y

	std::vector<IpWindow*> m_WindowChilds;

	IpWindow(const std::string& WindowName,
		const std::string& className,
		int x,
		int y,
		int width,
		int height,
		DWORD windowStyle,
		IpWindow* Parent,
		HMENU HMenu);

	virtual ~IpWindow();

	void Free();
	bool SetName(std::string& name);
	bool Move(long Xpos, long Ypos);				// ����������� ����
	bool Resize(long Width, long Height);		// �������� ������
	void ShowMouse(bool show = true);			// ��������\������ �����
	void SetHINSTANCE(HINSTANCE hInst);
	HWND GethWnd();	// �������� ���������� ����	
	HINSTANCE GethInst();	// �������� ���������� ����������
	HMENU GetHMenu();
	IpWindow* GetParent();

	void SetStyle(DWORD style);
	void SetHandler(WNDPROC handler);
	void SetCursor(LPCSTR cursor);
	void SetIcon(LPCSTR icon);
	void SetIConsm(LPCSTR iconsm);
	void SetMenuName(LPCSTR menu);
	void SetBackColor(unsigned char R, unsigned char G, unsigned char B);

	void Show(bool visible = true);
	bool IsActive();

	DWORD GetXPos();
	DWORD GetYPos();
	DWORD GetWidht();
	DWORD GetHeight();
	COLORREF GetBackColor();
	const std::string& GetName();
	void Update();
	void Redraw();
	POINT GetCursorPosition(); // ��������� ���������� �������
};

class IpWForm : public IpWindow
{
public:
	std::vector<IpTimer*> m_Timers;

	IpDelegate Paint;
	IpDelegate MouseMove;
	IpDelegate LMouseDown;
	IpDelegate LMouseUp;
	IpDelegate RMouseDown;
	IpDelegate RMouseUp;
	IpDelegate LMouseDClick;
	IpDelegate RMouseDClick;

	/**
	@brief ����������� �����
	WindowName - ��������� ����
	x - ������� �� �����������
	y - ������� �� ���������
	width - ������ ����
	height - ������ ����
	classStyle - ����� ������ ���� (�������� ClassStyles)
	windowStyle - ����� ���� (�������� WinStyles)
	Parent - ��������� �� ������������ ���� (����� ���� NULL)
	handler - ������� ��������� ���� (�� ��������� IpApplication::fnStandartProc)
	menuName - �������� ������� ����
	**/
	IpWForm(const std::string& WindowName,
		int x = 50,
		int y = 50,
		int width = 500,
		int height = 500,
		UINT classStyle = (UINT)ClassStyles::STANDART,
		DWORD windowStyle = (DWORD)WinStyles::STANDART,
		IpWForm* Parent = NULL,
		WNDPROC handler = IpApplication::fnStandartProc,
		LPCTSTR menuName = NULL
		);
};

class IpWPanel : public IpWForm
{
public:
	IpWPanel(int x, int y, int width, int height, IpWForm* Parent);
};

static std::vector<IpWForm*> m_ActiveWindows;

class IpWOpenFileDialog
{
private:
	OPENFILENAME m_Ofn;
	char m_BuffFile[256];
	std::string m_Filters;
	std::string m_Name;
public:
	/**
	@brief �������� ����������� ���� �������� ������
	Name - ��������� ����
	Parent - ��������� �� �������� (����� ���� NULL)
	Flags - ���� �������� (�� OFDStyles)
	**/
	IpWOpenFileDialog(const std::string& Name = "",IpWindow* Parent = NULL, DWORD Flags = (DWORD)OFDStyles::Standart);
	void AddFilters(int Numfilters, const char* FilterName, const char* Filter, ...);
	bool ShowDialog();
	void SetName(const std::string& name);
	void SetFlags(DWORD Flags);
	const char* GetPath();
};

class IpWSaveFileDialog
{
private:
	OPENFILENAME m_Ofn;
	char m_BuffFile[256];
	std::string m_Filters;
	std::string m_Name;
	std::vector<std::string> m_Ext;

public:
	/**
	@brief �������� ����������� ���� ���������� ������
	Name - ��������� ����
	Parent - ��������� �� �������� (����� ���� NULL)
	Flags - ���� �������� (�� SFDStyles)
	**/
	IpWSaveFileDialog(const std::string& Name = "", IpWindow* Parent = NULL, DWORD Flags = (DWORD)SFDStyles::Standart);
	~IpWSaveFileDialog();
	void AddFilters(int Numfilters, const char* FilterName, const char* Filter, ...);
	void SetName(const std::string& name);
	void SetFlags(DWORD Flags);
	bool ShowDialog();
	std::string GetPath();
};

class IpWCheckBox : public IpWindow
{
private:
	bool m_Checked;
public:
	IpDelegate ClickedEvent;

	IpWCheckBox(const std::string& CheckBoxName,
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent);

	void SetState(bool checked);
	bool Checked();
};

class IpWRadioButton : public IpWindow
{
private:
	bool m_Checked;
public:
	IpDelegate ClickedEvent;
	int ButtonGroup;

	IpWRadioButton(const std::string& RadioButtonName,
		int group,
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent);

	void SetState(bool checked);
	bool Checked();
};

class IpWButton : public IpWindow
{
public:
	IpDelegate ClickedEvent;

	IpWButton(const std::string& ButtonName,
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent);
};

class IpWListBox : public IpWindow
{
public:
	IpDelegate DoubleClickedEvent;
	IpDelegate ChangeChoiseEvent;
	IpDelegate SetFocusEvent;
	IpDelegate KillFocusEvent;

	/**
	@brief - ����������� ������ IpWListBox
	x - x-���������� ������ �������� ����
	y - y-���������� ������ �������� ����
	width - ������
	height - �������
	parent - ��������� �� ���� ��������
	Flags - ����� �������� (�������� �� ListBoxStyles)
	**/
	IpWListBox(
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent,
		DWORD Flags = (DWORD)ListBoxStyles::Standart);

	int NumItems();
	bool AddString(const std::string& string);
	bool DeleteItem(int index);
	// ������ (-1)
	int GetString(int index, char* buff);
	// ������ (-1)
	int FindStringPrefix(int StartIndex, char* prefix);
	// ������ (-1)
	int FindString(int StartIndex, char* string);
	int GetSelectedIndex();
	void SetSelectedIndex(int index);
	void Clear();
};

class IpWComboBox : public IpWindow
{
public:
	IpDelegate CloseUpEvent;
	IpDelegate DoubleClickedEvent;
	IpDelegate ChangedTextEvent;
	IpDelegate SetFocusEvent;
	IpDelegate KillFocusEvent;
	IpDelegate ChangeChoiseEvent;

	/**
	@brief - ����������� ������ IpWComboBox
	x - x-���������� ������ �������� ����
	y - y-���������� ������ �������� ����
	width - ������
	height - �������
	parent - ��������� �� ���� ��������
	Flags - ����� �������� (�������� �� ComboBoxStyles)
	**/
	IpWComboBox(
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent,
		DWORD Flags = (DWORD)ComboBoxStyles::DROPDOWNLIST);

	int NumItems();
	bool AddString(const std::string& string);
	bool DeleteItem(int index);
	// ������ (-1)
	int GetString(int index, char* buff);
	// ������ (-1)
	int FindStringPrefix(int StartIndex, char* prefix);
	// ������ (-1)
	int FindString(int StartIndex, char* string);
	int GetSelectedIndex();
	void SetSelectedIndex(int index);
	void Clear();
	
	bool IsExpandedState();
	bool IsDroppedState();
	void SetDropDown(bool state);
	void SetExpandedState(bool state);

};

class IpWTextBox : public IpWindow
{
private:
	bool m_ReadOnly;
public:
	IpDelegate ChangedEvent;

	IpWTextBox(
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent);

	void Clear();
	int GetLine(char* buff);
	void ReadOnly(bool value = true);
	int GetLenght();
};

class IpWRichTextBox : public IpWindow
{
private:
	bool m_ReadOnly;
public:
	IpDelegate ChangedEvent;

	IpWRichTextBox(
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent);

	void Clear();
	int GetNumLines();
	int GetLine(int line,char* buff);
	void ReadOnly(bool value = true);
	int GetLenght(int line = -1);
	bool SetText(char* buff);
	bool AddText(char* buff,bool newLine = false);
};

class IpWLabel : public IpWindow
{
public:
	/**
	@brief - ����������� ������ IpWLabel
	x - x-���������� ������ �������� ����
	y - y-���������� ������ �������� ����
	width - ������
	height - �������
	parent - ��������� �� ���� ��������
	Flags - ����� �������� (�������� �� LabelStyles ��� 0)
	**/
	IpWLabel(const std::string& LabelName,
		int x,
		int y,
		int width,
		int height,
		IpWForm* Parent,
		DWORD Style = 0);
};

class IpWScroll : public IpWindow
{
private:
	int m_Min;
	int m_Max;
	bool m_Show;
	bool m_Enable;
	int m_SStep;
	int m_BStep;
public:
	IpDelegate Scroll;
	IpDelegate EndScroll;

	IpWScroll(int x,int y,int width,int height, ScrollStyles Styleflag,IpWForm* m_Parent);
	void SetRangeValues(int min, int max);
	void SetStepsValues(int smallStep, int bigStep);
	int GetSmallStep();
	int GetBigStep();
	int GetMinValue();
	int GetMaxValue();
	bool SetValue(int value);
	int GetCurrentValue();
	void Show(bool show);
	void Enable(bool enable);
	bool IsActive();
	bool IsEnabled();
};

class IpTimer
{
private:
	static int TimerIndex;

	IpWForm* m_Form;
	unsigned long m_Interval;
	int m_Index;
	bool m_Started;
public:
	IpDelegate Tick;

	IpTimer(unsigned long Interval, IpWForm* Form);
	void Start();
	void Stop();
	int GetIndex();
	bool IsStarted();
};

#endif