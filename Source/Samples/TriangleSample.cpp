#include <stdio.h>
#include "Core/EntryPoint.h"
#include "Core/App/AppBase.h"
#include "Core/Logging.h"
#include "Graphics/GraphicsInterface.h"
#include "Graphics/Platform/BaseWindow.h"
#include "Graphics/UI/UIInterface.h"

using namespace Graphics;
struct Vertex
{
	float x, y, z;
	float r, g, b;
};

class TriangleApp : public AppBase
{
public:
	TriangleApp(){}
	~TriangleApp(){}
	void Init();
	void Update();
	void Release();

private:
	BufferHandle m_VertexBuffer;
	GraphicsPipeline m_Pipeline;
};

void TriangleApp::Init()
{
	AppBase::Init();
	
	Vertex arr[3] =
	{
		-0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f
	};
	m_VertexBuffer = mGraphicsInterface->CreateBuffer(BufferType::VertexBuffer, CPUAccess::None, GPUAccess::None, sizeof(Vertex) * 3, 0, &arr[0]);
	{
		GraphicsPipelineDescription pdesc = {};
		pdesc.PixelShader.ShaderEntryPoint = "PSSimple";
		pdesc.PixelShader.ShaderPath = "TriangleSample.hlsl";
		pdesc.PixelShader.Type = ShaderType::Pixel;

		pdesc.VertexShader.ShaderEntryPoint = "VSSimple";
		pdesc.VertexShader.ShaderPath = "TriangleSample.hlsl";
		pdesc.VertexShader.Type = ShaderType::Vertex;

		VertexInputDescription::VertexInputElement eles[2];
		eles[0].Semantic = "POSITION";
		eles[0].Idx = 0;
		eles[0].EleFormat = Format::RGB_32_Float;
		eles[0].Offset = 0;

		eles[1].Semantic = "COLOR";
		eles[1].Idx = 0;
		eles[1].EleFormat = Format::RGB_32_Float;
		eles[1].Offset = sizeof(float) * 3;

		pdesc.VertexDescription.NumElements = sizeof(eles) / sizeof(VertexInputDescription::VertexInputElement);
		pdesc.VertexDescription.Elements = eles;
		pdesc.ColorFormats[0] = mGraphicsInterface->GetOutputFormat();
		m_Pipeline = mGraphicsInterface->CreateGraphicsPipeline(pdesc);
	}

	mGraphicsInterface->FlushAndWait();
}

void TriangleApp::Update()
{
	AppBase::Update();

	mGraphicsInterface->SetScissor(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
	mGraphicsInterface->SetTopology(Topology::TriangleList);
	mGraphicsInterface->SetGraphicsPipeline(m_Pipeline);
	mGraphicsInterface->SetVertexBuffer(m_VertexBuffer, sizeof(Vertex) * 3, sizeof(Vertex));
	mGraphicsInterface->Draw(3, 0);

	ImGui::Begin("Paco");
	ImGui::End();
}

void TriangleApp::Release()
{
	mGraphicsInterface->ReleaseGraphicsPipeline(m_Pipeline);
	AppBase::Release();
}

TriangleApp app;
ENTRY_POINT(app, "Triangle App", false);