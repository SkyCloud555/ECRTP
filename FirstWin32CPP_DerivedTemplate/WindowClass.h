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
#include <Windows.h>			//основной включаемый файл Windows

//объявления классов

template<class WndCls> class List;				//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClass
template<class WndCls> class WindowClass;		//класс окна Windows

//структуры

template<class WndCls> struct ListElement				//узел списка
{
	//данные узла
	HWND hWnd;					//дескриптор окна Windows
	WindowClass<WndCls> *p_wndClass;	//указатель на объект класса WindowClass

	ListElement *pNext;			//указатель на следующий элемент списка
	ListElement *pPrev;			//указатель на предыдущий элемент списка
};

//классы

template<class WndCls> class WindowClass				//класс окна Windows
{
	using WndProcCallback = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);		//тип функции оконной процедуры

protected:						//изменение для производных классов!
	//данные
	HWND hWnd = NULL;								//дескриптор класса окна	
	WNDCLASSEX wc = { 0 };							//структура для регистрации класса окна внутри Windows
	const TCHAR *szWndTitle = nullptr;				//заголовок окна

	static const TCHAR *szWndTitleDefault;			//строка заголовка по умолчанию
	static List<WndCls> wndList;					//статический список, единый для всех классов

	//функции	
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//инициализирует и создаёт окно (вызывается из конструкторов)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//оконная процедура (статическая функция)

	template<class T, typename = T::OnCreate> void LaunchOnCreate(HWND hWnd, T *p_wndClass)		//ошибка! см. проект FirstWin32CPP_DerivedTemplate2
	{
		//выполняет запуск OnCreate для класса WndCls, если OnCreate определена в нём
		T::OnCreate(hWnd);
	}

	template<class T> void LaunchOnCreate(HWND hWnd, T *p_wndClass)		//выполняет запуск OnCreate с помощью механизма виртуальных функций по указателю на класс
	{
		p_wndClass->OnCreate(hWnd);			//запуск с помощью механизма виртуальных функций
	}

	void OnCreate(HWND hWnd);				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd);				//обеспечивает обработку WM_PAINT внутри оконной процедуры	
	virtual void OnClose(HWND hWnd);				//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd);				//обеспечивает обработку WM_DESTROY внутри оконной процедуры

	//привилегированные классы
	friend List<WndCls>;

public:
	//функции
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//конструктор для инициализации класса по умолчанию
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClass(WindowClass&);			//конструктор копирования
	
	virtual ~WindowClass();		//виртуальный деструктор
};

template<class WndCls> class List						//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClass
{
	using lstElement = ListElement<WndCls>;				//объявляем тип элемента списка, параметризированный типом класса окна

	lstElement *pFirst = nullptr;		//C++ 11: инициализация члена класса
	lstElement *pLast = nullptr;		//C++ 11: инициализация члена класса

	void ListInitialization(WindowClass<WndCls> *p_wndClass)
	{
		if (!pFirst)
		{
			pFirst = pLast = new lstElement;			//обработку ошибок в виде исключений, кодов ошибки и т.п. я пока опускаю, т.к. проект учебный	
			if (pFirst != nullptr)
			{
				pFirst->hWnd = p_wndClass->hWnd;
				pFirst->p_wndClass = p_wndClass;

				pFirst->pNext = nullptr;
				pFirst->pPrev = nullptr;
			}
		}
	}

public:
	List(WindowClass<WndCls> *p_wndClass = nullptr)		//конструктор
	{
		if (p_wndClass != nullptr)
			ListInitialization(p_wndClass);		//проводим инициализацию класса
	}
	~List()										//деструктор
	{
		//удаляем все элементы списка
		lstElement *pCurr = pFirst;
		lstElement *pNext = nullptr;
		while (pCurr != nullptr)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
		}
	}

	lstElement * add(WindowClass<WndCls> *p_wndClass)			//добавляет новый элемент в конец списка
	{
		if (pLast != nullptr && p_wndClass != nullptr)
		{
			lstElement *pCurr = new lstElement;
			if (pCurr != nullptr)
			{
				pLast->pNext = pCurr;
				pCurr->pPrev = pLast;
				pCurr->pNext = nullptr;
				pLast = pCurr;

				pCurr->hWnd = p_wndClass->hWnd;
				pCurr->p_wndClass = p_wndClass;

				return pCurr;
			}
			else
				return nullptr;
		}
		else
		{
			if (p_wndClass != nullptr)
			{
				ListInitialization(p_wndClass);		//проводим инициализацию класса
				return pFirst;
			}
			else
				return nullptr;
		}
	}

	lstElement * search(HWND hWnd)				//выполняет поиск по заданному hWnd внутри списка
	{
		lstElement *pCurr = pFirst;
		while (pCurr != nullptr)
		{
			if (pCurr->hWnd == hWnd)
				break;
			else
				pCurr = pCurr->pNext;
		}
		if (pCurr != nullptr && pCurr->hWnd != hWnd)			//устанавливаем признак, что не нашли нужный узел
			pCurr = nullptr;
		return pCurr;
	}


	bool del(WindowClass<WndCls> *p_wndClass)		//удаляет узел списка по заданному WindowClass*
	{
		if (p_wndClass == nullptr)
			return false;

		//ищем узел, содержащий данный указатель
		lstElement *le = search(p_wndClass->hWnd);
		if (le == nullptr)
			return false;

		//нашли узел, связываем предыдущий и следующий элементы воедино вместо удаляемого
		lstElement *pPrev = le->pPrev;
		lstElement *pNext = le->pNext;
		if (pPrev == nullptr)		//удаляем самый первый элемент списка		
			pFirst = pNext;
		if (pNext == nullptr)		//удаляем последний элемент списка
			pLast = pPrev;

		if(pPrev != nullptr && pPrev->pNext != nullptr)
			pPrev->pNext = pNext;
		if(pNext != nullptr && pNext->pPrev != nullptr)
			pNext->pPrev = pPrev;

		//удаляем текущий узел
		delete le;
		return true;
	}
};

