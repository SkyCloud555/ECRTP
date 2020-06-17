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

//��� ���� ����������� ������� ��� ������������

//���������� �����
#include "WindowClassDerived.h"

//������

class WindowClassDerivedNext : public WindowClassDerived		//������ ����� ����� �� ������ ����������� ������������
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerived(wc, szWndTitle) {}

	virtual void OnCreate(HWND hWnd) override;				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd) override;				//������������ ��������� WM_PAINT ������ ������� ���������
};

//��� ����� ����� ���������� ������������, � ���������, ����������� ������ ������ �� ��� ����������: ��, ��� �����, �������� ������ ��� ����������, ������������
//� ������� �������� ������, ��� ���� �� �� �������� ��������� ��������; ������ ��� ����������� OnPaint, OnClose � �.�. ���������������� ��� �������: ������
//�������� OnCreate, ���� ������� � ���� �������� ��� ��������� ����
//������ ����� ��������� ����� (� ������� ��������) � ���� ��������� ���������� ���������� (�������-���������� ControlType) �������� ��� ����; �� ���� ��
//������������� ������� ���������� OnCreate ������ ������� (��������) ������ ����� �������� ������� WindowClassControlBaseTemplate - � ����� ������ ���
//WndClsDerivedTemplateClass: ������� ����� �������, ���� ���������� ��� ������� ��������� � ������� ���������� �� ������ � ������
//� ���� ����� OnCreate ���������� �� ���� ������, ��� �������� �� ��������� ������, ������ ��� ���� ����� ������ � ��������� ControlType = thisclass, � ��
//������� ���� ��� ����� ���� �������, ������� �� ������, �� ������ ���������� ControlType ���������� �������� ������

//������� �����: ��� ���������� � ������� �������� ������ ��� ������������ �������� ControlType ����� ��������, ����� ����������� ������� �� �����������
//thisclass; ����������� ������ �� �������� ������ ���������� ControlType, ����� ������������ �������� ��������� ����� � WindowClassBase

//��� ����� �������� ���������������, ��� ���������� ���� ������� ��������, � ��� ���������� ������ ������ ������ ����� �������� ��������� ������� 
//���������� �����������

//� ��������� ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>, � ������������ � ����:
//�������_�����<DerWndCls>, ControlType - ��� ����� ����������� ������� ���������� ��������������, � ��� ���������� ��������� ��� ����������� �����������
//�������

template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WndClsDerivedTemplateClass : public WindowClassControlBaseTemplate<WndClsDerivedTemplateClass<DerWndCls>, ControlType>		//������ ����� ����� �� ������ ����������� ������������
{
protected:
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	//����������� ��� ������������� ������ �� ���������
	WndClsDerivedTemplateClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WndClsDerivedTemplateClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);
	WndClsDerivedTemplateClass(WndClsDerivedTemplateClass&);			//����������� �����������

	virtual ~WndClsDerivedTemplateClass() override;		//����������� ����������

	virtual void OnCreate(HWND hWnd) override;				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd) override;				//������������ ��������� WM_PAINT ������ ������� ���������
	virtual void OnDestroy(HWND hWnd) override;				//������������ ��������� WM_DESTROY ������ ������� ���������
};

