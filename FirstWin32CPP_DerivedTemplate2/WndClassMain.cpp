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

//учебный класс, основанный на Windows API
//главный модуль программы

//включаемые файлы
#include "WindowClassDerived.h"
#include "WindowClassDerived2.h"
#include "WindowClassDerivedVariadic.h"
#include "WindowClassDerived3.h"
#include "WindowClassDerived4.h"
#include "WindowClassDerived5.h"

//главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Tab-отступами отмечены закомментированое создание объектов тех классов, которые не прошли преобразование static_cast внутри WindowClass.h

	//WindowClass w0(hInstance, TEXT("Base windows class"), TEXT("Base windows class"));

	//создаём два окна с параметрами по умолчанию
	//WindowClassDerived w1(hInstance, TEXT("Default C++ Class"), TEXT("Windows API and C++ test program"));
	//WindowClassDerived w2 = w1;

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
	wc->hCursor = LoadCursor(NULL, IDC_NO);
	wc->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc->lpszMenuName = NULL;
	wc->lpszClassName = szClassName;
	wc->hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//WindowClassDerived w3(*wc);			//создаём окно

	//виртуальная функция WindowClassDerivedNext::OnPaint будет вызвана, однако WindowClassDerivedNext::OnCreate вызвана не будет - информация о типе
	//WindowClassDerivedNext не будет передана в шаблон для WindowClassBase
	//WindowClassDerivedNext w4(hInstance, TEXT("DerivedNext"), TEXT("DerivedNext"));

		WndClsDerivedTemplateClass<> w5(hInstance, TEXT("DerivedTemplate"), TEXT("DerivedTemplate"));
		//WndClsDerivedTemplateClass<> w6 = w5;

		//WindowClassDerivedTemplateNext<> w7(hInstance, TEXT("DerivedTemplateNext"), TEXT("DerivedTemplateNext"));
		//WindowClassDerivedTemplateNext<> w8 = w7;

		//WindowClassDerivedTemplateNext2<> w9(hInstance, TEXT("DerivedTemplateNext2"), TEXT("DerivedTemplateNext2"));
		//WindowClassDerivedTemplateNext2<> w10 = w9;

		//WindowClassDerivedTemplateNextAlt<> w11(hInstance, TEXT("DerivedTemplateNextAlt"), TEXT("DerivedTemplateNextAlt"));

	//WndClsDerivedTemplateClassAlt<> w13(hInstance, TEXT("DerivedTemplateAlt"), TEXT("DerivedTemplateAlt"));

	WindowClassVariadic1<> w14(hInstance, TEXT("DerivedVariadicTemplate1"), TEXT("DerivedVariadicTemplate1"));
	WindowClassVariadic2<> w15(hInstance, TEXT("DerivedVariadicTemplate2"), TEXT("DerivedVariadicTemplate2"));
	WindowClassVariadic3<> w16(hInstance, TEXT("DerivedVariadicTemplate3"), TEXT("DerivedVariadicTemplate3"));
	WindowClassVariadic4<> w17(hInstance, TEXT("DerivedVariadicTemplate4"), TEXT("DerivedVariadicTemplate4"));
	WindowClassVariadic2Alt<> w18(hInstance, TEXT("DerivedVariadicTemplate2Alt"), TEXT("DerivedVariadicTemplate2Alt"));

	//играемся с указателями
	//WindowClassVariadic1<> *p_w15 = &w15;		//не сработает - WindowClassVariadic2<> не наследуется от WindowClassVariadic1<>, и потому не может быть
												//преобразован к этому типу как к базовому
	//WindowClassVariadic1<WindowClassVariadic2<>> *p_w15 = &w15;		//работает: неявное преобразование производного типа к базовому
	//p_w15 = &w14;		//та же ошибка: тип WindowClassVariadic1<WindowClassVariadic2<>> не совпадает с типом WindowClassVariadic1<>
	
	//WindowClassBase *wndArray[5] = {&w14, &w15, &w16, &w17, &w18};	//массив указателей, неявное преобразование всех ветвей наследования к корню дерева иерархии

		//WindowClassDerivedI1<> w19(hInstance, TEXT("DerivedInit1"), TEXT("DerivedInit1"));
		//WindowClassDerivedI2<> w20(hInstance, TEXT("DerivedInit2"), TEXT("DerivedInit2"));
		//WindowClassDerivedI3<> w21(hInstance, TEXT("DerivedInit3"), TEXT("DerivedInit3"));
		//WindowClassDerivedI4<> w22(hInstance, TEXT("DerivedInit4"), TEXT("DerivedInit4"));
		//WindowClassDerivedI2Alt<> w23(hInstance, TEXT("DerivedInit2Alt"), TEXT("DerivedInit2Alt"));
	
	//играемся с указателями
	//WindowClassDerivedI1<> *p_w20 = &w20;		//ошибка - базовый класс w20 есть WindowClassDerivedI1<WindowClassInit2>, а не WindowClassDerivedI1<WindowClassInit1>
	//WindowClassDerivedI1<WindowClassInit2> *p_w20 = &w20;		//всё верно

	WindowClassDerivedAlternative1<> w24(hInstance, TEXT("WindowClassDerivedAlternative1"), TEXT("WindowClassDerivedAlternative1"));
	WindowClassDerivedAlternative2<> w25(hInstance, TEXT("WindowClassDerivedAlternative2"), TEXT("WindowClassDerivedAlternative2"));
	WindowClassDerivedAlternative3<> w26(hInstance, TEXT("WindowClassDerivedAlternative3"), TEXT("WindowClassDerivedAlternative3"));
	WindowClassDerivedAlternative4<> w27(hInstance, TEXT("WindowClassDerivedAlternative4"), TEXT("WindowClassDerivedAlternative4"));

	//WindowClassDerivedSimplest1<> w28(hInstance, TEXT("WindowClassDerivedSimplest1"), TEXT("WindowClassDerivedSimplest1"));
	//WindowClassDerivedSimplest2<> w29(hInstance, TEXT("WindowClassDerivedSimplest2"), TEXT("WindowClassDerivedSimplest2"));
	//WindowClassDerivedSimplest3<> w30(hInstance, TEXT("WindowClassDerivedSimplest3"), TEXT("WindowClassDerivedSimplest3"));
	//WindowClassDerivedSimplest4<> w31(hInstance, TEXT("WindowClassDerivedSimplest4"), TEXT("WindowClassDerivedSimplest4"));

	//играемся с указателями
	//WindowClassDerivedAlternative1<> *p_25 = &w25;		//опять ошибка - несоответствие типа

	delete wc;

	//выполняем цикл обработки сообщений до закрытия приложения
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}