#define NOMINMAX

#include "Graphics/DX12/DX12GraphicsInterface.h"
#include "Graphics/Platform/Windows/WWindow.h"
#include "Graphics/AssetImporter.h"
#include "ShowcaseScene.h"

bool InitSystems();
void Resize(int w, int h);

Graphics::Platform::BaseWindow* gWindow = nullptr;
Graphics::GraphicsInterface* gGraphics = nullptr;
Graphics::AssetImporter* gAssetImporter = nullptr;
ShowcaseScene* gScene = nullptr;

int main()
{
	InitSystems();
	
	gScene = new ShowcaseScene(gGraphics, gAssetImporter);
	gScene->Initialize();

	Resize(1280, 920);
	gGraphics->FlushAndWait();

	bool running = true;
	while (running)
	{
		gWindow->Update();
		gScene->Update(0.0f);

		gGraphics->StartFrame();
		gScene->Draw(0.0f);
		gGraphics->EndFrame();

		running = !gWindow->IsClosed();
	}

	return 1;
}

bool InitSystems()
{
	gWindow = new Graphics::Platform::Windows::WWindow();
	gWindow->Initialize("Awesome Showcase", false, 1280, 920);

	gGraphics = new Graphics::DX12::DX12GraphicsInterface();
	gGraphics->Initialize(gWindow);

	gAssetImporter = new Graphics::AssetImporter(gGraphics);

	return true;
}

void Resize(int w, int h)
{
	gScene->Resize(w, h);
}