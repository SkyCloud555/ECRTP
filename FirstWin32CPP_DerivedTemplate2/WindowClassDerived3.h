#pragma once

//Copyright (C) 2020 Andrey Khishchenko

//This file is part of Extended CRTP research program.
//
//Extended CRTP research program is free software : you can redistribute
//it and /or modify it under the terms of the GNU Lesser General Public License
//as published by the Free Software Foundation, either version 3 of the License,
//or any later version.
//
//Extended CRTP research program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
//more details.
//
//You should have received a copy of the GNU Lesser General Public License
//along with Extended CRTP research program. If not, see < https://www.gnu.org/licenses/>.

//включаемые файлы
#include "WindowClass.h"

//третий вариант шаблонного наследования

//классы инициализации, переопределяющие OnCreate

class WindowClassInit1
{
public:
	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(200, 160, 255)));
	}
};

class WindowClassInit2
{
public:
	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 255, 200)));
	}
};

class WindowClassInit3
{
public:
	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(210, 250, 207)));
	}
};

class WindowClassInit4
{
public:
	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(40, 244, 255)));
	}
};

class WindowClassInit2Alt
{
public:
	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(253, 189, 55)));
	}
};

//класс, реализующий переопределение виртуальных функций для обработки сообщений

template<class WndClsInit = WindowClassInit1> class WindowClassDerivedI1 : public WindowClassTemplate<WndClsInit>
{
protected:
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedI1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedI1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassDerivedI1(WindowClassDerivedI1& wcObj) : WindowClassDerivedI1(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassDerivedI1() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с классом инициализации (ПЕРВОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
	virtual void OnDestroy(HWND hWnd) override				//обеспечивает обработку WM_DESTROY внутри оконной процедуры
	{
		//обеспечивает обработку WM_DESTROY внутри оконной процедуры	
		usiWndNum--;		//уменьшаем количество объектов данного класса
		if (usiWndNum == 0)
			PostQuitMessage(0);
	}
};

template<class WndClsInit = WindowClassInit2> class WindowClassDerivedI2 : public WindowClassDerivedI1<WndClsInit>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedI2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedI2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с классом инициализации (ВТОРОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit3> class WindowClassDerivedI3 : public WindowClassDerivedI2<WndClsInit>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedI3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI2(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedI3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI2(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с классом инициализации (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit4> class WindowClassDerivedI4 : public WindowClassDerivedI3<WndClsInit>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedI4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI3(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedI4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI3(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с классом инициализации (ЧЕТВЁРТОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit2Alt> class WindowClassDerivedI2Alt : public WindowClassDerivedI1<WndClsInit>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedI2Alt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedI2Alt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с классом инициализации (ВТОРОЕ наследование - ВТОРАЯ ВЕТВЬ)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

//реализация шаблонных классов многократного наследования

template<class WndClsInit> unsigned short int WindowClassDerivedI1<WndClsInit>::usiWndNum = 0;		//количество объектов переопределённого класса