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
#include "WindowClass.h"						//�������� ������ ���� Windows

//����� WindowClass

//������������� ����������� ������
List WindowClass::wndList;						//������������� ������������ ������� ������
const TCHAR *WindowClass::szWndTitleDefault = TEXT("Window");		//������ ��������� �� ���������

//�������-����� ������

bool WindowClass::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
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
	//WindowClass *p_wndClass = reinterpret_cast<WindowClass *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

	//��� �� �������� ����������, ������ � �������� ���� ��� �� ������: ��� ��������, ���� ��� - �� ������ �������

	HWND hWnd = CreateWindow(wc.lpszClassName, szWndTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, (HWND)NULL, (HMENU)NULL, wc.hInstance, reinterpret_cast<LPVOID>(this));

	if (!hWnd)		//WindowClass::hWnd ��� ���������������� ������ ������� ��������� ��� ��������� WM_CREATE; ����� ��� ��������� hWnd ������������ ������
	{				//��� �������� ���������� ������ CreateWindow
		MessageBox(NULL, TEXT("�� ������� ������� ����!"), TEXT("������"), MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);			//���������� ����	

	return true;
}

WindowClass::WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� ����������� �� ���������
	//�� ����: hInstance - ��������� ����������, szClassName - ��� ������ ���� (������ Windows), szWndTitle - ������ ��������� ����

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
	
	//������ ����
	WindowClass::szWndTitle = szWndTitle;
	CreateWnd(wc, false, szWndTitle);
}

WindowClass::WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� �����������, ����������� ����� wc
	//�� ����: wc - ������ �� ��������� ������ ���� ��� ����������� ������ Windows, szWndTitle - ������ ��������� ����

	WindowClass::wc = wc;
	WindowClass::wc.lpfnWndProc = WndProc;		//�������������� ������� ��������� �������� ����������� ��������-������ ������ ������
	WindowClass::szWndTitle = szWndTitle;
	//������ ����
	CreateWnd(WindowClass::wc, false, szWndTitle);
}

WindowClass::WindowClass(WindowClass& wcObj)	//����������� �����������
{
	//�������� ������������� ������-����� ������
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	CreateWnd(wc, true, szWndTitle);			//���������� ����������� ������, ��� ��� �� ��� ��������������� ��� �������� �������, �� �������� ������������ �����������
}

WindowClass::~WindowClass()						//����������
{
	wndList.del(this);							//������� �� ������
}

LRESULT CALLBACK WindowClass::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//������� ���������	
	ListElement * pListElem = nullptr;

	switch (uMsg)
	{
	case WM_CREATE:
		{
			//lParam �������� ��������� �� ��������� ���� CREATESTRUCT, ����������� ������ ����� ������� ��������� �� ������ ������ WindowClass, ������� ���
			//����� (��. ������� WindowClass::CreateWnd)
			CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
			WindowClass *p_wndClass = reinterpret_cast<WindowClass *>(cs->lpCreateParams);
			p_wndClass->hWnd = hWnd;		//�������������� hWnd ������� ������ ���������, ���������� � ������� ���������
			//������� ��������� ���� � ������
			pListElem = wndList.add(p_wndClass);
			if (pListElem)
				p_wndClass->OnCreate(hWnd);		//����������� ������� �� ���������, ��� ���������� �� ������������: ��� ����� ��������� ����� ��� ��� ������ ������ �������
		}
		break;
	case WM_PAINT:
		pListElem = wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)	
			pListElem->p_wndClass->OnPaint(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	case WM_CLOSE:
		pListElem = wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)
			pListElem->p_wndClass->OnClose(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	case WM_DESTROY:
		pListElem = wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)
			pListElem->p_wndClass->OnDestroy(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void WindowClass::OnCreate(HWND hWnd)
{
	//������������ ��������� WM_CREATE ������ ������� ���������
}

void WindowClass::OnPaint(HWND hWnd)
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

void WindowClass::OnClose(HWND hWnd)
{
	//������������ ��������� WM_CLOSE ������ ������� ���������
	DestroyWindow(hWnd);
}

void WindowClass::OnDestroy(HWND hWnd)
{
	//������������ ��������� WM_DESTROY ������ ������� ���������
	PostQuitMessage(0);
}