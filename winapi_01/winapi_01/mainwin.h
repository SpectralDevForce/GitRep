#pragma once
#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H


#include <Windows.h>
#include <iostream>
#include <string>
#include <exception> 
#include <map>

#include "IObserver.h"

namespace spectral
{

class MainWin : public IObservable
{
	enum class CTL_ID
	{
		STATIC1,
		STATIC2,
		STATIC3,
		BUTTON1,
		EDIT1,
		EDIT2,
		EDIT3,
		THREAD1,
		THREAD2
	};
private:
	vector<IObserver*> observers;

public:
	MainWin();
	~MainWin();

public:
	void Run();

public:
	void addObserver(IObserver* o) override;
	void removeObserver(IObserver* o) override;
	void notify(pair<string, int> p) override;
	void notify(map<string, int> p) override;
	void changeCountThread(int p);
	void changeCountThread(int count,int max,int min,int step);

private:
	void init_native_window_obj();

	//статическая процедура передает указать на объект класса в динамическую процедуру
	static LRESULT CALLBACK static_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK dinamic_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void create_native_controls();

private:
	const std::wstring m_szClassName{ L"ClassName" }, m_szMenuName{ L"WinName" };
	HWND m_hWnd{}, m_hWndButton{}, m_hWndEdit1{}, m_hWndEdit2{}, m_hWndEdit3{};
	HWND m_hWndStatic1{}, m_hWndStatic2{}, m_hWndStatic3{}, m_hWndStaticStep{};
	HWND m_hWndThread1{}, m_hWndThread2{};
	const int m_WinWidth{ 720 }, m_WinHeight{ 350 };
	const int m_XWin{ 400 }, m_YWin{ 200 };
};

}

#endif