// IGRA_LAB.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "IGRA_LAB.h"

//For IGRA - OPENGL
#include <gl\GL.h>
#include <gl\GLU.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <vector>
#include "Vector3f.h"
#include "Clock.h"
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;					 // current instance
WCHAR szTitle[MAX_LOADSTRING];		 // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

// Added for IGRA
int width = 400;
int height = 400;
HWND hWnd = NULL;
HDC hDC = NULL;
HGLRC hRC = NULL;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
GLuint PixelFormat;

// The position of the jumping circle
float xPosObject = 0.0;
float yPosObject = 0.0;

Clock *clock;

int InitOpenGL();
void DrawGLScene();
void Draw3D_AxisSystem();
void ReSizeGLScene(GLsizei width, GLsizei height);

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos)
{
	//Normalise pixels
	float normalisexWinPos = xWinPos * 2.0 / width;
	float normaliseyWinPos = (yWinPos + 60) * 2.0 / height;
	xOpenGLPos = normalisexWinPos > 1 ? normalisexWinPos - 1 : 0 - (1 - normalisexWinPos);
	yOpenGLPos = normaliseyWinPos > 1 ? (0 - (1 - normaliseyWinPos)) * -1 : (normaliseyWinPos - 1) * -1;
}

void SetupLight()
{
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightDiffuse[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightSpecular[] = {0.5, 0.5, 0.5, 1};
	GLfloat LightPosition[] = {10, 10, 10, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,						 // Version Number
	PFD_DRAW_TO_WINDOW |	 // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
	PFD_TYPE_RGBA,			 // Request An RGBA Format
	16,						 // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0,		 // Color Bits Ignored
	0,						 // No Alpha Buffer
	0,						 // Shift Bit Ignored
	0,						 // No Accumulation Buffer
	0, 0, 0, 0,				 // Accumulation Bits Ignored
	32,						 // 32 Bit Z-Buffer (Depth Buffer)
	0,						 // No Stencil Buffer
	0,						 // No Auxiliary Buffer
	PFD_MAIN_PLANE,			 // Main Drawing Layer
	0,						 // Reserved
	0, 0, 0					 // Layer Masks Ignored
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IGRALAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRALAB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			DrawGLScene();
			SwapBuffers(hDC);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRALAB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IGRALAB);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	/*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
	//Changed for IGRA
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						 760, 340, width, height, nullptr, nullptr, hInstance, nullptr);

	InitOpenGL();

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//Starting a timer
	SetTimer(hWnd, NULL, 0, NULL);
	ReSizeGLScene(width, height);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:

			switch (wmId)
			{
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
		}
		break;

		// Added for IGRA
		case WM_SIZE:
		{
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

		case WM_LBUTTONDOWN:
		{
			// Get the left mouse press coordinates
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
			break;
		}

		case WM_MOUSEMOVE:
		{
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
			convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosObject, yPosObject);
			break;
		}

		case WM_TIMER:
		{
			//Handle Timer Events
			break;
		}
	}
	return 0;
}

void ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0) // Prevent A Divide By Zero By
		height = 1;  // Making Height Equal One
			// Reset The Current Viewport
	glViewport(0, 0, width, height);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

int InitOpenGL()
{
	// Get Device Dontext
	if (!(hDC = GetDC(hWnd)))
	{
		MessageBox(NULL, L"Can't Create A GL Device Context.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
				   MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC)))
	{
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
				   L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC))
	{
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	SetupLight();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	clock = new Clock;
	clock->Start();

	return 1;
}

void DrawGLScene()
{
	double deltatime = clock->DeltaTime();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White
							  // Locate camera in the (1,1,1) position then look at
							  // origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float speed = 1;
	float distance = 15;

	float relativeX = sin(xPosObject * speed) * distance;
	float relativeY = sin(yPosObject * speed) * distance;
	float relativeZ = cos(xPosObject * speed) * cos(yPosObject * speed) * distance;

	gluLookAt
	(
		relativeX, relativeY, relativeZ, // Camera's position
		0, 0, 0,						 // Camera's target to look at
		0, 1, 0							 // Orientation of camera
	);
	Draw3D_AxisSystem();

	glLoadIdentity();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Draw3D_AxisSystem()
{
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(10000, 0, 0);
	// Draw positive y-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10000, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10000);
	glEnd();
}