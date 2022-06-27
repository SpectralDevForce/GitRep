

#include "mainwin.h"

namespace spectral
{

#define TIME_FRAME 100
long int time_frame;

MainWin::MainWin()
{
	using std::wstring;
	using std::string;

	try
	{
		this->init_native_window_obj();
		this->create_native_controls();
	}
	catch (const std::exception& e)
	{
		string expt_data = e.what();

		MessageBox(nullptr, wstring(begin(expt_data), end(expt_data)).c_str(), L"Error constructor", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

MainWin::~MainWin()
{

}

void MainWin::Run()
{
	MSG _msg{};
	ShowWindow(this->m_hWnd, SW_SHOWNORMAL); //SW_SHOWDEFAULT
	UpdateWindow(this->m_hWnd);

	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
		Sleep(1);
	}
	//return static_cast<int>(_msg.wParam);
}

//////////////////////////////////////////////////
void MainWin::addObserver(IObserver* o)
{
	observers.push_back(o);
}
void MainWin::removeObserver(IObserver* o)
{
	vector<IObserver*>::iterator itErase = observers.begin();
	for (itErase; itErase != observers.end(); itErase++)
	{
		if (*itErase == o) { observers.erase(itErase); break; }
	}
}
void MainWin::notify(pair<string, int> p)
{
	if (p.first == "CountThread")
	{
		for (auto o : observers) { o->updateCountThread(p.second); }
	}
}
void MainWin::notify(map<string, int> p)
{
	for (auto o : observers) { o->updateCountThread(p); }
}
void MainWin::changeCountThread(int p)
{
	notify(pair<string, int>("CountThread", p));
}
void MainWin::changeCountThread(int count, int max, int min, int step)
{ 
	map<string,int> threadMap;
	threadMap.emplace("CountThread", count);
	threadMap.emplace("max", max);
	threadMap.emplace("min", min);
	threadMap.emplace("step", step);
	notify(threadMap);
}

//////////////////////////////////////////////
void MainWin::init_native_window_obj()
{
	using std::runtime_error;
	using namespace std::string_literals;

	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) };
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	_wc.hInstance = GetModuleHandle(nullptr);
	_wc.lpfnWndProc = MainWin::static_proc;
	_wc.lpszClassName = this->m_szClassName.c_str();
	_wc.lpszMenuName = this->m_szMenuName.c_str();
	_wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&_wc))
		throw runtime_error("Error,can't register main window class!");

	RECT _windowRC{ 0,0,this->m_WinWidth,this->m_WinHeight };
	AdjustWindowRect(&_windowRC, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, false); // WS_OVERLAPPEDWINDOW

	if (this->m_hWnd = CreateWindowEx(
		0,
		_wc.lpszClassName,
		_wc.lpszMenuName,
		WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		_windowRC.left+m_XWin, _windowRC.top+m_YWin, _windowRC.right, _windowRC.bottom,
		nullptr, nullptr, nullptr, this);
	!this->m_hWnd)
		throw std::runtime_error("Error, can't create main window!"s);
}


LRESULT MainWin::static_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MainWin* winMain;
	//сообщение неклиентской области до создания клиентной области
	if (uMsg == WM_NCCREATE)
	{
		//получает указатель от CreateWindowEx(...this) на окна из указателя на lparam()
		winMain = static_cast<MainWin*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winMain)))
		{
			if (GetLastError() != 0)
				return false;
		}
	}
	else
	{
		//получает указатель из структуры окна
		winMain = reinterpret_cast<MainWin*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (winMain)
	{
		//получает дискриптор окна из структуры окна
		winMain->m_hWnd = hwnd;
		//и возвращает динамическую процедуру окна
		return winMain->dinamic_proc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT MainWin::dinamic_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
	case WM_PAINT:
	{

	}
	return 0;

	case WM_SIZE:
	{

	}
	return 0;

	case WM_KEYDOWN:
	{
		std::cout << "code = " << wparam << std::endl;
		if (wparam == VK_F1)
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		if (wparam == VK_F2)
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}

		//деактивирует рамку окна
		if (wparam == VK_F3)
		{
			SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN);
		}
		//активиурет рамку окна
		if (wparam == VK_F4)
		{
			SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN);
		}

		//закрывает окно на escape
		if (wparam == VK_ESCAPE) { PostMessage(hwnd, WM_QUIT, 0, 0); }
	}
	return 0;

	case WM_MOUSEMOVE:
	{
		int xPos = LOWORD(lparam);
		int yPos = HIWORD(lparam);
		//std::cout << "mose(x,y): " << xPos << " " << yPos << std::endl;
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		std::cout << "MLBD_LKM" << std::endl;
	}
	return 0;

	case WM_COMMAND:
	{
		switch (static_cast<MainWin::CTL_ID>(LOWORD(wparam)))
		{
		case MainWin::CTL_ID::BUTTON1:
		{
			std::wstring text1{};
			std::wstring text2{};
			std::wstring text3{};
			std::wstring textRes{};
			std::wstring textThread{};
			text1.resize(MAX_PATH);
			text2.resize(MAX_PATH);
			text3.resize(MAX_PATH);
			GetWindowText(this->m_hWndEdit1, &text1[0], MAX_PATH);
			GetWindowText(this->m_hWndEdit2, &text2[0], MAX_PATH);
			GetWindowText(this->m_hWndEdit3, &text3[0], MAX_PATH);
			//подгоняет размер буфера под размер текста
			text1.erase(remove(begin(text1), end(text1), 0), end(text1));
			text2.erase(remove(begin(text2), end(text2), 0), end(text2));
			text3.erase(remove(begin(text3), end(text3), 0), end(text3));

			int max = std::stoi(text1);
			int min = std::stoi(text2);
			int step = std::stoi(text3);
			if (step > 0)
			{
				int res = (max - min) / step;

				int countThread = res;
				if (res > 8) {countThread = 8;}

				textRes = std::to_wstring(res);
				textThread = std::to_wstring(countThread);
				SetWindowText(this->m_hWndStaticStep, textRes.c_str());
				SetWindowText(this->m_hWndThread2, textThread.c_str());

				changeCountThread(countThread, max, min, step);
			}
		}
		}

	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	return 0;

	case WM_DESTROY:
	{
		//PostQuitMessage(EXIT_SUCCESS);
		PostQuitMessage(0);
	}
	return 0;
	}

	return DefWindowProcA(hwnd, message, wparam, lparam);
}


