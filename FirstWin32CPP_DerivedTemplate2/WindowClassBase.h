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
#include <type_traits>			//enable_if и прочие логические операции с типами времени компиляции
#include <Windows.h>			//основной включаемый файл Windows

//объявления классов

class List;						//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClassBase
class WindowClassBase;			//класс окна Windows (основа шаблонного класса)

//структуры

struct ListElement				//узел списка
{
	//данные узла
	HWND hWnd;					//дескриптор окна Windows
	WindowClassBase *p_wndClass;	//указатель на объект класса WindowClassBase

	ListElement *pNext;			//указатель на следующий элемент списка
	ListElement *pPrev;			//указатель на предыдущий элемент списка
};

template<class T> struct has_OnCreate		//проверяем наличие функции-члена OnCreate в классе T посредством SFINAE: https://habrahabr.ru/post/205772/
{
	static int detect(...);						//функция с произвольным количеством произвольных аргументов, служащая для создания ошибочной ситуации (её тип должен отличаться  от возвращаемого типа проверяемой функции-члена)
	template<class U> static decltype(std::declval<U>().OnCreate(NULL)) detect(const U&);	//функция-детектор, срабатывающая, если класс U имеет нужную функцию-член

	static constexpr bool value = std::is_same<void, decltype(detect(std::declval<T>()))>::value;		//флаг наличия OnCreate (см. учебный проект CPP 11\SFINAE\HasMember)
};

//классы

class WindowClassBase				//класс окна Windows
{
protected:						//изменение для производных классов!
	//данные
	HWND hWnd = NULL;								//дескриптор класса окна	
	WNDCLASSEX wc = { 0 };							//структура для регистрации класса окна внутри Windows
	const TCHAR *szWndTitle = nullptr;				//заголовок окна
	void *p_drvWndCls;								//указатель на производный класс, дополняющий этот основной (т.к. шаблонные данные-члены допустимы только
													//статические, то используем (по старинке) указатель без типа, т.е. указатель на void

	static const TCHAR *szWndTitleDefault;			//строка заголовка по умолчанию
	static List wndList;							//статический список, единый для всех классов

	//функции
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//инициализирует и создаёт окно (вызывается из конструкторов)
	
	template<class WndCls> void LaunchOnCreate(HWND hWnd, std::true_type)
	{
		//выполняет запуск OnCreate для класса WndCls, если OnCreate определена в нём
		if (p_drvWndCls)
			(static_cast<WndCls *>(p_drvWndCls))->WndCls::OnCreate(hWnd);
	}

	template<class WndCls> void LaunchOnCreate(HWND hWnd, std::false_type)
	{
		//OnCreate нет в классе - ничего не вызываем (страховка на этапе компиляции, если производный шаблонный класс будет радикально изменён)
		//хотя по факту в SFINAE здесь нет необходимости: можно оставить только вариант выше - если OnCreate нигде нет, то просто не скомпилируется
	}
	
	virtual void OnPaint(HWND hWnd);				//обеспечивает обработку WM_PAINT внутри оконной процедуры	
	virtual void OnClose(HWND hWnd);				//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd);				//обеспечивает обработку WM_DESTROY внутри оконной процедуры

