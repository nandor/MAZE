// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#	ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#	endif
#else
int main(int argc, char **argv)
{
#endif

	Engine *engine = NULL;
		
	try 
	{
		new Log("engine.log");
		engine = new Engine();

		engine->LoadConfig("config.cfg");
		engine->Init();
		engine->MainLoop();

		delete engine;
	} 
	catch (std::exception& e) 
	{
		try
		{
			if (engine != NULL) 
			{
				delete engine;
				engine = NULL;
			}
		
			MessageBox(NULL, e.what(), "Unhandled exception", MB_ICONERROR);
			return -1;
		}
		catch (std::exception& e)
		{
			MessageBox(NULL, e.what(), "Unhandled exception", MB_ICONERROR);
			return -1;
		}
	}

	return 0;
}
