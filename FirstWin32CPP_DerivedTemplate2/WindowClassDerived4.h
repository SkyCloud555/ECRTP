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

//логика очень проста: перед передачей параметра в базовый класс проверяется, совпадает ли переданный параметр с параметром по умолчанию
//если совпадает, то передаём себя же; если нет, то передаём тот класс, который получили

template<class DerWndCls = thisclass> class WindowClassDerivedAlternative1 : public WindowClassTemplate<std::conditional_t<std::is_same<DerWndCls, thisclass>::value, WindowClassDerivedAlternative1<>, DerWndCls>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedAlternative1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedAlternative1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(wc, szWndTitle) {}

	virtual ~WindowClassDerivedAlternative1() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с условной передачей параметра (ПЕРВОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = thisclass> class WindowClassDerivedAlternative2 : public WindowClassDerivedAlternative1<std::conditional_t<std::is_same<DerWndCls, thisclass>::value, WindowClassDerivedAlternative2<>, DerWndCls>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedAlternative2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative1(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedAlternative2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative1(wc, szWndTitle) {}

	virtual ~WindowClassDerivedAlternative2() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с условной передачей параметра (ВТОРОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = thisclass> class WindowClassDerivedAlternative3 : public WindowClassDerivedAlternative2<std::conditional_t<std::is_same<DerWndCls, thisclass>::value, WindowClassDerivedAlternative3<>, DerWndCls>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedAlternative3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative2(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedAlternative3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative2(wc, szWndTitle) {}

	virtual ~WindowClassDerivedAlternative3() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с условной передачей параметра (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = thisclass> class WindowClassDerivedAlternative4 : public WindowClassDerivedAlternative3<std::conditional_t<std::is_same<DerWndCls, thisclass>::value, WindowClassDerivedAlternative4<>, DerWndCls>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedAlternative4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative3(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedAlternative4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedAlternative3(wc, szWndTitle) {}

	virtual ~WindowClassDerivedAlternative4() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс с условной передачей параметра (ЧЕТВЁРТОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};