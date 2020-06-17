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

//���������� �����
#include "WindowClass.h"

//�������� ������� ���������� ������������
//���� ������� ��������� ������� �� WindowClassDerived4.h, �� ��� ���������������� ���� thistype � ��������� ������ � ������� std::is_same

//� �� �� ��������: ���������� �����, ��� ����������� ����������� ���� ��� ������� ������� ������

template<class DerWndCls = WindowClassDerivedSimplest1<>> class WindowClassDerivedSimplest1 : public WindowClassTemplate<DerWndCls>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedSimplest1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedSimplest1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest1() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override			//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(200, 160, 255)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ���������� ��������������� ����� � �������� ��������� ��������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest2<>> class WindowClassDerivedSimplest2 : public WindowClassDerivedSimplest1<DerWndCls>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedSimplest2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest1(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedSimplest2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest1(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest2() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override			//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 255, 200)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ���������� ��������������� ����� � �������� ��������� ��������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest3<>> class WindowClassDerivedSimplest3 : public WindowClassDerivedSimplest2<DerWndCls>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedSimplest3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest2(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedSimplest3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest2(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest3() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override			//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(210, 250, 207)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ���������� ��������������� ����� � �������� ��������� ��������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class DerWndCls = WindowClassDerivedSimplest4<>> class WindowClassDerivedSimplest4 : public WindowClassDerivedSimplest3<DerWndCls>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedSimplest4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest3(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedSimplest4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedSimplest3(wc, szWndTitle) {}

	virtual ~WindowClassDerivedSimplest4() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override			//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(40, 244, 255)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ���������� ��������������� ����� � �������� ��������� ��������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};