	//привилегированные классы и функции
	friend List;
	template<class WndCls> friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//оконная процедура

public:
	//функции
	template<class WndCls> WindowClassBase(WndCls *p_wndClass, HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//конструктор для инициализации класса по умолчанию
	template<class WndCls> WindowClassBase(WndCls *p_wndClass, WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassBase(WindowClassBase&);			//конструктор копирования

	virtual ~WindowClassBase();		//виртуальный деструктор
};

class List						//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClassBase
{
	ListElement *pFirst = nullptr;		//C++ 11: инициализация члена класса
	ListElement *pLast = nullptr;		//C++ 11: инициализация члена класса

	void ListInitialization(WindowClassBase *p_wndClass)
	{
		if (!pFirst)
		{
			pFirst = pLast = new ListElement;			//обработку ошибок в виде исключений, кодов ошибки и т.п. я пока опускаю, т.к. проект учебный	
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
	List(WindowClassBase *p_wndClass = nullptr)		//конструктор
	{
		if (p_wndClass != nullptr)
			ListInitialization(p_wndClass);		//проводим инициализацию класса
	}
	~List()										//деструктор
	{
		//удаляем все элементы списка
		ListElement *pCurr = pFirst;
		ListElement *pNext = nullptr;
		while (pCurr != nullptr)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
		}
	}

	ListElement * add(WindowClassBase *p_wndClass)			//добавляет новый элемент в конец списка
	{
		if (pLast != nullptr && p_wndClass != nullptr)
		{
			ListElement *pCurr = new ListElement;
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

	ListElement * search(HWND hWnd)				//выполняет поиск по заданному hWnd внутри списка
	{
		ListElement *pCurr = pFirst;
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

	bool del(WindowClassBase *p_wndClass)		//удаляет узел списка по заданному WindowClassBase*
	{
		if (p_wndClass == nullptr)
			return false;

		//ищем узел, содержащий данный указатель
		ListElement *le = search(p_wndClass->hWnd);
		if (le == nullptr)
			return false;

		//нашли узел, связываем предыдущий и следующий элементы воедино вместо удаляемого
		ListElement *pPrev = le->pPrev;
		ListElement *pNext = le->pNext;
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

//реализация шаблонных функций и функций-членов класса WindowClassBase

template<class WndCls> WindowClassBase::WindowClassBase(WndCls *p_wndClass, HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами по умолчанию
	//на вход: p_wndClass - указатель на производный класс, по типу которого будет выводиться тип шаблонного конструктора, hInstance - описатель приложения,
	//szClassName - имя класса окна (внутри Windows), szWndTitle - строка заголовка окна

	if (hInstance == NULL || szClassName == nullptr)
		return;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc<WndCls>;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	p_drvWndCls = p_wndClass;		//сохраняем указатель на производный класс, чтобы вызывать OnCreate() этого класса при обработке сообщения WM_CREATE

	//создаём окно
	WindowClassBase::szWndTitle = szWndTitle;
	CreateWnd(wc, false, szWndTitle);
}

template<class WndCls> WindowClassBase::WindowClassBase(WndCls *p_wndClass, WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//создаём окно, инициализируя его параметрами, переданными через wc
	//на вход: p_wndClass - указатель на производный класс, по типу которого будет выводиться тип шаблонного конструктора, wc - ссылка на структуру класса
	//окна для регистрации внутри Windows, szWndTitle - строка заголовка окна

	WindowClassBase::wc = wc;
	WindowClassBase::wc.lpfnWndProc = WndProc<WndCls>;
	WindowClassBase::szWndTitle = szWndTitle;

	p_drvWndCls = p_wndClass;		//сохраняем указатель на производный класс, чтобы вызывать OnCreate() этого класса при обработке сообщения WM_CREATE

	//создаём окно
	CreateWnd(WindowClassBase::wc, false, szWndTitle);
}

template<class WndCls> LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//оконная процедура	
	ListElement * pListElem = nullptr;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//lParam содержит указатель на структуру типа CREATESTRUCT, содержающую помимо всего прочего указатель на объект класса WindowClassBase, который нам
		//нужен (см. функцию WindowClassBase::CreateWnd)
		CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
		WindowClassBase *p_wndClass = reinterpret_cast<WindowClassBase *>(cs->lpCreateParams);
		p_wndClass->hWnd = hWnd;		//инициализируем hWnd объекта класса значением, переданным в оконную процедуру
										//заносим созданное окно в список
		pListElem = WindowClassBase::wndList.add(p_wndClass);
		if (pListElem)
			p_wndClass->LaunchOnCreate<WndCls>(hWnd, std::conditional_t<has_OnCreate<WndCls>::value, std::true_type, std::false_type>());
	}
	break;
	case WM_PAINT:
		pListElem = WindowClassBase::wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)
			pListElem->p_wndClass->OnPaint(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	case WM_CLOSE:
		pListElem = WindowClassBase::wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)
			pListElem->p_wndClass->OnClose(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	case WM_DESTROY:
		pListElem = WindowClassBase::wndList.search(hWnd);		//ищем в списке объект класса по заданному дескриптору окна
		if (pListElem)
			pListElem->p_wndClass->OnDestroy(hWnd);			//вызываем виртуальную функцию, соответствующую данному дескриптору
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}