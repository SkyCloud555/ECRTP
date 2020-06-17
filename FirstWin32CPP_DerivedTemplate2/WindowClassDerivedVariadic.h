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

//вариативные шаблонные классы многократного наследования (тестирование)

//включаемые файлы
#include "WindowClass.h"

//классы

//в данном случае каждый новый класс добавляется в КОНЕЦ списка параметров шаблона; это значит, что в WindowClassVariadicTemplate класс в вершине
//иерархии будет самым первым в списке
//поскольку класс объявлен как шаблонный, например, template<class... PrevWndClasses> class WindowClassVariadic1, то и в базовый класс текущий класс
//должен передаваться со списком параметров, причём с пустым (мы же не имеем никакого класса под рукой, чем можно было бы параметризовать)

//строго именно в такой форме и нужно объявлять все классы, если от них предполагается наследование с переопределением OnCreate

//если же текущий класс добавлять по-другому, перед списком, то есть, например:
//template<class... PrevWndClasses> class WindowClassVariadic1 : public WindowClassVariadicTemplate<WindowClassVariadic1<>, PrevWndClasses...>
//то тогда нужный класс окажется в итоге в конце списка, и его оттуда придётся извлекать довольно сложным способом через меташаблон GetTypeFromParamPack

template<class... PrevWndClasses> class WindowClassVariadic1 : public WindowClassVariadicTemplate<PrevWndClasses..., WindowClassVariadic1<>>
{
protected:
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadic1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadicTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadic1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadicTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassVariadic1(WindowClassVariadic1& wcObj) : WindowClassVariadicTemplate(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassVariadic1() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
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
		DrawText(hDC, TEXT("Шаблонный ВАРИАТИВНЫЙ переопределённый класс (ПЕРВОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic2 : public WindowClassVariadic1<PrevWndClasses..., WindowClassVariadic2<>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadic2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadic2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassVariadic2(WindowClassVariadic2& wcObj) : WindowClassVariadic1(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassVariadic2() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
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
		DrawText(hDC, TEXT("Шаблонный ВАРИАТИВНЫЙ переопределённый класс (ВТОРОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic3 : public WindowClassVariadic2<PrevWndClasses..., WindowClassVariadic3<>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadic3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic2(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadic3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic2(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassVariadic3(WindowClassVariadic3& wcObj) : WindowClassVariadic2(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassVariadic3() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
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
		DrawText(hDC, TEXT("Шаблонный ВАРИАТИВНЫЙ переопределённый класс (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic4 : public WindowClassVariadic3<PrevWndClasses..., WindowClassVariadic4<>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadic4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic3(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadic4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic3(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassVariadic4(WindowClassVariadic4& wcObj) : WindowClassVariadic3(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassVariadic4() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
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
		DrawText(hDC, TEXT("Шаблонный ВАРИАТИВНЫЙ переопределённый класс (ЧЕТВЁРТОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic2Alt : public WindowClassVariadic1<PrevWndClasses..., WindowClassVariadic2Alt<>>
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadic2Alt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadic2Alt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WindowClassVariadic2Alt(WindowClassVariadic2Alt& wcObj) : WindowClassVariadic1(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WindowClassVariadic2Alt() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(253, 189, 55)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный ВАРИАТИВНЫЙ переопределённый класс (ВТОРОЕ наследование - ВТОРАЯ ВЕТВЬ)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//реализация шаблонных классов многократного наследования

template<class... PrevWndClasses> unsigned short int WindowClassVariadic1<PrevWndClasses...>::usiWndNum = 0;		//количество объектов переопределённого класса
