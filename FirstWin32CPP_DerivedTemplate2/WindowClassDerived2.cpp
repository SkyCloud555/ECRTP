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

//включаемые файлы
#include "WindowClassDerived2.h"						//описание класса окна Windows (переопределённый)

//класс WindowClassDerivedNext

//функции-члены класса

void WindowClassDerivedNext::OnCreate(HWND hWnd)
{
	//обеспечивает обработку WM_CREATE внутри оконной процедуры
	SetClassLongPtr(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(100, 160, 255)));
}

void WindowClassDerivedNext::OnPaint(HWND hWnd)
{
	//обеспечивает обработку WM_PAINT внутри оконной процедуры
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	hDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	DrawText(hDC, TEXT("Переопределённый класс (ДВАЖДЫ единичное наследование)."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}