//������ ��������� ����� �� ������ ���������������
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNext : public WndClsDerivedTemplateClass<WindowClassDerivedTemplateNext<DerWndCls>, ControlType>		//������ ����� ����� �� ������ ����������� ������������
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedTemplateNext(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(hInstance, szClassName, szWndTitle) { usiWndNum++; }
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedTemplateNext(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(wc, szWndTitle) { usiWndNum++; }

	virtual ~WindowClassDerivedTemplateNext() override		//����������� ����������
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
		DrawText(hDC, TEXT("��������� ��������������� ����� (��������� ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//������ ��������� ����� �� ������ �������

template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNext2 : public WindowClassDerivedTemplateNext<WindowClassDerivedTemplateNext2<DerWndCls>>		//������ ����� ����� �� ������ ����������� ������������
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedTemplateNext2(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedTemplateNext(hInstance, szClassName, szWndTitle) {}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedTemplateNext2(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassDerivedTemplateNext(wc, szWndTitle) {}
	virtual ~WindowClassDerivedTemplateNext2() override		//����������� ����������
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
		DrawText(hDC, TEXT("��������� ��������������� ����� (������ ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//������ ��������� �����, �� ��� ��������� ControlType
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WindowClassDerivedTemplateNextAlt : public WndClsDerivedTemplateClass<WindowClassDerivedTemplateNextAlt<DerWndCls>>		//������ ����� ����� �� ������ ����������� ������������
{
public:
	//����������� ��� ������������� ������ �� ���������
	WindowClassDerivedTemplateNextAlt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(hInstance, szClassName, szWndTitle) { }
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerivedTemplateNextAlt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WndClsDerivedTemplateClass(wc, szWndTitle) {}
	virtual ~WindowClassDerivedTemplateNextAlt() override		//����������� ����������
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
		DrawText(hDC, TEXT("��������� ��������������� ����� (��������� ������������ - �������������� �����)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

//������ ��������� ����� - ������ �����
template<class DerWndCls = thisclass, class ControlType = std::conditional_t<std::is_same<DerWndCls, thisclass>::value, thisclass, DerWndCls>> class WndClsDerivedTemplateClassAlt : public WindowClassControlBaseTemplate<WndClsDerivedTemplateClassAlt<DerWndCls>, ControlType>		//������ ����� ����� �� ������ ����������� ������������
{
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	//����������� ��� ������������� ������ �� ���������
	WndClsDerivedTemplateClassAlt(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr) : WindowClassControlBaseTemplate(hInstance, szClassName, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WndClsDerivedTemplateClassAlt(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr) : WindowClassControlBaseTemplate(wc, szWndTitle)
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}
	WndClsDerivedTemplateClassAlt(WndClsDerivedTemplateClassAlt& wcObj) : WindowClassControlBaseTemplate(wcObj)			//����������� �����������
	{
		usiWndNum++;		//����������� ���������� �������� ������� ������
	}

	virtual ~WndClsDerivedTemplateClassAlt() override		//����������� ����������
	{
		if (hWnd)
			this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
	}

	virtual void OnCreate(HWND hWnd) override				//������������ ��������� WM_CREATE ������ ������� ���������
	{
		//������������ ��������� WM_CREATE ������ ������� ���������
		SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 200, 200)));
	}
	virtual void OnPaint(HWND hWnd) override				//������������ ��������� WM_PAINT ������ ������� ���������
	{
		//������������ ��������� WM_PAINT ������ ������� ���������
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;

		hDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("��������� ��������������� ����� (��������� ������������ - ������ �����)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hWnd, &ps);
	}
	virtual void OnDestroy(HWND hWnd) override				//������������ ��������� WM_DESTROY ������ ������� ���������
	{
		//������������ ��������� WM_DESTROY ������ ������� ���������	
		usiWndNum--;		//��������� ���������� �������� ������� ������
		WndClsDerivedTemplateClassAlt::hWnd = NULL;
		if (usiWndNum == 0)
			PostQuitMessage(0);
	}
};

//���������� ��������� ������� ������������� ������������

//������������� ����������� ������
template<class DerWndCls, class ControlType> unsigned short int WndClsDerivedTemplateClass<DerWndCls, ControlType>::usiWndNum = 0;		//���������� �������� ���������������� ������
template<class DerWndCls, class ControlType> unsigned short int WndClsDerivedTemplateClassAlt<DerWndCls, ControlType>::usiWndNum = 0;		//���������� �������� ���������������� ������

//�������-����� ������

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle) : WindowClassControlBaseTemplate(hInstance, szClassName, szWndTitle)
{
	usiWndNum++;		//����������� ���������� �������� ������� ������
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(WNDCLASSEX& wc, const TCHAR *szWndTitle) : WindowClassControlBaseTemplate(wc, szWndTitle)
{
	usiWndNum++;		//����������� ���������� �������� ������� ������
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::WndClsDerivedTemplateClass(WndClsDerivedTemplateClass& wcObj) : WindowClassControlBaseTemplate(wcObj)
{
	usiWndNum++;		//����������� ���������� �������� ������� ������
}

template<class DerWndCls, class ControlType> WndClsDerivedTemplateClass<DerWndCls, ControlType>::~WndClsDerivedTemplateClass()
{
	if (hWnd)
		this->OnClose(hWnd);		//��������� ����, ��������� �������� ����������� �������
}

template<class DerWndCls, class ControlType> void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnCreate(HWND hWnd)
{
	//������������ ��������� WM_CREATE ������ ������� ���������
	SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(100, 160, 255)));
}

template<class DerWndCls, class ControlType > void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnPaint(HWND hWnd)
{
	//������������ ��������� WM_PAINT ������ ������� ���������
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("��������� ��������������� ����� (��������� ������������)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

template<class DerWndCls, class ControlType> void WndClsDerivedTemplateClass<DerWndCls, ControlType>::OnDestroy(HWND hWnd)
{
	//������������ ��������� WM_DESTROY ������ ������� ���������	
	usiWndNum--;		//��������� ���������� �������� ������� ������
	WndClsDerivedTemplateClass::hWnd = NULL;
	if (usiWndNum == 0)
		PostQuitMessage(0);
}