void MainWin::create_native_controls()
{
	using std::runtime_error;
	using namespace std::string_literals;

	if (this->m_hWndButton = CreateWindowEx(
		0,
		L"BUTTON",
		L"Запустить тест",
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		300, 250, 150, 37, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::BUTTON1), nullptr, nullptr);
		!this->m_hWndButton)
		throw runtime_error("Error, can't create button!"s);

	///////////////////////////////////////////////////
	if (this->m_hWndEdit1 = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"0",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		300, 50, 150, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::EDIT1), nullptr, nullptr);
		!this->m_hWndEdit1)
		throw runtime_error("Error, can't create edit1!"s);

	if (this->m_hWndEdit2 = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"0",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		300, 100, 150, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::EDIT2), nullptr, nullptr);
		!this->m_hWndEdit2)
		throw runtime_error("Error, can't create edit2!"s);

	if (this->m_hWndEdit3 = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"0",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		300, 150, 150, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::EDIT3), nullptr, nullptr);
		!this->m_hWndEdit3)
		throw runtime_error("Error, can't create edit3!"s);

	////////////////////////////////////////////////
	if (this->m_hWndStatic1 = CreateWindowEx(
		0,
		L"STATIC",
		L"Введите максимальное значение",
		WS_CHILD | WS_VISIBLE,
		10, 50, 250, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC1), nullptr, nullptr);
		!this->m_hWndStatic1)
		throw runtime_error("Error, can't create STATIC1!"s);
	if (this->m_hWndStatic2 = CreateWindowEx(
		0,
		L"STATIC",
		L"Введите минимальное значение",
		WS_CHILD | WS_VISIBLE,
		10, 100, 250, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC2), nullptr, nullptr);
		!this->m_hWndStatic2)
		throw runtime_error("Error, can't create STATIC2!"s);
	if (this->m_hWndStatic3 = CreateWindowEx(
		0,
		L"STATIC",
		L"Введите шаг",
		WS_CHILD | WS_VISIBLE,
		10, 150, 250, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC3), nullptr, nullptr);
		!this->m_hWndStatic3)
		throw runtime_error("Error, can't create STATIC3!"s);
	if (this->m_hWndStaticStep = CreateWindowEx(
		0,
		L"STATIC",
		L"0",
		WS_CHILD | WS_VISIBLE,
		10, 200, 250, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC3), nullptr, nullptr);
		!this->m_hWndStaticStep)
		throw runtime_error("Error, can't create STATICSTEP!"s);
	/////////////////////////////////////////////
	if (this->m_hWndThread1 = CreateWindowEx(
		0,
		L"STATIC",
		L"Количество выделенных\nпотоков",
		WS_CHILD | WS_VISIBLE,
		500, 50, 200, 40, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC3), nullptr, nullptr);
		!this->m_hWndThread1)
		throw runtime_error("Error, can't create THREAD1!"s);
	if (this->m_hWndThread2 = CreateWindowEx(
		0,
		L"STATIC",
		L"0",
		WS_CHILD | WS_VISIBLE,
		500, 90, 200, 24, this->m_hWnd, reinterpret_cast<HMENU>(MainWin::CTL_ID::STATIC3), nullptr, nullptr);
		!this->m_hWndThread2)
		throw runtime_error("Error, can't create THREAD2!"s);


	HFONT hFont = CreateFont(18, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto");
	SendMessage(this->m_hWndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndEdit1, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndEdit2, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndEdit3, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndStatic1, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndStatic2, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndStatic3, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndStaticStep, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndThread1, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hWndThread2, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	if (m_hWndButton != NULL || m_hWndEdit1 != NULL || m_hWndEdit2 != NULL || m_hWndEdit3 != NULL)
	{
		SendMessage(this->m_hWnd, WM_CHANGEUISTATE, (WPARAM)(0x10001), (LPARAM)(0));
	}
}

}