// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define PI 3.14159265359
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <cmath>

// OpenGL
#include <gl\GL.h>
#include <gl\GLU.h>

// Project Classes
#include "Debug.h"
#include "Clock.h"
#include "ColorInt.h"
#include "Texture.h"
#include "Vector3f.h"
#include "Tank.h"
#include "Missile.h"

using namespace std;

typedef struct materialStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
} Material;

static void SetMaterial(Material * m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
}

static Material yellowPlasticMaterial =
{
	{ 0.4, 0.4, 0.0, 1.0 }, // Ambient
	{ 0.9, 0.9, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	8						// Shinesss
};

static Material redPlasticMaterial =
{
	{ 0.4, 0.0, 0.0, 1.0 }, // Ambient
	{ 0.9, 0.0, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	32						// Shininess
};

static Material greenPlasticMaterial =
{
	{ 0.0, 0.4, 0.0, 1.0 }, // Ambient
	{ 0.0, 0.9, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	8						// Shininess
};

static Material darkGreenMaterial =
{
	{ 0.0, 0.2, 0.0, 1.0 }, // Ambient
	{ 0.0, 0.5, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	8						// Shininess
};