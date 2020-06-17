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

//четвёртый вариант шаблонного наследования
//этот вариант повторяет вариант из WindowClassDerived4.h, но без вспомогательного типа thistype и условного выбора с помощью std::is_same

//и он не прокатит: компилятор пишет, что рекурсивная зависимость типа или функции слишком сложна

template<class DerWndCls = WindowClassDerivedSimplest1<>> class WindowClassDerivedSimplest1 : public WindowClassTemplate<DerWndCls>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedSimplest1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedSimplest1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest1() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override			//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(200, 160, 255)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный ПРОСТЕЙШИЙ переопределённый класс с условной передачей параметра (ПЕРВОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest2<>> class WindowClassDerivedSimplest2 : public WindowClassDerivedSimplest1<DerWndCls>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedSimplest2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest1(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedSimplest2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest1(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest2() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override			//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 255, 200)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный ПРОСТЕЙШИЙ переопределённый класс с условной передачей параметра (ВТОРОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest3<>> class WindowClassDerivedSimplest3 : public WindowClassDerivedSimplest2<DerWndCls>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedSimplest3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest2(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedSimplest3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest2(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest3() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override			//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(210, 250, 207)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный ПРОСТЕЙШИЙ переопределённый класс с условной передачей параметра (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest4<>> class WindowClassDerivedSimplest4 : public WindowClassDerivedSimplest3<DerWndCls>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedSimplest4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest3(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedSimplest4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest3(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest4() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override			//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(40, 244, 255)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный ПРОСТЕЙШИЙ переопределённый класс с условной передачей параметра (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};