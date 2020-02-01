#include "SceneGraph.h"
#include "Actor.h"
#include "Renderable.h"
#include "Camera.h"

using namespace World;

SceneGraph::SceneGraph()
{
	mRoot = new Actor;
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Update(float deltaTime)
{
	mRoot->Update(deltaTime);
}

Renderable* SceneGraph::SpawnRenderable(Actor* parent /*= nullptr*/)
{
	Renderable* renderable = new Renderable;
	if (parent)
	{
		parent->AddChild(renderable);
	}
	else
	{
		mRoot->AddChild(renderable);
	}
	return renderable;
}

Camera* SceneGraph::SpawnCamera(Actor* parent /*= nullptr*/)
{
	Camera* camera = new Camera;
	if (parent)
	{
		parent->AddChild(camera);
	}
	else
	{
		mRoot->AddChild(camera);
	}
	mCameras.push_back(camera);
	return camera;
}

Actor* World::SceneGraph::GetRoot() const
{
	return mRoot;
}

const std::vector<Camera*>& SceneGraph::GetCameras() const
{
	return mCameras;
}