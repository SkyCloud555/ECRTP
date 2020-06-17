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
#include "WindowClass.h"						//описание класса окна Windows

//класс WindowClass

//инициализация статических данных
List WindowClass::wndList;						//инициализация статического объекта класса
const TCHAR *WindowClass::szWndTitleDefault = TEXT("Window");		//строка заголовка по умолчанию

//функции-члены класса

bool WindowClass::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
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
	//WindowClass *p_wndClass = reinterpret_cast<WindowClass *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

	//это не является безопасным, однако я оставляю пока это за кадром: код работает, хотя это - не лучший вариант

	HWND hWnd = CreateWindow(wc.lpszClassName, szWndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, (HWND)NULL, (HMENU)NULL, wc.hInstance, reinterpret_cast<LPVOID>(this));

	if (!hWnd)		//WindowClass::hWnd уже инициализировано внутри оконной процедуры при обработке WM_CREATE; здесь это локальное hWnd используется только
	{				//для контроля результата вызова CreateWindow
		MessageBox(NULL, TEXT("Не удается создать окно!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);			//показываем окно	

	return true;
}

WindowClass::WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами по умолчанию
	//на вход: hInstance - описатель приложения, szClassName - имя класса окна (внутри Windows), szWndTitle - строка заголовка окна

	if (hInstance == NULL || szClassName == nullptr)
		return;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);	
	
	//создаём окно
	WindowClass::szWndTitle = szWndTitle;
	CreateWnd(wc, false, szWndTitle);
}

WindowClass::WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами, переданными через wc
	//на вход: wc - ссылка на структуру класса окна для регистрации внутри Windows, szWndTitle - строка заголовка окна

	WindowClass::wc = wc;
	WindowClass::wc.lpfnWndProc = WndProc;		//инициализируем оконную процедуру заданной статической функцией-членом внутри класса
	WindowClass::szWndTitle = szWndTitle;
	//создаём окно
	CreateWnd(WindowClass::wc, false, szWndTitle);
}

WindowClass::WindowClass(WindowClass& wcObj)	//конструктор копирования
{
	//копируем нестатические данные-члены класса
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	CreateWnd(wc, true, szWndTitle);			//пропускаем регистрацию класса, так как он уже зарегистрирован при создании объекта, из которого производится копирование
}

WindowClass::~WindowClass()						//деструктор
{
	wndList.del(this);							//удаляем из списка
}

LRESULT CALLBACK WindowClass::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//оконная процедура	
	ListElement * pListElem = nullptr;

	switch (uMsg)
	{
	case WM_CREATE:
		{
			//lParam содержит указатель на структуру типа CREATESTRUCT, содержающую помимо всего прочего указатель на объект класса WindowClass, который нам
			//нужен (см. функцию WindowClass::CreateWnd)
			CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
			WindowClass *p_wndClass = reinterpret_cast<WindowClass *>(cs->lpCreateParams);
			p_wndClass->hWnd = hWnd;		//инициализируем hWnd объекта класса значением, переданным в оконную процедуру
			//заносим созданное окно в список
			pListElem = wndList.add(p_wndClass);
			if (pListElem)
				p_wndClass->OnCreate(hWnd);		//виртуальная функция не сработает, ибо вызывается из конструктора: тут нужно указывать явный тип для вызова нужной функции
		}
		break;
	case WM_PAINT:
		pListElem = wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)	
			pListElem->p_wndClass->OnPaint(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	case WM_CLOSE:
		pListElem = wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)
			pListElem->p_wndClass->OnClose(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	case WM_DESTROY:
		pListElem = wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)
			pListElem->p_wndClass->OnDestroy(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void WindowClass::OnCreate(HWND hWnd)
{
	//обеспечивает обработку WM_CREATE внутри оконной процедуры
}

void WindowClass::OnPaint(HWND hWnd)
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

void WindowClass::OnClose(HWND hWnd)
{
	//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	DestroyWindow(hWnd);
}

void WindowClass::OnDestroy(HWND hWnd)
{
	//обеспечивает обработку WM_DESTROY внутри оконной процедуры
	PostQuitMessage(0);
}