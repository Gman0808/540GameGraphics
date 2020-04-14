#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Vertex.h"


class Mesh
{
	//indices counter
	int indCounter;

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

public:
	//constructor and destructor
	Mesh(Vertex vObjects[], int vCount, unsigned int indices[], int indicesCount, Microsoft::WRL::ComPtr<ID3D11Device> bufferPointer);
	Mesh();
	Mesh(const char* objFile, Microsoft::WRL::ComPtr<ID3D11Device> device);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	//Mesh(const char* objFile, Microsoft::WRL::ComPtr<ID3D11Device> device);
   ~Mesh();
   void CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device);
   void drawShape(Microsoft::WRL::ComPtr<ID3D11DeviceContext> contextPointer);
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

	int GetIndexCount();
	void SetBuffersAndDraw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
};

