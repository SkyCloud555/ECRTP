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

//классы

class WindowClassDerived : public WindowClass<WindowClassDerived>		//построение нового класса с другой логикой работы на основе старого
{
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	WindowClassDerived(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//конструктор для инициализации класса по умолчанию
	WindowClassDerived(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerived(WindowClassDerived&);			//конструктор копирования

	virtual ~WindowClassDerived() override;		//виртуальный деструктор

	void OnCreate(HWND hWnd);				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd) override;				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd) override;				//обеспечивает обработку WM_DESTROY внутри оконной процедуры
};

class WindowClassDerivedNext : public WindowClassDerived		//построение нового класса с другой логикой работы на основе старого
{
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	//конструктор для инициализации класса по умолчанию
	WindowClassDerivedNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerivedNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(wc, szWndTitle) {}

	void OnCreate(HWND hWnd)				//обеспечивает обработку WM_CREATE внутри оконной процедуры
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
		DrawText(hDC, TEXT("Переопределённый класс (ПОВТОРНОЕ НАСЛЕДОВАНИЕ)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};