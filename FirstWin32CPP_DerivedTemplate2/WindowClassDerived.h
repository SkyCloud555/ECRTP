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

class WindowClassDerived : public WindowClassTemplate<WindowClassDerived>		//���������� ������ ������ � ������ ������� ������ �� ������ �������
{
	static unsigned short int usiWndNum;			//���������� �������� ������

public:
	WindowClassDerived(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//����������� ��� ������������� ������ �� ���������
	WindowClassDerived(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//�����������, ����������� ������ �� ��������� ���� WNDCLASSEX ��� ����������� ���� � �����������, ��������� �� �� ���������
	WindowClassDerived(WindowClassDerived&);			//����������� �����������

	virtual ~WindowClassDerived() override;		//����������� ����������

	virtual void OnCreate(HWND hWnd) override;				//������������ ��������� WM_CREATE ������ ������� ���������
	virtual void OnPaint(HWND hWnd) override;				//������������ ��������� WM_PAINT ������ ������� ���������
	virtual void OnDestroy(HWND hWnd) override;				//������������ ��������� WM_DESTROY ������ ������� ���������
};