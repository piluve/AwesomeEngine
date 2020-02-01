#pragma once

#include "RendererBase.h"
#include "GraphicsInterface.h"
#include "Declarations.h"

#include <vector>

namespace World
{
	class Actor;
	class Camera;
}

namespace Graphics
{
	struct Mesh;
	struct RenderItem
	{
		glm::mat4 WorldMatrix;
		Mesh* Meshes;
		uint32_t NumMeshes;
	};

	class TestRenderer : public RendererBase
	{
	public:
		TestRenderer();
		~TestRenderer();
		void Initialize(AppBase* app);
		void Release();
		void Render(World::SceneGraph* scene)override;

	private:
		void ProcessVisibility(World::Camera* camera, const std::vector<World::Actor*>& actors, std::vector<RenderItem>& renderItems);
		void DrawOriginGizmo();

		Graphics::TextureHandle mColourRt;
		Graphics::TextureHandle mDepthRt;
		Graphics::GraphicsPipeline mTestPipeline;
		Graphics::GraphicsPipeline mPresentPipeline;
		Graphics::BufferHandle mPresentVtxBuffer;
		CameraData mCameraData;
		Graphics::BufferHandle mCameraDataCb;
		ItemData mItemData;
		Graphics::BufferHandle mItemDataCb;
	};
}