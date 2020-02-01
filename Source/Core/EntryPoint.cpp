#include "EntryPoint.h"
#include "App/AppBase.h"
#include "Core/Platform/Timer.h"

void EntryImpl(AppBase* app)
{
	Core::Timer timer;
	app->Init(); 
	while (app->Running())
	{
		timer.Start();
		
		app->StartFrame();
		app->Update();
		app->EndFrame();

		app->DeltaTime = timer.Stop();
		app->TotalTime += (app->DeltaTime / 1000.0f);
	}
	app->Release();
}