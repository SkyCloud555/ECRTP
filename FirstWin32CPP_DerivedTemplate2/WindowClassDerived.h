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

//включаемые файлы
#include "WindowClass.h"

//классы

class WindowClassDerived : public WindowClassTemplate<WindowClassDerived>		//построение нового класса с другой логикой работы на основе старого
{
	static unsigned short int usiWndNum;			//количество объектов класса

public:
	WindowClassDerived(HINSTANCE hInstance, const TCHAR *szClassName, const TCHAR *szWndTitle = nullptr);		//конструктор для инициализации класса по умолчанию
	WindowClassDerived(WNDCLASSEX& wc, const TCHAR *szWndTitle = nullptr);	//конструктор, принимающий ссылку на структуру типа WNDCLASSEX для регистрации окна с настройками, отличными от по умолчанию
	WindowClassDerived(WindowClassDerived&);			//конструктор копирования

	virtual ~WindowClassDerived() override;		//виртуальный деструктор

	virtual void OnCreate(HWND hWnd) override;				//обеспечивает обработку WM_CREATE внутри оконной процедуры
	virtual void OnPaint(HWND hWnd) override;				//обеспечивает обработку WM_PAINT внутри оконной процедуры
	virtual void OnDestroy(HWND hWnd) override;				//обеспечивает обработку WM_DESTROY внутри оконной процедуры
};