#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "renderer.h"
#include <string>
#include <unordered_map>

///////////////////////////////////////////////////////////////////////////////
// Loader class
///////////////////////////////////////////////////////////////////////////////
class Loader
{
	//=========================================================================
	// list of friend classes and methods.
	//=========================================================================
	/* NOTHING */

private:
	//=========================================================================
	// private variables.
	//=========================================================================
	static std::unordered_map<std::string, MODEL*> s_ModelPool;
	//=========================================================================
	// private methods
	//=========================================================================
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);
	static void LoadModel(const char* FileName, MODEL* Model);
public:
	//=========================================================================
	// public variables.
	//=========================================================================
	/* NOTHING */

	//=========================================================================
	// public methods
	//=========================================================================
	static void PreLoad3DModel(const char* FileName);
	static void Load3DModel(const char* FileName, MODEL** Model);
	static void UnloadAll();
};

