#include "CloudRendering.h"
#include "Graphics/UI/IMGUI/imgui.h"
#include "Graphics/Noise.h"

namespace Graphics
{
	CloudRenderer::CloudRenderer():
		mGraphicsInterface(nullptr)
	{
	}

	CloudRenderer::~CloudRenderer()
	{
	}

	bool CloudRenderer::Initialize(GraphicsInterface* graphicsInterface)
	{
		mGraphicsInterface = graphicsInterface;
		struct Texel { unsigned char r, g, b, a; };

		// 2D test
		{
			int numNoiseVals = 128;
			ValueNoise2D noise2D;
			noise2D.Initialize(numNoiseVals, numNoiseVals);

			const int tw = 256;
			const int th = 256;
			Texel* texData = new Texel[tw * th];
			for (int v = 0; v < th; v++)
			{
				for (int u = 0; u < tw; u++)
				{
					/*
						We want to use all the values within our noise object.
						This way we make sure we make a tileable texture.
						To do this we sample FBM using range [0,1] and offsetting by the numbers of 
						values the noise object holds.
						The size of the texture does not matter!
					*/
					float cu = float(u) / float(tw);
					float cv = float(v) / float(th);
					unsigned char n0 = unsigned char(noise2D.Fbm(cu * numNoiseVals, cv * numNoiseVals, 5) * 255.0f);
					unsigned char n = (n0);
					texData[u + v * tw] = Texel{ n,n,n, 255 };
				}
			}
			mTestTexture = mGraphicsInterface->CreateTexture2D(tw, th, 1, 1, Graphics::Format::RGBA_8_Unorm, Graphics::TextureFlagNone, texData);
			delete[] texData;
		}

		// 3D test
		{
			int numNoiseVals = 64;
			ValueNoise3D noise3D;
			noise3D.Initialize(numNoiseVals, numNoiseVals, numNoiseVals);

			const int tw = 256;
			const int th = 256;
			const int td = 16;
			Texel* texData = new Texel[tw * th * td];
			// Total size of each slice
			uint32_t slizeSize = tw * th;

			for (int w = 0; w < td; w++)			// Iterate over slices
			{
				for (int v = 0; v < th; v++)		// Top to bottom
				{
					for (int u = 0; u < tw; u++)	// Left to right
					{
						float cu = float(u) / float(tw);
						float cv = float(v) / float(th);
						float cw = float(w) / float(td);

						unsigned char r = unsigned char(cu * 255.0f);
						unsigned char g = unsigned char(cv * 255.0f);

						float n = noise3D.Sample(cu * numNoiseVals, cv * numNoiseVals, cw * numNoiseVals);

						size_t slizeOff = (slizeSize * w);
						texData[slizeOff + (u + v * tw)] = Texel{ unsigned char(n * 255.0f),0 ,0 , 0xff };
					}
				}
			}

			mTestTexture3D = mGraphicsInterface->CreateTexture3D(tw,th,1,td, Graphics::Format::RGBA_8_Unorm, Graphics::TextureFlagNone, texData);
			delete[] texData;
		}


		Graphics::GraphicsPipelineDescription desc;
		desc.DepthEnabled = true;
		desc.DepthWriteEnabled = false;
		desc.DepthFunction = Graphics::Equal; // we paint it at far (so we can discard fragments from it)
		desc.DepthFormat = Graphics::Format::Depth24_Stencil8;
		desc.VertexShader.ShaderEntryPoint = "VSClouds";
		desc.VertexShader.ShaderPath = "Clouds.hlsl";
		desc.VertexShader.Type = Graphics::Vertex;
		desc.PixelShader.ShaderEntryPoint = "PSClouds";
		desc.PixelShader.ShaderPath = "Clouds.hlsl";
		desc.PixelShader.Type = Graphics::Pixel;
		Graphics::VertexInputDescription::VertexInputElement eles[1] =
		{
			"POSITION",0, Graphics::Format::RGB_32_Float,0
		};
		desc.VertexDescription.NumElements = 1;
		desc.VertexDescription.Elements = eles;
		desc.ColorFormats[0] = Graphics::Format::RGBA_16_Float;

		desc.BlendTargets[0].Enabled = true;
		desc.BlendTargets[0].SrcBlendColor = Graphics::BlendFunction::BlendSrcAlpha;
		desc.BlendTargets[0].DstBlendColor = Graphics::BlendFunction::BlendInvSrcAlpha;
		desc.BlendTargets[0].BlendOpColor = Graphics::BlendOperation::BlendOpAdd;

		desc.BlendTargets[0].SrcBlendAlpha = Graphics::BlendFunction::BlendOne;
		desc.BlendTargets[0].DstBlendAlpha = Graphics::BlendFunction::BlendOne;
		desc.BlendTargets[0].BlendOpAlpha = Graphics::BlendOperation::BlendOpAdd;

		mCloudsPipeline = mGraphicsInterface->CreateGraphicsPipeline(desc);

		mCloudsDataHandle = mGraphicsInterface->CreateBuffer(Graphics::BufferType::ConstantBuffer, Graphics::CPUAccess::None, sizeof(mCloudsData));

		return true;
	}

	void CloudRenderer::Draw(float dt,glm::vec3 camPos, glm::mat4 iViewProj)
	{
		mCloudsData.ViewPosition = glm::vec4(camPos,0.0f);
		mCloudsData.InvViewProj = iViewProj;
		mGraphicsInterface->SetConstantBuffer(mCloudsDataHandle, 0, sizeof(mCloudsData),&mCloudsData);
		mGraphicsInterface->SetGraphicsPipeline(mCloudsPipeline);
		mGraphicsInterface->SetTexture(mTestTexture3D, 0);
		mGraphicsInterface->Draw(6, 0);
	}

	void CloudRenderer::ShowDebug()
	{
		ImGui::Begin("Clouds");
		ImGui::Text("Base value noise");
		ImGui::Image((ImTextureID)mTestTexture3D.Handle, ImVec2(512, 512));
		ImGui::Separator();
		ImGui::End();
	}
}