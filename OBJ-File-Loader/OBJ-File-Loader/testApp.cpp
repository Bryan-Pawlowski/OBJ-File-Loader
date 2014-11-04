// testApp.cpp : Defines the entry point for the console application.
//

// Essentially,this file will be updated as I finish writing the moving parts of the actual obj loader.

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <iostream>
#include <memory>
#include <windows.h>
#include <windowsx.h>
#include <sstream>
#include <string>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include "OBJ-loader.h"

using namespace std;

//globals
IDXGISwapChain *swapchain;
ID3D11Device *dev;
ID3D11DeviceContext *devcon;
ID3D11RenderTargetView *backbuffer;
ID3D11VertexShader * pVS;
ID3D11PixelShader * pPS;
ID3D11Buffer *pVBuffer;
ID3D11InputLayout *pLayout;
ID3D11Buffer *pCBuffer;
ID3D11Buffer *pIBuffer;
ID3D11Buffer *pVBuffer2;

bool modelTest = false;
unsigned int modelSize;

//prototypes
void createModelTests(void);
int testDX( HINSTANCE hInstance, int nCmdShow );
void InitD3D(HWND hWnd);
void CleanD3D(void);
void RenderFrame(void);
void InitPipeline(void);
void InitGraphics(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600


struct CBUFFER
{
	D3DXMATRIX Final;
	D3DXMATRIX Rotation;
	D3DXVECTOR4 LightVector;
	D3DXCOLOR LightColor;
	D3DXCOLOR AmbientColor;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*----------Test that we can create a model----------*/
	createModelTests();
	/*-----------We can now create a model---------------*/


	/*----------Test use of model in DirectX-------------*/

	testDX(hInstance, nCmdShow);

	/*If we make it this far, we know that DX works, especially if we see a proper image of our loaded model.*/

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

	InitD3D( hWnd );
	InitGraphics();
	InitPipeline();


	ShowWindow(hWnd, nCmdShow);

	MSG msg = { 0 };

	while(TRUE)
	{	

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) break;
		}
		else RenderFrame();
	}

	CleanD3D();

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
		case WM_CHAR:
		{
			switch (wParam)
			{
				case 's':
					modelTest = !modelTest;
					break;
				default:
					break;
			}
		}
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

void createModelTests(void)
{
	//test for correct failure
	Model *testModel = new Model;
	int fail = testModel->modelInit("null.obj");
	if (!fail){
		MessageBox(HWND_DESKTOP, L"Self-deletion on fail test failed!", L"Test1", MB_OK);
		delete testModel;
		exit(EXIT_FAILURE);
	}
	delete testModel;

	//test for open success
	testModel = new Model;
	int res = testModel->modelInit("cube.obj");
	if (res == 1){
		MessageBox(HWND_DESKTOP, L"Could not find or open the cube.obj test file.", L"Test2", MB_OK);
		delete testModel;
		exit(EXIT_FAILURE);
	}
	delete testModel;
}

void InitD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);
}

void CleanD3D(void) //Release all of our COM objects.
{
	swapchain->SetFullscreenState(FALSE, NULL);

	if (swapchain) swapchain->Release();
	if (dev) dev->Release();
	if (devcon) devcon->Release();
	if (backbuffer) backbuffer->Release();
	if (pPS) pPS->Release();
	if (pVS) pVS->Release();
	if (pCBuffer) pCBuffer->Release();
	if (pLayout) pLayout->Release();
	if (pVBuffer) pVBuffer->Release();
	if (pVBuffer2) pVBuffer2->Release();
	if (pIBuffer) pIBuffer->Release();
}

void RenderFrame(void)
{

	CBUFFER cBuffer;
	D3DXMATRIX matRotate, matView, matProjection, matFinal;

	static float Time = 0.0f;
	Time += 0.0003f;

	cBuffer.LightVector = D3DXVECTOR4(0.75f, 0.5f, 1.0f, 0.0f);
	cBuffer.LightColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	cBuffer.AmbientColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	//world matrix
	D3DXMatrixRotationY(&matRotate, Time);

	// create a view matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 3.0f, 5.0f),   // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

	// create a projection matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		(FLOAT)D3DXToRadian(45),                    // field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,                                       // near view-plane
		100.0f);

	matFinal = matRotate * matView * matProjection;
	cBuffer.Final = matFinal;
	cBuffer.Rotation = matRotate;


	devcon->VSSetConstantBuffers(0, 1, &pCBuffer);

	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	if (!modelTest) {

		devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
		devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);
		devcon->DrawIndexed(36, 0, 0);
	}
	else {
		devcon->IASetVertexBuffers(0, 1, &pVBuffer2, &stride, &offset);
		
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);
		devcon->Draw(modelSize, 0);
	}

	swapchain->Present(0, 0);
}

