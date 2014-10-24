// testApp.cpp : Defines the entry point for the console application.
//

// Essentially,this file will be updated as I finish writing the moving parts of the actual obj loader.

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include "OBJ-loader.h"

using namespace std;

void createModelTests(void);
int testDX(HINSTANCE hInstance, int nCmdShow, );
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*----------Test that we can create a model----------*/
	createModelTests();
	/*-----------We can now create a model---------------*/


	/*----------Test use of model in DirectX-------------*/





	return 0;
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