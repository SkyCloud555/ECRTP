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

//шаблон класса, дополняющего WindowClassBase, за счёт чего возможно функционирование этого "парного" класса

//включаемые файлы
#include <tuple>						//кортеж std::tuple
#include "WindowClassBase.h"			//базовый класс для WindowClass, выполняющий все основные функциональные действия по созданию окна Windows

//структуры

//распаковка параметров из вариативного набора параметров шаблона (получение n-го элемента из набора)

template<class... Classes> struct Tuple;		//общее объявление для указания параметров шаблона структуры

//структура состоит из двух частей - головной class First и "базовой" остальной части Tuple<Others...>
//таким образом, в этой основной специализации сохраняется первый тип поступившего на вход пакета параметров
template<class First, class... Others> struct Tuple<First, Others...> : Tuple<Others...>
{
	using FirstType = First;
};

//определяем дно рекурсии
template<> struct Tuple<> {};

//теперь нужен ещё один вариативный шаблон - для извлечения N-го элемента описанной выше вариативной структуры

template<unsigned int k, class... Classes> struct elem_type_holder;

template<unsigned int k, class Type, class... Others> struct elem_type_holder<k, Tuple<Type, Others...>>
{
	using type = typename elem_type_holder<k - 1, Tuple<Others...>>::type;
};

template<class Type, class... Others> struct elem_type_holder<0, Tuple<Type, Others...>>
{
	using type = Type;
};

//результирующее шаблонное вариативное объявление типа для извлечения N-го элемента из пакета параметров шаблона
template<unsigned int N, class... Classes> using GetTypeFromParamPack = typename elem_type_holder<N, Tuple<Classes...>>::type;

//классы

class thisclass {};						//класс-пустышка, используемый для аргумента по умолчанию в классе, производном от WindowClassTemplateDerived

class WindowClass : public WindowClassBase			//класс, дополняющий WindowClassBase до полноценно функционирующего класса
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(this, hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(this, wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//обеспечивает обработку WM_CREATE внутри оконной процедуры
};

//однократное наследование

template<class DerWndCls> class WindowClassTemplate : public WindowClassBase			//класс, дополняющий WindowClassBase до полноценно функционирующего класса (заготовка для наследования)
{
public:
	//конструктор для инициализации класса по умолчанию
	WindowClassTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerWndCls *>(this), hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerWndCls *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//обеспечивает обработку WM_CREATE внутри оконной процедуры
};

//шаблонный класс для многократного наследования (способ 1)

template<class DerWndCls, class ControlType> class WindowClassControlBaseTemplate : public WindowClassBase
{
	//если передаётся ControlType == thisclass, то тогда нужно использовать сам DerWndCls, в котором передаётся класс, передаваемый напрямую WindowClassBase
	//если же ControlType != thisclass, тогда следует использовать ControlType, эквивалентный классу в вершине иерархии наследования класса (при правильно
	//соблюдённых соглашениях о передаче ControlType вершинным и нижележащими базовыми классами)
	using DerivedWndClassType = std::conditional_t<std::is_same<ControlType, thisclass>::value, DerWndCls, ControlType>;

public:
	//конструктор для инициализации класса по умолчанию
	WindowClassControlBaseTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassControlBaseTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//обеспечивает обработку WM_CREATE внутри оконной процедуры
};

//шаблонный класс для многократного наследования (способ 2 - вариативный шаблон)

//при наследовании каждый текущий класс иерархии помещал себя в конец списка параметров, то тогда нужный нам класс (на вершине иерархии) должен быть самым первым
//в списке
//в противном случае (когда каждый текущий помещает свой класс в начало) - нужный нам класс находится самым последним

//реализация, когда нужный нам класс расположен последним
template<class... Classes> class WindowClassVariadicTemplate;		//общее объявление класса

//специализация, при которой первый в списке параметров класс отделяется от остальных
template<class DerWndCls, class... OtherWindowClasses> class WindowClassVariadicTemplate<DerWndCls, OtherWindowClasses...> : public WindowClassBase
{
	//просто извлекаем самый первый класс в списке: нужный нам класс - DerWndCls
	using DerivedWndClassType = DerWndCls;

public:
	//конструктор для инициализации класса по умолчанию
	WindowClassVariadicTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassVariadicTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//обеспечивает обработку WM_CREATE внутри оконной процедуры
};

//реализация, когда нужный нам класс расположен последним
//template<class... DerivedWindowClasses> class WindowClassVariadicTemplate : public WindowClassBase
//{
//	//идея очень простая: в вариативном шаблоне содержится набор, состоящий из всех классов наследования
//	//нужный нам класс, находящийся в вершине иерархии, располагается последним; узнав количество переданных классов, извлекаем самый последний и передаём
//	//его WindowClassBase
//
//	//количество переданных параметров в пакете параметров шаблона (а по факту - количество наследований данного класса)
//	static constexpr unsigned int N = sizeof...(DerivedWindowClasses);
//	//извлекаем нужный класс
//	using DerivedWndClassType = std::conditional_t<N == 0, thisclass, GetTypeFromParamPack<N - 1, DerivedWindowClasses...>>;
//
//public:
//	//конструктор для инициализации класса по умолчанию
//	WindowClassVariadicTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
//	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
//	WindowClassVariadicTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}
//
//	virtual void OnCreate(HWND hWnd) {}				//обеспечивает обработку WM_CREATE внутри оконной процедуры
//};