//РЕАЛИЗАЦИЯ ШАБЛОНА КЛАССА

//инициализация статических данных
template<class WndCls> List<WndCls> WindowClass<WndCls>::wndList;						//инициализация статического объекта класса
template<class WndCls> const TCHAR *WindowClass<WndCls>::szWndTitleDefault = TEXT("Window");		//строка заголовка по умолчанию

//функции-члены класса

template<class WndCls> bool WindowClass<WndCls>::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
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

template<class WndCls> WindowClass<WndCls>::WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами по умолчанию
	//на вход: hInstance - описатель приложения, szClassName - имя класса окна (внутри Windows), szWndTitle - строка заголовка окна,
	//параметр WndCls служит только для вывода параметра шаблона

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

template<class WndCls> WindowClass<WndCls>::WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами, переданными через wc
	//на вход: wc - ссылка на структуру класса окна для регистрации внутри Windows, szWndTitle - строка заголовка окна,
	//параметр WndCls служит только для вывода параметра шаблона

	WindowClass::wc = wc;
	WindowClass::wc.lpfnWndProc = WndProc;
	WindowClass::szWndTitle = szWndTitle;
	//создаём окно
	CreateWnd(WindowClass::wc, false, szWndTitle);
}

template<class WndCls> WindowClass<WndCls>::WindowClass(WindowClass& wcObj)	//конструктор копирования
{
	//копируем нестатические данные-члены класса
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	CreateWnd(wc, true, szWndTitle);			//пропускаем регистрацию класса, так как он уже зарегистрирован при создании объекта, из которого производится копирование
}

template<class WndCls> WindowClass<WndCls>::~WindowClass()						//деструктор
{
	wndList.del((WindowClass<WndCls> *)this);							//удаляем из списка
}

template<class WndCls> LRESULT CALLBACK WindowClass<WndCls>::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//оконная процедура	
	ListElement<WndCls> * pListElem = nullptr;

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
			p_wndClass->LaunchOnCreate<WndCls>(hWnd, (WndCls *)p_wndClass);
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

template<class WndCls> void WindowClass<WndCls>::OnCreate(HWND hWnd)
{
	//обеспечивает обработку WM_CREATE внутри оконной процедуры
}

template<class WndCls> void WindowClass<WndCls>::OnPaint(HWND hWnd)
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

template<class WndCls> void WindowClass<WndCls>::OnClose(HWND hWnd)
{
	//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	DestroyWindow(hWnd);
}

template<class WndCls> void WindowClass<WndCls>::OnDestroy(HWND hWnd)
{
	//обеспечивает обработку WM_DESTROY внутри оконной процедуры
	PostQuitMessage(0);
}