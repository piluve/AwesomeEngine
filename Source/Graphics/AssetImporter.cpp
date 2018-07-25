#include "AssetImporter.h"
#include "GraphicsInterface.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Graphics
{
	AssetImporter::AssetImporter(GraphicsInterface* graphics):
		mGraphicsInterface(graphics)
	{
	}

	AssetImporter::~AssetImporter()
	{
	}

	bool AssetImporter::LoadModel(const char* path,Mesh*& outMeshes, uint8_t& numMeshes)
	{
		std::string fullPath = "..\\..\\Assets\\Meshes\\" + std::string(path);
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string log;

		if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &log, fullPath.c_str()))
		{
			std::cout << log << std::endl;
		}
		if (log.size())
		{
			std::cout << log << std::endl;
		}

		std::vector<std::vector<FullVertex>> vertexDataShapes;

		vertexDataShapes.resize(shapes.size());

		int shapeIdx = 0;
		for (auto shapeIt = shapes.begin(); shapeIt != shapes.end(); ++shapeIt)
		{
			vertexDataShapes[shapeIdx].resize(shapeIt->mesh.indices.size());
			FullVertex* cur = &vertexDataShapes[shapeIdx][0];
			for (int i = 0; i < shapeIt->mesh.indices.size(); i++)
			{
				const auto curIdx = shapeIt->mesh.indices[i];

				cur->Position.x = attribs.vertices[3 * curIdx.vertex_index + 0];
				cur->Position.y = attribs.vertices[3 * curIdx.vertex_index + 1];
				cur->Position.z = attribs.vertices[3 * curIdx.vertex_index + 2];

				cur->Normal.x = attribs.normals[3 * curIdx.normal_index + 0];
				cur->Normal.y = attribs.normals[3 * curIdx.normal_index + 1];
				cur->Normal.z = attribs.normals[3 * curIdx.normal_index + 2];
				cur->Normal = glm::normalize(cur->Normal);
				
				cur->Texcoords.x = attribs.texcoords[2 * curIdx.texcoord_index + 0];
				cur->Texcoords.y = attribs.texcoords[2 * curIdx.texcoord_index + 1];

				cur++;
			}
			shapeIdx++;
		}

		// Calculate tangents
		for (int i=0;i<vertexDataShapes.size();i++)
		{
			auto& curVtxBuffer = vertexDataShapes[i];
			for (int i = 0; i < curVtxBuffer.size(); i += 3)
			{
				FullVertex* curVtx = &curVtxBuffer[i];

				// vtx1
				auto p1 = curVtx->Position;
				auto tc1 = curVtx->Texcoords;
				curVtx++;

				// vtx2
				auto p2 = curVtx->Position;
				auto tc2 = curVtx->Texcoords;
				curVtx++;

				// vtx3
				auto p3 = curVtx->Position;
				auto tc3 = curVtx->Texcoords;
				curVtx++;

				glm::vec3 edge1		= p2 - p1;
				glm::vec3 edge2		= p3 - p1;
				glm::vec2 deltaUV1	= tc2 - tc1;
				glm::vec2 deltaUV2	= tc3 - tc1;
				float f				= 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
				glm::vec3 triTangent;
				triTangent.x	= f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				triTangent.y	= f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				triTangent.z	= f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				triTangent		= glm::normalize(triTangent);

				curVtxBuffer[i + 0].Tangent = glm::vec4(triTangent,0.0f);
				curVtxBuffer[i + 1].Tangent = glm::vec4(triTangent,0.0f);	
				curVtxBuffer[i + 2].Tangent = glm::vec4(triTangent,0.0f);
			}
		}

		// Create the meshes 
		auto eleSize = sizeof(FullVertex);
		numMeshes = shapes.size();
		outMeshes = new Mesh[numMeshes];
		for (int i = 0; i < numMeshes; i++)
		{
			auto numEles = vertexDataShapes[i].size();
			outMeshes[i].VertexBuffer = mGraphicsInterface->CreateBuffer
			(
				BufferType::VertexBuffer, 
				CPUAccess::None, 
				numEles * eleSize, 
				vertexDataShapes[i].data()
			);
			outMeshes[i].ElementSize = eleSize;
			outMeshes[i].NumVertex = numEles;
		}

		return true;
	}

	bool AssetImporter::LoadTexture(const char* path, unsigned char*& outData, int& width, int& height,Graphics::Format& format )
	{
		int n;
		std::string fullPath = "../../Assets/Textures/" + std::string(path);
		// we force all textures to have RGBA!
		outData = stbi_load(fullPath.c_str(), &width, &height, &n, 4);
		if (!outData)
		{
			return false;
		}
		// We are loading a texture with 8bits per element (unsigned char)
		// Also we force it to be RGBA
		format = Graphics::Format::RGBA_8_Unorm;
		return true;
	}

	void AssetImporter::FreeLoadedTexture(void * loadedData)
	{
		if (loadedData)
		{
			stbi_image_free(loadedData);
		}
	}
}