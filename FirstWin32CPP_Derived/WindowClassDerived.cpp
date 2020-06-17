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
#include "WindowClassDerived.h"						//описание класса окна Windows (переопределённый)

//класс WindowClass

//инициализация статических данных
unsigned short int WindowClassDerived::usiWndNum = 0;		//количество объектов переопределённого класса

//функции-члены класса

WindowClassDerived::WindowClassDerived(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle) : WindowClass(hInstance, szClassName, szWndTitle)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

WindowClassDerived::WindowClassDerived(WNDCLASSEX& wc, const TCHAR *szWndTitle) : WindowClass(wc, szWndTitle)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

WindowClassDerived::WindowClassDerived(WindowClassDerived& wcObj) : WindowClass(wcObj)
{
	usiWndNum++;		//увеличиваем количество объектов данного класса
}

WindowClassDerived::~WindowClassDerived()
{
	if(hWnd)
		this->OnClose(hWnd);		//закрываем окно, используя механизм виртуальных функций
}

void WindowClassDerived::OnCreate(HWND hWnd)
{
	//обеспечивает обработку WM_CREATE внутри оконной процедуры
	SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(200, 160, 255)));
}

void WindowClassDerived::OnPaint(HWND hWnd)
{
	//обеспечивает обработку WM_PAINT внутри оконной процедуры
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("Переопределённый класс С++ для окна Windows посредством Win32 API."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

void WindowClassDerived::OnDestroy(HWND hWnd)
{
	//обеспечивает обработку WM_DESTROY внутри оконной процедуры	
	usiWndNum--;		//уменьшаем количество объектов данного класса
	WindowClassDerived::hWnd = NULL;
	if (usiWndNum == 0)
		PostQuitMessage(0);
}