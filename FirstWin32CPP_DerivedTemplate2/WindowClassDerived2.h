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

//ещё пару производных классов для тестирования

//включаемые файлы
#include "WindowClassDerived.h"

//классы

class WindowClassDerivedNext : public WindowClassDerived		//строим новый класс на основе предыдущего производного
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) override;				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd) override;				//обеспечивает обработку WM_PAINT внутри оконной процедуры
};

//ПРИ ТАКОЙ форме шаблонного наследования, к сожалению, последующие классы влияют на все предыдущие: всё, что нужно, работает только для последнего, находящегося
//в вершине иерархии класса, для ниже же по иерархии наступают сюрпризы; причём все виртуальные OnPaint, OnClose и т.д. переопределяются без проблем: вопрос
//касается OnCreate, ради которой и была заварена эта шаблонная каша
//только самый последний класс (в вершине иерархии) в силу правильно переданных параметров (наличие-отсутствие ControlType) работает как надо; во всех же
//нижележащищих классах вызывается OnCreate самого первого (базового) класса после базового шаблона WindowClassControlBaseTemplate - в нашем случае это
//WndClsDerivedTemplateClass: причина этого понятна, если проследить всю цепочку обращений и передач параметров от класса к классу
//в силу этого OnCreate вызывается не того класса, для которого мы создавали объект, потому что этот класс описан с передачей ControlType = thisclass, а по
//правилу ниже для класс того объекта, который мы создаём, не должен передавать ControlType ближайшему базовому классу

//ПРАВИЛО ТАКОЕ: для ПОСЛЕДНЕГО в вершине иерархии класса при наследовании параметр ControlType нужно опускать, чтобы нижележащим классам не передавался
//thisclass; нижележашие классы по иерархии должны передавать ControlType, чтобы впоследствии передать вершинный класс в WindowClassBase

//при таком принципе подразумевается, что объявления всех классов доступны, и при добавлении нового класса всегда можно поменять параметры шаблоны 
//последнего предыдущего

//и замудрёное ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>, и наследование в виде:
//текущий_класс<DerWndCls>, ControlType - это часть архитектуры данного шаблонного проектирования, и ему необходимо следовать при определении производных
//классов

template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WndClsDerivedTemplateClass : public WindowClassControlBaseTemplate<WndClsDerivedTemplateClass<DerWndCls>, ControlType>		//строим новый класс на основе предыдущего производного
{
protected:
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	//конструктор для инициализации класса по умолчанию
	WndClsDerivedTemplateClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WndClsDerivedTemplateClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);
	WndClsDerivedTemplateClass(WndClsDerivedTemplateClass&);			//конструктор копирования

	virtual ~WndClsDerivedTemplateClass() override;		//виртуальный деструктор

	virtual void OnCreate(HWND hWnd) override;				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd) override;				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd) override;				//обеспечивает обработку WM_DESTROY внутри оконной процедуры
};

