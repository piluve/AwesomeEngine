#define NOMINMAX

#include "Graphics/Platform/BaseWindow.h"
#include "Core/EntryPoint.h"
#include "Core/App/AppBase.h"
#include "Graphics/GraphicsInterface.h"
#include "Graphics/World/Model.h"
#include "Graphics/World/SceneGraph.h"
#include "Graphics/World/Actor.h"
#include "Graphics/World/Renderable.h"
#include "Graphics/TestRenderer.h"

#include "glm/ext.hpp"

#include <stdio.h>

using namespace World;

class AdvancedApp : public AppBase
{
public:
	AdvancedApp() {}
	~AdvancedApp() {}
	void Init();
	void Update();
	void Release();

private:
	Graphics::Model* mCube;
	World::SceneGraph mScene;
	Graphics::TestRenderer mRenderer;
};

void AdvancedApp::Init()
{
	AppBase::Init();

	mRenderer.Initialize(this);

	// Spawn some stuff
	mCube = Graphics::ModelFactory::Get()->LoadFromFile("Meshes\\cube.obj", mGraphicsInterface);
	for (uint32_t x = 0; x < 16; ++x)
	{
		for (uint32_t y = 0; y < 16; ++y)
		{
			World::Renderable* curCube = mScene.SpawnRenderable();
			curCube->SetPosition((float)x * 2.0f, (float)y * 2.0f, 0.0f);
			curCube->SetModel(mCube);
		}
	}	

	mGraphicsInterface->FlushAndWait();
}

void AdvancedApp::Update()
{
	AppBase::Update();

	mRenderer.Render(&mScene);
}

void AdvancedApp::Release()
{

	AppBase::Release();
}

AdvancedApp app;
ENTRY_POINT(app, "Advanced App", false);

