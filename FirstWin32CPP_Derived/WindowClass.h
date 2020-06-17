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

class List;						//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClass
class WindowClass;				//����� ���� Windows

//���������

struct ListElement				//���� ������
{
	//������ ����
	HWND hWnd;					//���������� ���� Windows
	WindowClass *p_wndClass;	//��������� �� ������ ������ WindowClass

	ListElement *pNext;			//��������� �� ��������� ������� ������
	ListElement *pPrev;			//��������� �� ���������� ������� ������
};

//������

class WindowClass				//����� ���� Windows
{
protected:						//��������� ��� ����������� �������!
	//������
	HWND hWnd = NULL;								//���������� ������ ����	
	WNDCLASSEX wc = { 0 };							//��������� ��� ����������� ������ ���� ������ Windows
	const TCHAR *szWndTitle = nullptr;					//��������� ����

	static const TCHAR *szWndTitleDefault;			//������ ��������� �� ���������
	static List wndList;							//����������� ������, ������ ��� ���� �������

	//�������
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		//������� ��������� (����������� �������)
	bool CreateWnd(WNDCLASSEX& wc, bool bSkipClassRegister = false, const TCHAR *szWndTitle = nullptr);		//�������������� � ������ ���� (���������� �� �������������)
	
	virtual void OnCreate(HWND hWnd);				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd);				//������������ ��������� WM_PAINT ������ ������� ���������	
	virtual void OnClose(HWND hWnd);				//������������ ��������� WM_CLOSE ������ ������� ���������
	virtual void OnDestroy(HWND hWnd);				//������������ ��������� WM_DESTROY ������ ������� ���������

	//����������������� ������
	friend List;

public:
	//�������
	WindowClass(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//����������� ��� ������������� ������ �� ���������
	WindowClass(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClass(WindowClass&);			//����������� �����������

	virtual ~WindowClass();		//����������� ����������
};

class List						//�����, ���������� �� ���������������� ������ ������������ ���� � ���������� �� WindowClass
{
	ListElement *pFirst = nullptr;		//C++ 11: ������������� ����� ������
	ListElement *pLast = nullptr;		//C++ 11: ������������� ����� ������

	void ListInitialization(WindowClass *p_wndClass)
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
	List(WindowClass *p_wndClass = nullptr)		//�����������
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

	ListElement * add(WindowClass *p_wndClass)			//��������� ����� ������� � ����� ������
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


	bool del(WindowClass *p_wndClass)		//������� ���� ������ �� ��������� WindowClass*
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