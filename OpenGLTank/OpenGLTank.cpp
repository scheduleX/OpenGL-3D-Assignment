// OpenGLTank.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "OpenGLTank.h"
#include <time.h>

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

	srand(time(NULL));
	Texture::InitialiseAllGlobal();

	Clock::GlobalClock.Start();

	PlayerTank = new Tank;
	PlayerTank->BuildTree();

	return 1;
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
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
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
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			// Get the left mouse press coordinates
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
		}
		break;

		case WM_MOUSEMOVE:
		{
			int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
			convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosObject, yPosObject);
		}
		break;

		case WM_KEYDOWN:
		{
			PlayerTank->HandleKeyDown(wParam);
		}
		break;

		case WM_KEYUP:
		{
			PlayerTank->HandleKeyUp(wParam);
		}

		case WM_TIMER:
		{
			//Handle Timer Events
		}
		break;

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	}
	return 0;
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

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos)
{
	//Normalise pixels
	float normalisexWinPos = xWinPos * 2.0 / width;
	float normaliseyWinPos = (yWinPos + 60) * 2.0 / height;
	xOpenGLPos = normalisexWinPos > 1 ? normalisexWinPos - 1 : 0 - (1 - normalisexWinPos);
	yOpenGLPos = normaliseyWinPos > 1 ? (0 - (1 - normaliseyWinPos)) * -1 : (normaliseyWinPos - 1) * -1;
}

#include "Environment.h"

void DrawGLScene()
{
	Clock::GlobalClock.Refresh();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White
							  // Locate camera in the (1,1,1) position then look at
							  // origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);

	float speed = 1;
	float distance = 15;

	float relativeX = -sin(xPosObject * speed) * distance + PlayerTank->Position.X;
	float relativeY = sin(yPosObject * speed) * distance;
	float relativeZ = -cos(xPosObject * speed) * cos(yPosObject * speed) * distance + PlayerTank->Position.Z;

	relativeY = relativeY < 3 ? 3 : relativeY;
	gluLookAt
	(
		relativeX, relativeY, relativeZ, // Camera's position
		PlayerTank->Position.X, 0, PlayerTank->Position.Z,						 // Camera's target to look at
		0, 1, 0							 // Orientation of camera
	);
	Draw3D_AxisSystem();

	
	PlayerTank->Draw();
	Environment::Global.Draw();

	glLoadIdentity();

	PlayerTank->Update();
	Debug::Log("Position: " + PlayerTank->TurrentPosition.ToString() + "     Direction: " + PlayerTank->TurrentDirection.ToString());
}