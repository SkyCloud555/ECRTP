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

//������

class WindowClassDerived : public WindowClass<WindowClassDerived>		//���������� ������ ������ � ������ ������� ������ �� ������ �������
{
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	WindowClassDerived(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//����������� ��� ������������� ������ �� ���������
	WindowClassDerived(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerived(WindowClassDerived&);			//����������� �����������

	virtual ~WindowClassDerived() override;		//����������� ����������

	void OnCreate(HWND hWnd);				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd) override;				//������������ ��������� WM_PAINT ������ ������� ���������
	virtual void OnDestroy(HWND hWnd) override;				//������������ ��������� WM_DESTROY ������ ������� ���������
};

class WindowClassDerivedNext : public WindowClassDerived		//���������� ������ ������ � ������ ������� ������ �� ������ �������
{
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(wc, szWndTitle) {}

	void OnCreate(HWND hWnd)				//������������ ��������� WM_CREATE ������ ������� ���������
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
		DrawText(hDC, TEXT("��������������� ����� (��������� ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
};