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
#include <Windows.h>			//�������� ���������� ���� Windows

//���������� �������

template<class WndCls> class List;				//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClass
template<class WndCls> class WindowClass;		//����� ���� Windows

//���������

template<class WndCls> struct ListElement				//���� ������
{
	//������ ����
	HWND hWnd;					//���������� ���� Windows
	WindowClass<WndCls> *p_wndClass;	//��������� �� ������ ������ WindowClass

	ListElement *pNext;			//��������� �� ��������� ������� ������
	ListElement *pPrev;			//��������� �� ���������� ������� ������
};

//������

template<class WndCls> class WindowClass				//����� ���� Windows
{
	using WndProcCallback = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);		//��� ������� ������� ���������

protected:						//��������� ��� ����������� �������!
	//������
	HWND hWnd = NULL;								//���������� ������ ����	
	WNDCLASSEX wc = { 0 };							//��������� ��� ����������� ������ ���� ������ Windows
	const TCHAR *szWndTitle = nullptr;				//��������� ����

	static const TCHAR *szWndTitleDefault;			//������ ��������� �� ���������
	static List<WndCls> wndList;					//����������� ������, ������ ��� ���� �������

	//�������	
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//�������������� � ������ ���� (���������� �� �������������)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//������� ��������� (����������� �������)

	template<class T, typename = T::OnCreate> void LaunchOnCreate(HWND hWnd, T *p_wndClass)		//������! ��. ������ FirstWin32CPP_DerivedTemplate2
	{
		//��������� ������ OnCreate ��� ������ WndCls, ���� OnCreate ���������� � ��
		T::OnCreate(hWnd);
	}

	template<class T> void LaunchOnCreate(HWND hWnd, T *p_wndClass)		//��������� ������ OnCreate � ������� ��������� ����������� ������� �� ��������� �� �����
	{
		p_wndClass->OnCreate(hWnd);			//������ � ������� ��������� ����������� �������
	}

	void OnCreate(HWND hWnd);				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd);				//������������ ��������� WM_PAINT ������ ������� ���������	
	virtual void OnClose(HWND hWnd);				//������������ ��������� WM_CLOSE ������ ������� ���������
	virtual void OnDestroy(HWND hWnd);				//������������ ��������� WM_DESTROY ������ ������� ���������

	//����������������� ������
	friend List<WndCls>;

public:
	//�������
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//����������� ��� ������������� ������ �� ���������
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClass(WindowClass&);			//����������� �����������
	
	virtual ~WindowClass();		//����������� ����������
};

template<class WndCls> class List						//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClass
{
	using lstElement = ListElement<WndCls>;				//��������� ��� �������� ������, ������������������� ����� ������ ����

	lstElement *pFirst = nullptr;		//C++ 11: ������������� ����� ������
	lstElement *pLast = nullptr;		//C++ 11: ������������� ����� ������

	void ListInitialization(WindowClass<WndCls> *p_wndClass)
	{
		if (!pFirst)
		{
			pFirst = pLast = new lstElement;			//��������� ������ � ���� ����������, ����� ������ � �.�. � ���� �������, �.�. ������ �������	
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
	List(WindowClass<WndCls> *p_wndClass = nullptr)		//�����������
	{
		if (p_wndClass != nullptr)
			ListInitialization(p_wndClass);		//�������� ������������� ������
	}
	~List()										//����������
	{
		//������� ��� �������� ������
		lstElement *pCurr = pFirst;
		lstElement *pNext = nullptr;
		while (pCurr != nullptr)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
		}
	}

	lstElement * add(WindowClass<WndCls> *p_wndClass)			//��������� ����� ������� � ����� ������
	{
		if (pLast != nullptr && p_wndClass != nullptr)
		{
			lstElement *pCurr = new lstElement;
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
				ListInitialization(p_wndClass);		//�������� ������������� ������
				return pFirst;
			}
			else
				return nullptr;
		}
	}

	lstElement * search(HWND hWnd)				//��������� ����� �� ��������� hWnd ������ ������
	{
		lstElement *pCurr = pFirst;
		while (pCurr != nullptr)
		{
			if (pCurr->hWnd == hWnd)
				break;
			else
				pCurr = pCurr->pNext;
		}
		if (pCurr != nullptr && pCurr->hWnd != hWnd)			//������������� �������, ��� �� ����� ������ ����
			pCurr = nullptr;
		return pCurr;
	}


	bool del(WindowClass<WndCls> *p_wndClass)		//������� ���� ������ �� ��������� WindowClass*
	{
		if (p_wndClass == nullptr)
			return false;

		//���� ����, ���������� ������ ���������
		lstElement *le = search(p_wndClass->hWnd);
		if (le == nullptr)
			return false;

		//����� ����, ��������� ���������� � ��������� �������� ������� ������ ����������
		lstElement *pPrev = le->pPrev;
		lstElement *pNext = le->pNext;
		if (pPrev == nullptr)		//������� ����� ������ ������� ������		
			pFirst = pNext;
		if (pNext == nullptr)		//������� ��������� ������� ������
			pLast = pPrev;

		if(pPrev != nullptr && pPrev->pNext != nullptr)
			pPrev->pNext = pNext;
		if(pNext != nullptr && pNext->pPrev != nullptr)
			pNext->pPrev = pPrev;

		//������� ������� ����
		delete le;
		return true;
	}
};

//���������� ������� ������

//������������� ����������� ������
template<class WndCls> List<WndCls> WindowClass<WndCls>::wndList;						//������������� ������������ ������� ������
template<class WndCls> const TCHAR *WindowClass<WndCls>::szWndTitleDefault = TEXT("Window");		//������ ��������� �� ���������

//�������-����� ������

template<class WndCls> bool WindowClass<WndCls>::CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister, const TCHAR *szWndTitle)
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

template<class WndCls> WindowClass<WndCls>::WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� ����������� �� ���������
	//�� ����: hInstance - ��������� ����������, szClassName - ��� ������ ���� (������ Windows), szWndTitle - ������ ��������� ����,
	//�������� WndCls ������ ������ ��� ������ ��������� �������

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

template<class WndCls> WindowClass<WndCls>::WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� �����������, ����������� ����� wc
	//�� ����: wc - ������ �� ��������� ������ ���� ��� ����������� ������ Windows, szWndTitle - ������ ��������� ����,
	//�������� WndCls ������ ������ ��� ������ ��������� �������

	WindowClass::wc = wc;
	WindowClass::wc.lpfnWndProc = WndProc;
	WindowClass::szWndTitle = szWndTitle;
	//������ ����
	CreateWnd(WindowClass::wc, false, szWndTitle);
}

template<class WndCls> WindowClass<WndCls>::WindowClass(WindowClass& wcObj)	//����������� �����������
{
	//�������� ������������� ������-����� ������
	hWnd = wcObj.hWnd;
	wc = wcObj.wc;
	szWndTitle = wcObj.szWndTitle;
	CreateWnd(wc, true, szWndTitle);			//���������� ����������� ������, ��� ��� �� ��� ��������������� ��� �������� �������, �� �������� ������������ �����������
}

template<class WndCls> WindowClass<WndCls>::~WindowClass()						//����������
{
	wndList.del((WindowClass<WndCls> *)this);							//������� �� ������
}

template<class WndCls> LRESULT CALLBACK WindowClass<WndCls>::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//������� ���������	
	ListElement<WndCls> * pListElem = nullptr;

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
			p_wndClass->LaunchOnCreate<WndCls>(hWnd, (WndCls *)p_wndClass);
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

template<class WndCls> void WindowClass<WndCls>::OnCreate(HWND hWnd)
{
	//������������ ��������� WM_CREATE ������ ������� ���������
}

template<class WndCls> void WindowClass<WndCls>::OnPaint(HWND hWnd)
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

template<class WndCls> void WindowClass<WndCls>::OnClose(HWND hWnd)
{
	//������������ ��������� WM_CLOSE ������ ������� ���������
	DestroyWindow(hWnd);
}

template<class WndCls> void WindowClass<WndCls>::OnDestroy(HWND hWnd)
{
	//������������ ��������� WM_DESTROY ������ ������� ���������
	PostQuitMessage(0);
}