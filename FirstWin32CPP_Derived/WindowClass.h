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

class List;						//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClass
class WindowClass;				//класс окна Windows

//структуры

struct ListElement				//узел списка
{
	//данные узла
	HWND hWnd;					//дескриптор окна Windows
	WindowClass *p_wndClass;	//указатель на объект класса WindowClass

	ListElement *pNext;			//указатель на следующий элемент списка
	ListElement *pPrev;			//указатель на предыдущий элемент списка
};

//классы

class WindowClass				//класс окна Windows
{
protected:						//изменение для производных классов!
	//данные
	HWND hWnd = NULL;								//дескриптор класса окна	
	WNDCLASSEX wc = { 0 };							//структура для регистрации класса окна внутри Windows
	const TCHAR *szWndTitle = nullptr;					//заголовок окна

	static const TCHAR *szWndTitleDefault;			//строка заголовка по умолчанию
	static List wndList;							//статический список, единый для всех классов

	//функции
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//оконная процедура (статическая функция)
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//инициализирует и создаёт окно (вызывается из конструкторов)
	
	virtual void OnCreate(HWND hWnd);				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd);				//обеспечивает обработку WM_PAINT внутри оконной процедуры	
	virtual void OnClose(HWND hWnd);				//обеспечивает обработку WM_CLOSE внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd);				//обеспечивает обработку WM_DESTROY внутри оконной процедуры

	//привилегированные классы
	friend List;

public:
	//функции
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//конструктор для инициализации класса по умолчанию
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClass(WindowClass&);			//конструктор копирования

	virtual ~WindowClass();		//виртуальный деструктор
};

class List						//класс, отвечающий за функционирование списка дескрипторов окон и указателей на WindowClass
{
	ListElement *pFirst = nullptr;		//C++ 11: инициализация члена класса
	ListElement *pLast = nullptr;		//C++ 11: инициализация члена класса

	void ListInitialization(WindowClass *p_wndClass)
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
	List(WindowClass *p_wndClass = nullptr)		//конструктор
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

	ListElement * add(WindowClass *p_wndClass)			//добавляет новый элемент в конец списка
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


	bool del(WindowClass *p_wndClass)		//удаляет узел списка по заданному WindowClass*
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