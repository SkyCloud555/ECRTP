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

//������ ������, ������������ WindowClassBase, �� ���� ���� �������� ���������������� ����� "�������" ������

//���������� �����
#include <tuple>						//������ std::tuple
#include "WindowClassBase.h"			//������� ����� ��� WindowClass, ����������� ��� �������� �������������� �������� �� �������� ���� Windows

//���������

//���������� ���������� �� ������������ ������ ���������� ������� (��������� n-�� �������� �� ������)

template<class... Classes> struct Tuple;		//����� ���������� ��� �������� ���������� ������� ���������

//��������� ������� �� ���� ������ - �������� class First � "�������" ��������� ����� Tuple<Others...>
//����� �������, � ���� �������� ������������� ����������� ������ ��� ������������ �� ���� ������ ����������
template<class First, class... Others> struct Tuple<First, Others...> : Tuple<Others...>
{
	using FirstType = First;
};

//���������� ��� ��������
template<> struct Tuple<> {};

//������ ����� ��� ���� ����������� ������ - ��� ���������� N-�� �������� ��������� ���� ����������� ���������

template<unsigned int k, class... Classes> struct elem_type_holder;

template<unsigned int k, class Type, class... Others> struct elem_type_holder<k, Tuple<Type, Others...>>
{
	using type = typename elem_type_holder<k - 1, Tuple<Others...>>::type;
};

template<class Type, class... Others> struct elem_type_holder<0, Tuple<Type, Others...>>
{
	using type = Type;
};

//�������������� ��������� ����������� ���������� ���� ��� ���������� N-�� �������� �� ������ ���������� �������
template<unsigned int N, class... Classes> using GetTypeFromParamPack = typename elem_type_holder<N, Tuple<Classes...>>::type;

//������

class thisclass {};						//�����-��������, ������������ ��� ��������� �� ��������� � ������, ����������� �� WindowClassTemplateDerived

class WindowClass : public WindowClassBase			//�����, ����������� WindowClassBase �� ���������� ���������������� ������
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(this, hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(this, wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//������������ ��������� WM_CREATE ������ ������� ���������
};

//����������� ������������

template<class DerWndCls> class WindowClassTemplate : public WindowClassBase			//�����, ����������� WindowClassBase �� ���������� ���������������� ������ (��������� ��� ������������)
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerWndCls *>(this), hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerWndCls *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//������������ ��������� WM_CREATE ������ ������� ���������
};

//��������� ����� ��� ������������� ������������ (������ 1)

template<class DerWndCls, class ControlType> class WindowClassControlBaseTemplate : public WindowClassBase
{
	//���� ��������� ControlType == thisclass, �� ����� ����� ������������ ��� DerWndCls, � ������� ��������� �����, ������������ �������� WindowClassBase
	//���� �� ControlType != thisclass, ����� ������� ������������ ControlType, ������������� ������ � ������� �������� ������������ ������ (��� ���������
	//���������� ����������� � �������� ControlType ��������� � ������������ �������� ��������)
	using DerivedWndClassType = std::conditional_t<std::is_same<ControlType, thisclass>::value, DerWndCls, ControlType>;

public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassControlBaseTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassControlBaseTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//������������ ��������� WM_CREATE ������ ������� ���������
};

//��������� ����� ��� ������������� ������������ (������ 2 - ����������� ������)

//��� ������������ ������ ������� ����� �������� ������� ���� � ����� ������ ����������, �� ����� ������ ��� ����� (�� ������� ��������) ������ ���� ����� ������
//� ������
//� ��������� ������ (����� ������ ������� �������� ���� ����� � ������) - ������ ��� ����� ��������� ����� ���������

//����������, ����� ������ ��� ����� ���������� ���������
template<class... Classes> class WindowClassVariadicTemplate;		//����� ���������� ������

//�������������, ��� ������� ������ � ������ ���������� ����� ���������� �� ���������
template<class DerWndCls, class... OtherWindowClasses> class WindowClassVariadicTemplate<DerWndCls, OtherWindowClasses...> : public WindowClassBase
{
	//������ ��������� ����� ������ ����� � ������: ������ ��� ����� - DerWndCls
	using DerivedWndClassType = DerWndCls;

public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadicTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadicTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) {}				//������������ ��������� WM_CREATE ������ ������� ���������
};

//����������, ����� ������ ��� ����� ���������� ���������
//template<class... DerivedWindowClasses> class WindowClassVariadicTemplate : public WindowClassBase
//{
//	//���� ����� �������: � ����������� ������� ���������� �����, ��������� �� ���� ������� ������������
//	//������ ��� �����, ����������� � ������� ��������, ������������� ���������; ����� ���������� ���������� �������, ��������� ����� ��������� � �������
//	//��� WindowClassBase
//
//	//���������� ���������� ���������� � ������ ���������� ������� (� �� ����� - ���������� ������������ ������� ������)
//	static constexpr unsigned int N = sizeof...(DerivedWindowClasses);
//	//��������� ������ �����
//	using DerivedWndClassType = std::conditional_t<N == 0, thisclass, GetTypeFromParamPack<N - 1, DerivedWindowClasses...>>;
//
//public:
//	//����������� ��� ������������� ������ �� ���������
//	WindowClassVariadicTemplate(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), hInstance, szClassName, szWndTitle) {}
//	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
//	WindowClassVariadicTemplate(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassBase(static_cast<DerivedWndClassType *>(this), wc, szWndTitle) {}
//
//	virtual void OnCreate(HWND hWnd) {}				//������������ ��������� WM_CREATE ������ ������� ���������
//};