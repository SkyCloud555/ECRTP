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

//����������� ��������� ������ ������������� ������������ (������������)

//���������� �����
#include "WindowClass.h"

//������

//� ������ ������ ������ ����� ����� ����������� � ����� ������ ���������� �������; ��� ������, ��� � WindowClassVariadicTemplate ����� � �������
//�������� ����� ����� ������ � ������
//��������� ����� �������� ��� ���������, ��������, template<class... PrevWndClasses> class WindowClassVariadic1, �� � � ������� ����� ������� �����
//������ ������������ �� ������� ����������, ������ � ������ (�� �� �� ����� �������� ������ ��� �����, ��� ����� ���� �� ���������������)

//������ ������ � ����� ����� � ����� ��������� ��� ������, ���� �� ��� �������������� ������������ � ���������������� OnCreate

//���� �� ������� ����� ��������� ��-�������, ����� �������, �� ����, ��������:
//template<class... PrevWndClasses> class WindowClassVariadic1 : public WindowClassVariadicTemplate<WindowClassVariadic1<>, PrevWndClasses...>
//�� ����� ������ ����� �������� � ����� � ����� ������, � ��� ������ ������� ��������� �������� ������� �������� ����� ���������� GetTypeFromParamPack

template<class... PrevWndClasses> class WindowClassVariadic1 : public WindowClassVariadicTemplate<PrevWndClasses..., WindowClassVariadic1<>>
{
protected:
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadic1(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadicTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadic1(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadicTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassVariadic1(WindowClassVariadic1& wcObj) : WindowClassVariadicTemplate(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassVariadic1() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
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
		DrawText(hDC, TEXT("��������� ����������� ��������������� ����� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic2 : public WindowClassVariadic1<PrevWndClasses..., WindowClassVariadic2<>>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadic2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadic2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassVariadic2(WindowClassVariadic2& wcObj) : WindowClassVariadic1(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassVariadic2() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
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
		DrawText(hDC, TEXT("��������� ����������� ��������������� ����� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic3 : public WindowClassVariadic2<PrevWndClasses..., WindowClassVariadic3<>>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadic3(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic2(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadic3(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic2(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassVariadic3(WindowClassVariadic3& wcObj) : WindowClassVariadic2(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassVariadic3() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
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
		DrawText(hDC, TEXT("��������� ����������� ��������������� ����� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic4 : public WindowClassVariadic3<PrevWndClasses..., WindowClassVariadic4<>>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadic4(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic3(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadic4(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic3(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassVariadic4(WindowClassVariadic4& wcObj) : WindowClassVariadic3(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassVariadic4() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
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
		DrawText(hDC, TEXT("��������� ����������� ��������������� ����� (���¨���� ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

template<class... PrevWndClasses> class WindowClassVariadic2Alt : public WindowClassVariadic1<PrevWndClasses..., WindowClassVariadic2Alt<>>
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassVariadic2Alt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassVariadic2Alt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassVariadic1(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WindowClassVariadic2Alt(WindowClassVariadic2Alt& wcObj) : WindowClassVariadic1(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WindowClassVariadic2Alt() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(253, 189, 55)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ����������� ��������������� ����� (������ ������������ - ������ �����)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//���������� ��������� ������� ������������� ������������

template<class... PrevWndClasses> unsigned short int WindowClassVariadic1<PrevWndClasses...>::usiWndNum = 0;		//���������� �������� ���������������� ������
