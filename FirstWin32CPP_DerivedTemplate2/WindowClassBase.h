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
#include <type_traits>			//enable_if � ������ ���������� �������� � ������ ������� ����������
#include <Windows.h>			//�������� ���������� ���� Windows

//���������� �������

class List;						//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClassBase
class WindowClassBase;			//����� ���� Windows (������ ���������� ������)

//���������

struct ListElement				//���� ������
{
	//������ ����
	HWND hWnd;					//���������� ���� Windows
	WindowClassBase *p_wndClass;	//��������� �� ������ ������ WindowClassBase

	ListElement *pNext;			//��������� �� ��������� ������� ������
	ListElement *pPrev;			//��������� �� ���������� ������� ������
};

template<class T> struct has_OnCreate		//��������� ������� �������-����� OnCreate � ������ T ����������� SFINAE: https://habrahabr.ru/post/205772/
{
	static int detect(...);						//������� � ������������ ����������� ������������ ����������, �������� ��� �������� ��������� �������� (� ��� ������ ����������  �� ������������� ���� ����������� �������-�����)
	template<class U> static decltype(std::declval<U>().OnCreate(NULL)) detect(const U&);	//�������-��������, �������������, ���� ����� U ����� ������ �������-����

	static constexpr bool value = std::is_same<void, decltype(detect(std::declval<T>()))>::value;		//���� ������� OnCreate (��. ������� ������ CPP 11\SFINAE\HasMember)
};

//������

class WindowClassBase				//����� ���� Windows
{
protected:						//��������� ��� ����������� �������!
	//������
	HWND hWnd = NULL;								//���������� ������ ����	
	WNDCLASSEX wc = { 0 };							//��������� ��� ����������� ������ ���� ������ Windows
	const TCHAR *szWndTitle = nullptr;				//��������� ����
	void *p_drvWndCls;								//��������� �� ����������� �����, ����������� ���� �������� (�.�. ��������� ������-����� ��������� ������
													//�����������, �� ���������� (�� ��������) ��������� ��� ����, �.�. ��������� �� void

	static const TCHAR *szWndTitleDefault;			//������ ��������� �� ���������
	static List wndList;							//����������� ������, ������ ��� ���� �������

	//�������
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//�������������� � ������ ���� (���������� �� �������������)
	
	template<class WndCls> void LaunchOnCreate(HWND hWnd, std::true_type)
	{
		//��������� ������ OnCreate ��� ������ WndCls, ���� OnCreate ���������� � ��
		if (p_drvWndCls)
			(static_cast<WndCls *>(p_drvWndCls))->WndCls::OnCreate(hWnd);
	}

	template<class WndCls> void LaunchOnCreate(HWND hWnd, std::false_type)
	{
		//OnCreate ��� � ������ - ������ �� �������� (��������� �� ����� ����������, ���� ����������� ��������� ����� ����� ���������� ������)
		//���� �� ����� � SFINAE ����� ��� �������������: ����� �������� ������ ������� ���� - ���� OnCreate ����� ���, �� ������ �� ��������������
	}
	
	virtual void OnPaint(HWND hWnd);				//������������ ��������� WM_PAINT ������ ������� ���������	
	virtual void OnClose(HWND hWnd);				//������������ ��������� WM_CLOSE ������ ������� ���������
	virtual void OnDestroy(HWND hWnd);				//������������ ��������� WM_DESTROY ������ ������� ���������

	//����������������� ������ � �������
	friend List;
	template<class WndCls> friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//������� ���������

public:
	//�������
	template<class WndCls> WindowClassBase(WndCls *p_wndClass, HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//����������� ��� ������������� ������ �� ���������
	template<class WndCls> WindowClassBase(WndCls *p_wndClass, WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassBase(WindowClassBase&);			//����������� �����������

	virtual ~WindowClassBase();		//����������� ����������
};

class List						//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClassBase
{
	ListElement *pFirst = nullptr;		//C++ 11: ������������� ����� ������
	ListElement *pLast = nullptr;		//C++ 11: ������������� ����� ������

	void ListInitialization(WindowClassBase *p_wndClass)
	{
		if (!pFirst)
		{
			pFirst = pLast = new ListElement;			//��������� ������ � ���� ����������, ����� ������ � �.�. � ���� �������, �.�. ������ �������	
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
	List(WindowClassBase *p_wndClass = nullptr)		//�����������
	{
		if (p_wndClass != nullptr)
			ListInitialization(p_wndClass);		//�������� ������������� ������
	}
	~List()										//����������
	{
		//������� ��� �������� ������
		ListElement *pCurr = pFirst;
		ListElement *pNext = nullptr;
		while (pCurr != nullptr)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
		}
	}

	ListElement * add(WindowClassBase *p_wndClass)			//��������� ����� ������� � ����� ������
	{
		if (pLast != nullptr && p_wndClass != nullptr)
		{
			ListElement *pCurr = new ListElement;
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

	ListElement * search(HWND hWnd)				//��������� ����� �� ��������� hWnd ������ ������
	{
		ListElement *pCurr = pFirst;
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

	bool del(WindowClassBase *p_wndClass)		//������� ���� ������ �� ��������� WindowClassBase*
	{
		if (p_wndClass == nullptr)
			return false;

		//���� ����, ���������� ������ ���������
		ListElement *le = search(p_wndClass->hWnd);
		if (le == nullptr)
			return false;

		//����� ����, ��������� ���������� � ��������� �������� ������� ������ ����������
		ListElement *pPrev = le->pPrev;
		ListElement *pNext = le->pNext;
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

//���������� ��������� ������� � �������-������ ������ WindowClassBase

template<class WndCls> WindowClassBase::WindowClassBase(WndCls *p_wndClass, HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� ����������� �� ���������
	//�� ����: p_wndClass - ��������� �� ����������� �����, �� ���� �������� ����� ���������� ��� ���������� ������������, hInstance - ��������� ����������,
	//szClassName - ��� ������ ���� (������ Windows), szWndTitle - ������ ��������� ����

	if (hInstance == NULL || szClassName == nullptr)
		return;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc<WndCls>;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	p_drvWndCls = p_wndClass;		//��������� ��������� �� ����������� �����, ����� �������� OnCreate() ����� ������ ��� ��������� ��������� WM_CREATE

	//������ ����
	WindowClassBase::szWndTitle = szWndTitle;
	CreateWnd(wc, false, szWndTitle);
}

template<class WndCls> WindowClassBase::WindowClassBase(WndCls *p_wndClass, WNDCLASSEX& wc, const TCHAR *szWndTitle)
{
	//������ ����, ������������� ��� �����������, ����������� ����� wc
	//�� ����: p_wndClass - ��������� �� ����������� �����, �� ���� �������� ����� ���������� ��� ���������� ������������, wc - ������ �� ��������� ������
	//���� ��� ����������� ������ Windows, szWndTitle - ������ ��������� ����

	WindowClassBase::wc = wc;
	WindowClassBase::wc.lpfnWndProc = WndProc<WndCls>;
	WindowClassBase::szWndTitle = szWndTitle;

	p_drvWndCls = p_wndClass;		//��������� ��������� �� ����������� �����, ����� �������� OnCreate() ����� ������ ��� ��������� ��������� WM_CREATE

	//������ ����
	CreateWnd(WindowClassBase::wc, false, szWndTitle);
}

template<class WndCls> LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//������� ���������	
	ListElement * pListElem = nullptr;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//lParam �������� ��������� �� ��������� ���� CREATESTRUCT, ����������� ������ ����� ������� ��������� �� ������ ������ WindowClassBase, ������� ���
		//����� (��. ������� WindowClassBase::CreateWnd)
		CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
		WindowClassBase *p_wndClass = reinterpret_cast<WindowClassBase *>(cs->lpCreateParams);
		p_wndClass->hWnd = hWnd;		//�������������� hWnd ������� ������ ���������, ���������� � ������� ���������
										//������� ��������� ���� � ������
		pListElem = WindowClassBase::wndList.add(p_wndClass);
		if (pListElem)
			p_wndClass->LaunchOnCreate<WndCls>(hWnd, std::conditional_t<has_OnCreate<WndCls>::value, std::true_type, std::false_type>());
	}
	break;
	case WM_PAINT:
		pListElem = WindowClassBase::wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)
			pListElem->p_wndClass->OnPaint(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	case WM_CLOSE:
		pListElem = WindowClassBase::wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)
			pListElem->p_wndClass->OnClose(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	case WM_DESTROY:
		pListElem = WindowClassBase::wndList.search(hWnd);		//���� � ������ ������ ������ �� ��������� ����������� ����
		if (pListElem)
			pListElem->p_wndClass->OnDestroy(hWnd);			//�������� ����������� �������, ��������������� ������� �����������
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}