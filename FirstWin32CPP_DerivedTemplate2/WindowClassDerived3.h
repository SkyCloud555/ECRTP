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

//������ ������� ���������� ������������

//������ �������������, ���������������� OnCreate

class WindowClassInit1
{
public:
	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(200, 160, 255)));
	}
};

class WindowClassInit2
{
public:
	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 255, 200)));
	}
};

class WindowClassInit3
{
public:
	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(210, 250, 207)));
	}
};

class WindowClassInit4
{
public:
	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(40, 244, 255)));
	}
};

class WindowClassInit2Alt
{
public:
	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(253, 189, 55)));
	}
};

//�����, ����������� ��������������� ����������� ������� ��� ��������� ���������

template<class WndClsInit = WindowClassInit1> class WindowClassDerivedI1 : public WindowClassTemplate<WndClsInit>
{
protected:
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedI1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedI1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassDerivedI1(WindowClassDerivedI1& wcObj) : WindowClassDerivedI1(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassDerivedI1() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� � ������� ������������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
	virtual void OnDestroy(HWND hWnd) override				//������������ ��������� WM_DESTROY ������ ������� ���������
	{
		//������������ ��������� WM_DESTROY ������ ������� ���������	
		usiWndNum--;		//��������� ���������� �������� ������� ������
		if (usiWndNum == 0)
			PostQuitMessage(0);
	}
};

template<class WndClsInit = WindowClassInit2> class WindowClassDerivedI2 : public WindowClassDerivedI1<WndClsInit>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedI2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedI2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� � ������� ������������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit3> class WindowClassDerivedI3 : public WindowClassDerivedI2<WndClsInit>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedI3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI2(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedI3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI2(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� � ������� ������������� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit4> class WindowClassDerivedI4 : public WindowClassDerivedI3<WndClsInit>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedI4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI3(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedI4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI3(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� � ������� ������������� (���¨���� ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

template<class WndClsInit = WindowClassInit2Alt> class WindowClassDerivedI2Alt : public WindowClassDerivedI1<WndClsInit>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedI2Alt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedI2Alt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedI1(wc, szWndTitle) {}

	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� � ������� ������������� (������ ������������ - ������ �����)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};

//���������� ��������� ������� ������������� ������������

template<class WndClsInit> unsigned short int WindowClassDerivedI1<WndClsInit>::usiWndNum = 0;		//���������� �������� ���������������� ������