void InitPipeline(void)
{
	ID3D10Blob *VS, *PS, *VErrors, *PErrors;
	HRESULT res = D3DX11CompileFromFile(L"VShader.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, &VErrors, 0);
	if (res)
	{
		char *buff = (char *)VErrors->GetBufferPointer();
		wchar_t wtext[1000], wtext2[1000];
		mbstowcs(wtext, buff, strlen(buff) + 1);
		LPCWSTR myString = wtext;
		MessageBox(HWND_DESKTOP, myString, L"Vertex Shader Error!", MB_OK);
		CleanD3D();
		exit(EXIT_FAILURE);
	}
	res = D3DX11CompileFromFile(L"PShader.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, &PErrors, 0);
	if (res)
	{
		char *buff = (char *)PErrors->GetBufferPointer();
		wchar_t wtext[1000], wtext2[1000];
		mbstowcs(wtext, buff, strlen(buff) + 1);
		LPCWSTR myString = wtext;
		MessageBox(HWND_DESKTOP, myString, L"Pixel Shader Error!", MB_OK);
		CleanD3D();
		exit(EXIT_FAILURE);
	}

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);


	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	res = dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	if (res){
		MessageBox(HWND_DESKTOP, L"Problem with setting input layout!", L"Input Layout Error!", MB_OK);
		CleanD3D();
		exit(EXIT_FAILURE);
	}
	devcon->IASetInputLayout(pLayout);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBUFFER);				//This must ALWAYS be a multiple of 16. will throw an error if not.
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT buffRes = dev->CreateBuffer(&bd, NULL, &pCBuffer);

	if (buffRes)
	{
		MessageBox(HWND_DESKTOP, L"Problem creating constant buffer (size most likely not multiple of 16)!", L"CBuffer not created.", MB_OK);
		CleanD3D();
		exit(EXIT_FAILURE);
	}

}

void InitGraphics(void)
{
	//SET UP AND LOAD GEOMETRY HERE

	//test triangle
	VERTEX ourVerts[] = 
	{	//Side1 of test cube
		{ D3DXVECTOR4(-1.0f, -1.0, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		{ D3DXVECTOR4(1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		{ D3DXVECTOR4(-1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		{ D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		
		//Side2 of test cube
		{ D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f)},
		{ D3DXVECTOR4(-1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 0.0f) },

		//Side3 of test cube
		{ D3DXVECTOR4(-1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
	
		//Side4 of test cube
		{ D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },

		//Side5 of test cube
		{ D3DXVECTOR4(1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },

		//Side6 of test cube
		{ D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bd, NULL, &pVBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	//map our buffer to ms
	memcpy(ms.pData, ourVerts, sizeof(ourVerts));
	devcon->Unmap(pVBuffer, NULL);

	//END SET UP AND LOAD OF GEOMETRY


	//TELL GPU HOW VERTICES WILL BE INPUT

	DWORD OurIndices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create the index buffer
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DWORD)* 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	dev->CreateBuffer(&bd, NULL, &pIBuffer);

	devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurIndices, sizeof(OurIndices));                   // copy the data
	devcon->Unmap(pIBuffer, NULL);


	//test cube finished loading.

	//Begin trying to load model.

	Model *cube = new Model;

	int result = cube->modelInit("dog.txt");
	
	if (result)
	{
		MessageBox(HWND_DESKTOP, L"Could not load model!", L"Model Load Error", MB_OK);
		CleanD3D();
		exit(EXIT_FAILURE);
	}

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * cube->getSize();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bd, NULL, &pVBuffer2);

	modelSize = cube->getSize();

	devcon->Map(pVBuffer2, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	//map our buffer to ms
	memcpy(ms.pData, cube->vertices->_Myfirst, bd.ByteWidth);
	devcon->Unmap(pVBuffer2, NULL);

	delete cube;

}