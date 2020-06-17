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
#include "WindowClassBase.h"						//описание класса окна Windows

//класс WindowClassBase

//инициализация статических данных
List WindowClassBase::wndList;						//инициализация статического объекта класса
const TCHAR *WindowClassBase::szWndTitleDefault = TEXT("Window");		//строка заголовка по умолчанию

//функции-члены класса

bool WindowClassBase::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
{
	//инициализирует и создаёт окно (вызывается из конструкторов - вспомогательная функция)
	//на вход: wc - ссылка на структуру класса окна для регистрации внутри Windows, bSkipClassRegister - флаг пропуска регистрации класса внутри Windows,
	//szWndTitle - строка заголовка окна
	
	if (bSkipClassRegister == false && !RegisterClassEx(&wc))		//регистрируем класс окна
	{
		MessageBox(NULL, TEXT("Не удается зарегистрировать класс для окна!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return false;
	}

	//создаём окно
	if (szWndTitle == nullptr)
		szWndTitle = szWndTitleDefault;
	//передаём указатель на этот объект класса (this) в функцию CreateWindow, чтобы при обработке сообщения WM_CREATE внутри оконной процедуры
	//занести создаваемый объект в список
	//этот указатель нельзя передать напрямую - его надо занести в структуру типа CREATESTRUCT, и адрес этой структуры передать в CreateWindow:
	//https://msdn.microsoft.com/en-us/library/ms632603(v=vs.85).aspx
	//там написано, что в этой структуре первым элементом идёт lpCreateParams, равный по значению параметру lParam, переданному CreateWindow
	//фактически это означает, что вместо адреса структуры CREATESTRUCT можно передать this в lParam, а там считать его посредством конструкции:
	//WindowClassBase *p_wndClass = reinterpret_cast<WindowClassBase *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

	//это не является безопасным, однако я оставляю пока это за кадром: код работает, хотя это - не лучший вариант

	HWND hWnd = CreateWindow(wc.lpszClassName, szWndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, (HWND)NULL, (HMENU)NULL, wc.hInstance, reinterpret_cast<LPVOID>(this));

	if (!hWnd)		//WindowClassBase::hWnd уже инициализировано внутри оконной процедуры при обработке WM_CREATE; здесь это локальное hWnd используется только
	{				//для контроля результата вызова CreateWindow
		MessageBox(NULL, TEXT("Не удается создать окно!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);			//показываем окно	

	return true;
}

WindowClassBase::WindowClassBase(WindowClassBase& wcObj)	//конструктор копирования
{
	//копируем нестатические данные-члены класса
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	p_drvWndCls = wcObj.p_drvWndCls;
	CreateWnd(wc, true, szWndTitle);			//пропускаем регистрацию класса, так как он уже зарегистрирован при создании объекта, из которого производится копирование
}

WindowClassBase::~WindowClassBase()						//деструктор
{
	wndList.del(this);							//удаляем из списка
}

void WindowClassBase::OnPaint(HWND hWnd)
{
	//обеспечивает обработку WM_PAINT внутри оконной процедуры
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("Класс С++ для окна Windows посредством Win32 API."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

void WindowClassBase::OnClose(HWND hWnd)
{
	//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	DestroyWindow(hWnd);
}

void WindowClassBase::OnDestroy(HWND hWnd)
{
	//обеспечивает обработку WM_DESTROY внутри оконной процедуры
	PostQuitMessage(0);
}