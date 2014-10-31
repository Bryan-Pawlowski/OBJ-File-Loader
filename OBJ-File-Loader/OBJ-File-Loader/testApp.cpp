// testApp.cpp : Defines the entry point for the console application.
//

// Essentially,this file will be updated as I finish writing the moving parts of the actual obj loader.

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <sstream>
#include <string>
#include "OBJ-loader.h"

using namespace std;

void createModelTests(void);
int testDX( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


#define SCREEN_WIDTH	500
#define SCREEN_HEIGHT	400

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*----------Test that we can create a model----------*/
	createModelTests();
	/*-----------We can now create a model---------------*/


	/*----------Test use of model in DirectX-------------*/

	testDX(hInstance, nCmdShow);


	return 0;
}

int testDX(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// window class struct has the following info

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	//create window and use the result as the handle for our window.

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(	NULL,
							L"WindowClass1",
							L"DX Model Loader Test",
							WS_OVERLAPPEDWINDOW,
							300,
							300,
							wr.right - wr.left,
							wr.bottom - wr.top,
							NULL,
							NULL,
							hInstance,  //handle for our app
							NULL);

	ShowWindow(hWnd, nCmdShow);

	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
				PostQuitMessage(0);
				return 0;
		} break;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

void createModelTests(void)
{
	//test for correct failure
	Model *testModel = new Model;
	int fail = testModel->modelInit("null.obj");
	if (!fail) MessageBox(HWND_DESKTOP, L"Self-deletion on fail test failed!", L"Test1", MB_OK);
	delete testModel;

	//test for open success
	testModel = new Model;
	int res = testModel->modelInit("cube.obj");
	if (res == 1) MessageBox(HWND_DESKTOP, L"Could not find or open the cube.obj test file.", L"Test2", MB_OK);	

	delete testModel;
}