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
#include "WindowClassBase.h"						//�������� ������ ���� Windows

//����� WindowClassBase

//������������� ����������� ������
List WindowClassBase::wndList;						//������������� ������������ ������� ������
const TCHAR *WindowClassBase::szWndTitleDefault = TEXT("Window");		//������ ��������� �� ���������

//�������-����� ������

bool WindowClassBase::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
{
	//�������������� � ������ ���� (���������� �� ������������� - ��������������� �������)
	//�� ����: wc - ������ �� ��������� ������ ���� ��� ����������� ������ Windows, bSkipClassRegister - ���� �������� ����������� ������ ������ Windows,
	//szWndTitle - ������ ��������� ����
	
	if (bSkipClassRegister == false && !RegisterClassEx(&wc))		//������������ ����� ����
	{
		MessageBox(NULL, TEXT("�� ������� ���������������� ����� ��� ����!"), TEXT("������"), MB_OK | MB_ICONERROR);
		return false;
	}

	//������ ����
	if (szWndTitle == nullptr)
		szWndTitle = szWndTitleDefault;
	//������� ��������� �� ���� ������ ������ (this) � ������� CreateWindow, ����� ��� ��������� ��������� WM_CREATE ������ ������� ���������
	//������� ����������� ������ � ������
	//���� ��������� ������ �������� �������� - ��� ���� ������� � ��������� ���� CREATESTRUCT, � ����� ���� ��������� �������� � CreateWindow:
	//https://msdn.microsoft.com/en-us/library/ms632603(v=vs.85).aspx
	//��� ��������, ��� � ���� ��������� ������ ��������� ��� lpCreateParams, ������ �� �������� ��������� lParam, ����������� CreateWindow
	//���������� ��� ��������, ��� ������ ������ ��������� CREATESTRUCT ����� �������� this � lParam, � ��� ������� ��� ����������� �����������:
	//WindowClassBase *p_wndClass = reinterpret_cast<WindowClassBase *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

	//��� �� �������� ����������, ������ � �������� ���� ��� �� ������: ��� ��������, ���� ��� - �� ������ �������

	HWND hWnd = CreateWindow(wc.lpszClassName, szWndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, (HWND)NULL, (HMENU)NULL, wc.hInstance, reinterpret_cast<LPVOID>(this));

	if (!hWnd)		//WindowClassBase::hWnd ��� ���������������� ������ ������� ��������� ��� ��������� WM_CREATE; ����� ��� ��������� hWnd ������������ ������
	{				//��� �������� ���������� ������ CreateWindow
		MessageBox(NULL, TEXT("�� ������� ������� ����!"), TEXT("������"), MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);			//���������� ����	

	return true;
}

WindowClassBase::WindowClassBase(WindowClassBase& wcObj)	//����������� �����������
{
	//�������� ������������� ������-����� ������
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	p_drvWndCls = wcObj.p_drvWndCls;
	CreateWnd(wc, true, szWndTitle);			//���������� ����������� ������, ��� ��� �� ��� ��������������� ��� �������� �������, �� �������� ������������ �����������
}

WindowClassBase::~WindowClassBase()						//����������
{
	wndList.del(this);							//������� �� ������
}

void WindowClassBase::OnPaint(HWND hWnd)
{
	//������������ ��������� WM_PAINT ������ ������� ���������
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("����� �++ ��� ���� Windows ����������� Win32 API."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

void WindowClassBase::OnClose(HWND hWnd)
{
	//������������ ��������� WM_CLOSE ������ ������� ���������
	DestroyWindow(hWnd);
}

void WindowClassBase::OnDestroy(HWND hWnd)
{
	//������������ ��������� WM_DESTROY ������ ������� ���������
	PostQuitMessage(0);
}