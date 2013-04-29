// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
#include "MZLog.h"
#include "MZEngine.h"
using namespace MAZE;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{ 
	Engine *engine = NULL;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
		
			Log::Inst() << "Exception: " << e.what();
			return -1;
		}
		catch (std::exception& e)
		{
			Log::Inst() << "Exception: " << e.what();
			return -1;
		}
	}

	return 0;
}