//второй шаблонный класс на основе вышенаписанного
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNext : public WndClsDerivedTemplateClass<WindowClassDerivedTemplateNext<DerWndCls>, ControlType>		//строим новый класс на основе предыдущего производного
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedTemplateNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(hInstance, szClassName, szWndTitle) { usiWndNum++; }
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedTemplateNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(wc, szWndTitle) { usiWndNum++; }

	virtual ~WindowClassDerivedTemplateNext() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс (ПОВТОРНОЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//третий шаблонный класс на основе второго

template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNext2 : public WindowClassDerivedTemplateNext<WindowClassDerivedTemplateNext2<DerWndCls>>		//строим новый класс на основе предыдущего производного
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedTemplateNext2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedTemplateNext(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedTemplateNext2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedTemplateNext(wc, szWndTitle) {}
	virtual ~WindowClassDerivedTemplateNext2() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс (ТРЕТЬЕ наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//второй шаблонный класс, но без параметра ControlType
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNextAlt : public WndClsDerivedTemplateClass<WindowClassDerivedTemplateNextAlt<DerWndCls>>		//строим новый класс на основе предыдущего производного
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedTemplateNextAlt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(hInstance, szClassName, szWndTitle) { }
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedTemplateNextAlt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(wc, szWndTitle) {}
	virtual ~WindowClassDerivedTemplateNextAlt() override		//виртуальный деструктор
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
		DrawText(hDC, TEXT("Шаблонный переопределённый класс (ПОВТОРНОЕ наследование - АЛЬТЕРНАТИВНАЯ ВЕТВЬ)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//первый шаблонный класс - третья ветвь
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WndClsDerivedTemplateClassAlt : public WindowClassControlBaseTemplate<WndClsDerivedTemplateClassAlt<DerWndCls>, ControlType>		//строим новый класс на основе предыдущего производного
{
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	//конструктор для инициализации класса по умолчанию
	WndClsDerivedTemplateClassAlt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassControlBaseTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WndClsDerivedTemplateClassAlt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassControlBaseTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}
	WndClsDerivedTemplateClassAlt(WndClsDerivedTemplateClassAlt& wcObj) : WindowClassControlBaseTemplate(wcObj)			//конструктор копирования
	{
		usiWndNum++;		//увеличиваем количество объектов данного класса
	}

	virtual ~WndClsDerivedTemplateClassAlt() override		//виртуальный деструктор
	{
		if (hWnd)
			this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
	}

	virtual void OnCreate(HWND hWnd) override				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	{
		//обеспечивает обработку WM_CREATE внутри оконной процедуры
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 200, 200)));
	}
	virtual void OnPaint(HWND hWnd) override				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	{
		//обеспечивает обработку WM_PAINT внутри оконной процедуры
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Шаблонный переопределённый класс (единичное наследование - ТРЕТЬЯ ВЕТВЬ)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
	virtual void OnDestroy(HWND hWnd) override				//обеспечивает обработку WM_DESTROY внутри оконной процедуры
	{
		//обеспечивает обработку WM_DESTROY внутри оконной процедуры	
		usiWndNum--;		//уменьшаем количество объектов данного класса
		WndClsDerivedTemplateClassAlt::hWnd = NULL;
		if (usiWndNum == 0)
			PostQuitMessage(0);
	}
};

//реализация шаблонных классов многократного наследования

//инициализация статических данных
template<class DerWndCls, class ControlType> unsigned short int WndClsDerivedTemplateClass<DerWndCls, ControlType>::usiWndNum = 0;		//количество объектов переопределённого класса
template<class DerWndCls, class ControlType> unsigned short int WndClsDerivedTemplateClassAlt<DerWndCls, ControlType>::usiWndNum = 0;		//количество объектов переопределённого класса

//функции-члены класса

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle) : WindowClassControlBaseTemplate(hInstance, szClassName, szWndTitle)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(WNDCLASSEX& wc, const TCHAR *szWndTitle) : WindowClassControlBaseTemplate(wc, szWndTitle)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(WndClsDerivedTemplateClass& wcObj) : WindowClassControlBaseTemplate(wcObj)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::~WndClsDerivedTemplateClass()
{
	if (hWnd)
		this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
}

template<class DerWndCls, class ControlType> void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnCreate(HWND hWnd)
{
	//обеспечивает обработку WM_CREATE внутри оконной процедуры
	SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(100, 160, 255)));
}

template<class DerWndCls, class ControlType > void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnPaint(HWND hWnd)
{
	//обеспечивает обработку WM_PAINT внутри оконной процедуры
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("Шаблонный переопределённый класс (единичное наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

template<class DerWndCls, class ControlType> void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnDestroy(HWND hWnd)
{
	//обеспечивает обработку WM_DESTROY внутри оконной процедуры	
	usiWndNum--;		//уменьшаем количество объектов данного класса
	WndClsDerivedTemplateClass::hWnd = NULL;
	if (usiWndNum == 0)
		PostQuitMessage(0);
}