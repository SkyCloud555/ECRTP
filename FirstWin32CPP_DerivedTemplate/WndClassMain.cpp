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
#include "WindowClassDerived.h"

//главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//WindowClass<> w0;			//ошибка! объект базового класса уже не создашь: отсутствует параметр шаблона

	//создаём два окна с параметрами по умолчанию
	WindowClassDerived w1(hInstance, TEXT("Default C++ Class"), TEXT("Windows API and C++ test program"));
	WindowClassDerived w2 = w1;

	WindowClassDerivedNext w4(hInstance, TEXT("DerivedNext"), TEXT("DerivedNext"));
	
	TCHAR szClassName[] = TEXT("Class C++");
	MSG msg;
	WNDCLASSEX *wc;

	//создаём окно с нестандартными параметрами
	if (!(wc = new WNDCLASSEX))
	{
		MessageBox(NULL, TEXT("Ошибка выделения памяти!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return 0;
	}

	wc->cbSize = sizeof(WNDCLASSEX);
	wc->style = CS_HREDRAW | CS_VREDRAW;
	wc->lpfnWndProc = NULL;				//будет инициализирована внутри класса
	wc->cbClsExtra = 0;
	wc->cbWndExtra = 0;
	wc->hInstance = hInstance;
	wc->hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc->hCursor = LoadCursor(NULL, IDC_CROSS);
	wc->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc->lpszMenuName = NULL;
	wc->lpszClassName = szClassName;
	wc->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//WindowClassDerived w3(*wc);			//создаём окно

	delete wc;

	//выполняем цикл обработки сообщений до закрытия приложения
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}