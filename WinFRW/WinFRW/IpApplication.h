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
	NOCLOSE = CS_NOCLOSE, // Нельзя закрыть
	HREDRAW = CS_HREDRAW, // Перерисовывается при изменении ширины
	VREDRAW = CS_VREDRAW, // Перерисовывается при изменении высоты
	DOUBLECLICK = CS_DBLCLKS, // Обрабатывает событие двойного клика
	STANDART = HREDRAW | VREDRAW, // Стандартный стиль класса
};

enum class WinStyles
{
	BORDER = WS_BORDER, // Окно с рамкой
	CHILD = WS_CHILD, // Дочернее окно
	HASHSCROLL = WS_HSCROLL, // Имеет вертикальную полосу просмотра
	HASVSCROLL = WS_VSCROLL, // Имеет горизонтальную полосу просмотра
	MAXIMAZE = WS_MAXIMIZE,	// Создается окно максимального размера
	MINIMAZE = WS_MINIMIZE, // Создается окно минимального размера
	OVERLAPPED = WS_OVERLAPPED, // Окно перекрывает все остальные окна приложения
	SIZABLE = WS_SIZEBOX,	// Имеет возможность изменять размеры
	PARENT = WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // Указывается, при создании окна родителя
	POPUP = WS_POPUPWINDOW,	// Всплывающее окно
	STANDART = WS_OVERLAPPEDWINDOW | PARENT, // Стандартный стиль окна
};

enum class ListBoxStyles
{
	Standart = WS_VISIBLE | WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | LBS_WANTKEYBOARDINPUT,	// Стандартный тип
	Sort = LBS_SORT,		// Все строки списка будут отсортированы
	AlwaysScroll = LBS_DISABLENOSCROLL, // Всегда отображается полоса прокрутки
	UseKeyboad = LBS_WANTKEYBOARDINPUT, // Использовании клавиатуры
	MainStyle = WS_VISIBLE | WS_CHILD, // Использовать, если не используется Standart
};

enum class ComboBoxStyles
{
	DROPDOWN = CBS_DROPDOWN, // Классический стиль combobox
	DROPDOWNLIST = CBS_DROPDOWNLIST, // Классический стиль (ReadOnly)
	FULLLIST = CBS_SIMPLE,	// Список виден всегда
	SORT = CBS_SORT,	// Сортировка элементов
};

enum class LabelStyles
{
	None = 0,
	Left = SS_LEFT,				// Вывод текста слева
	Right = SS_RIGHT,			// Вывод текста справа
	Center = SS_CENTER,			// Вывод текста по центру	
	BlackFrame = SS_BLACKFRAME,	// Черная рамка
	BlackRect = SS_BLACKRECT,	// Черная закраска
};
enum class ScrollStyles
{
	Vertical = SBS_VERT,
	Horizontal = SBS_HORZ,
};

enum class OFDStyles
{
	FileMustExits = OFN_FILEMUSTEXIST, // Файл обязательно должен быть
	ShowHiddenFiles = OFN_FORCESHOWHIDDEN, // Показывать скрытые файлы
	PathMustExits = OFN_PATHMUSTEXIST,	// Путь должен обязательно существовать
	Standart = FileMustExits | PathMustExits, // Должен существовать файл и путь к нему
};

enum class SFDStyles
{
	PathMustExits = OFN_PATHMUSTEXIST,	// Путь должен обязательно существовать
	OverridePrompt = OFN_OVERWRITEPROMPT, // Спросит, перезаписать ли существующий файл
	CannotCreateNone = OFN_NOTESTFILECREATE, // Скажет, что не может сохранить файл
	Standart = PathMustExits | OverridePrompt, // Должен быть путь и спросит о перезаписи
};
#pragma endregion

class IpApplication
{
protected:
	HINSTANCE     m_hInst;					// Дескриптор приложения
public:
	IpApplication();
	bool Run();								// Запуск

	virtual bool Init() = 0;				// Инициализация
	virtual bool Frame() { return true; }	// Рисование кадра
	virtual bool Shutdown() = 0;			// Отчистка памяти

	static POINT GetCursorPosition();
	static POINT GetScreenResolution();

	static LRESULT CALLBACK fnStandartProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

static class IpWindow
{
	static unsigned long m_Index;
	static unsigned long ClassName;
protected:

	HWND          m_hWnd;  // Дескриптор окна
	HINSTANCE     m_hInst; // Дескриптор экземпляра
	std::string m_ClassName; // Имя класса окна
	std::string m_WinName;	// Заголовок

	WNDCLASSEX m_wndclass;			// Структура окна

	DWORD m_Style;					// Стиль

	DWORD m_Width;					// Ширина
	DWORD m_Height;					// Высота

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
	bool Move(long Xpos, long Ypos);				// Переместить окно
	bool Resize(long Width, long Height);		// Изменить размер
	void ShowMouse(bool show = true);			// Показать\скрыть мышку
	void SetHINSTANCE(HINSTANCE hInst);
	HWND GethWnd();	// Получить дескриптор окна	
	HINSTANCE GethInst();	// Получить дескриптор экземпляра
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
	POINT GetCursorPosition(); // Локальные координаты курсора
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
	@brief Конструктор формы
	WindowName - заголовок окна
	x - позиция по горизонтали
	y - позиция по вертикали
	width - ширина окна
	height - высота окна
	classStyle - стиль класса окна (значение ClassStyles)
	windowStyle - стиль окна (значение WinStyles)
	Parent - указатель на родительское окно (может быть NULL)
	handler - функция обработки окна (по умолчанию IpApplication::fnStandartProc)
	menuName - название ресурса меню
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
	@brief создание диалогового окна загрузки файлов
	Name - заголовок окна
	Parent - указатель на родителя (может быть NULL)
	Flags - флаг создания (из OFDStyles)
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
	@brief создание диалогового окна сохранения файлов
	Name - заголовок окна
	Parent - указатель на родителя (может быть NULL)
	Flags - флаг создания (из SFDStyles)
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
	@brief - конструктор класса IpWListBox
	x - x-координата левого верхнего угла
	y - y-координата левого верхнего угла
	width - ширина
	height - вытсота
	parent - указатель на окно родителя
	Flags - флаги создания (значение из ListBoxStyles)
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
	// Ошибка (-1)
	int GetString(int index, char* buff);
	// Ошибка (-1)
	int FindStringPrefix(int StartIndex, char* prefix);
	// Ошибка (-1)
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
	@brief - конструктор класса IpWComboBox
	x - x-координата левого верхнего угла
	y - y-координата левого верхнего угла
	width - ширина
	height - вытсота
	parent - указатель на окно родителя
	Flags - флаги создания (значение из ComboBoxStyles)
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
	// Ошибка (-1)
	int GetString(int index, char* buff);
	// Ошибка (-1)
	int FindStringPrefix(int StartIndex, char* prefix);
	// Ошибка (-1)
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
	@brief - конструктор класса IpWLabel
	x - x-координата левого верхнего угла
	y - y-координата левого верхнего угла
	width - ширина
	height - вытсота
	parent - указатель на окно родителя
	Flags - флаги создания (значение из LabelStyles или 0)
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