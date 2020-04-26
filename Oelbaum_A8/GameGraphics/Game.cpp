#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include <iostream>
using namespace std;
// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include <ctime>
#include <ratio>
#include <chrono>

// For the DirectX Math library
using namespace DirectX;
using namespace std::chrono;
// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	cam = 0;
//	currentEntity = 0;
	//prevTab = false;
	destructivePixelShader = 0;
	destructiveVertexShader = 0;
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object
	//cam->~Camera();
//	cam = nullptr;
//	delete []cam;
	
	
	delete cam;
	cam = nullptr;

	delete destructiveVertexShader;
	destructiveVertexShader = nullptr;
	delete destructivePixelShader;
	destructivePixelShader = nullptr;
	delete vertexShader;
	vertexShader = nullptr;
	delete pixelShader;
	pixelShader = nullptr;

	
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	currentTime = 0;
	lData.ambientColor = XMFLOAT3(0.5f, 0.1f, 0.6f);
	lData.diffuseColor = XMFLOAT3(1.0f, 0.1f, 0.1f);
	lData.direction = XMFLOAT3(1, 0.5f, 0);

	lData2.ambientColor = XMFLOAT3(0.2f, 0.8f, 0.0f);
	lData2.diffuseColor = XMFLOAT3(1.0f, 0.5f, 1.0f);
	lData2.direction = XMFLOAT3(1.0f, 0.0f, 0.0f);

	lData3.ambientColor = XMFLOAT3(0.5f, 0.5f, 0.0f);
	lData3.diffuseColor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	lData3.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Texture releated init
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/rock.png").c_str(),
		nullptr,		// We don't need the texture ref ourselves
		diffuseTexture.GetAddressOf()); // We do need an SRV
		// Texture releated init
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/surface_input.png").c_str(),
		nullptr,		// We don't need the texture ref ourselves
		surfInput.GetAddressOf()); // We do need an SRV


	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Tri-linear filtering
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, samplerOptions.GetAddressOf());

	



	// Texture releated init
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/rock.png").c_str(),
		nullptr,		// We don't need the texture ref ourselves
		diffuseTexture2.GetAddressOf()); // We do need an SRV

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/rock_normals.png").c_str(),
		nullptr,		// We don't need the texture ref ourselves
		normalMap2.GetAddressOf()); // We do need an SRV
		// Texture releated init
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),	// Passing in the context auto-generates mipmaps!!
		GetFullPathTo_Wide(L"../../Assets/Textures/surface_input.png").c_str(),
		nullptr,		// We don't need the texture ref ourselves
		surfInput2.GetAddressOf()); // We do need an SRV

	D3D11_SAMPLER_DESC sampDesc2 = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Tri-linear filtering
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, samplerOptions2.GetAddressOf());

	mat2 = Material(XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), destructiveVertexShader, destructivePixelShader, 1.5f, diffuseTexture2, normalMap2, surfInput2, samplerOptions2);
	mat1 = Material(XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), vertexShader, pixelShader, 1.5f, diffuseTexture2, normalMap2,surfInput2, samplerOptions2);

	tObj1 = Transform(XMFLOAT3(0, 0, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	tObj2 = Transform(XMFLOAT3(0, 0, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(10.0f, 10.0f, 10.0f));

	Mesh* levelGeometry = new Mesh(GetFullPathTo("../../Assets/Models/LevelGeo.obj").c_str(), device);
//	Mesh* levelGeometry = new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);
	Mesh* playMesh = new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);


	pEntity = new Entity(playMesh, tObj1);
	pEntity->GetTransform()->SetPosition(0, 0, 0.0f);
	pEntity->mat = &mat1;

	
	


	//cam = new Camera(XMFLOAT3(0,0,0), ((float)this->width/this->height), keyMove, mouseMove, farClip,nearClip, feild view);
	cam = new Camera(XMFLOAT3(0, 1.9f, -3.3f), ((float)this->width / this->height), 2, 1, 1000.0f, 0.001f, 0.78f);
	
	player = Player(pEntity, cam);

	levelEntity = new Entity(levelGeometry, tObj2);
	levelEntity->mat = &mat2;



	skyMesh = new  Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);
	// Load the sky box texture
	CreateDDSTextureFromFile(
		device.Get(),
		GetFullPathTo_Wide(L"../../Assets/Skies/SpaceCubeMap.dds").c_str(),
		0,
		skySRV.GetAddressOf());

	// Make the sky rasterizer state
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rastDesc, skyRasterState.GetAddressOf());

	// Make the sky depth state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dsDesc, skyDepthState.GetAddressOf());

	//Since everything is using backfaces, don't worry about resetting culling
	context->RSSetState(skyRasterState.Get());
}

void Game::RenderSky()
{
	// Set up my render state
	context->OMSetDepthStencilState(skyDepthState.Get(), 0);

	// Set up the sky shaders
	skyVS->SetShader();
	skyPS->SetShader();
	
	skyVS->SetMatrix4x4("view", cam->getView());
	skyVS->SetMatrix4x4("proj", cam->getProj());
	skyVS->CopyAllBufferData();

	skyPS->SetShaderResourceView("sky", skySRV.Get());
	skyPS->SetSamplerState("samplerOptions", samplerOptions.Get());

	// Actually draw the geometry
	skyMesh->SetBuffersAndDraw(context);

	// Reset any states back to the default
	context->OMSetDepthStencilState(0, 0);
}






// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{

	destructiveVertexShader = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"DestructiveVertexShader.cso").c_str());

	destructivePixelShader = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"DestructivePixelShader.cso").c_str());
	
	vertexShader = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());

	pixelShader = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());

	skyVS = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"SkyVS.cso").c_str());

	skyPS = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"SkyPS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(-0.4f, +0.4f, +0.0f),XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(-0.4f, -0.4f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
	};

	Vertex vertices2[] =
	{
		{ XMFLOAT3(+0.0f, +0.95f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
		{ XMFLOAT3(+0.0f, +0.0f, +0.20f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
		{ XMFLOAT3(-0.95f, +0.0f, +0.20f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(-0.95f, +0.95f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },

	};

	Vertex vertices3[] =
	{
	    { XMFLOAT3(+0.1f, +0.5f, +0.0f),XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
		{ XMFLOAT3(+0.2f, +0.1f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(+0.2f, -0.1f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(+0.1f, -0.5f, +0.0f),XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f) },
		{ XMFLOAT3(-0.2f, -0.1f, +0.0f), XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
		{ XMFLOAT3(-0.2f, +0.1f, +0.0f),XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f, 0.0f,-1.0f)},
	};


	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };
	unsigned int indices2[] = { 0, 1, 2, 0,2,3};
	unsigned int indices3[] = { 0,1,2,1,4,5,1,2,4,2,3,4};


	
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	if(cam != NULL)
	cam->UpdateProjectionMatrix((float)this->width / this->height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	
	cam->Update(deltaTime, this->hWnd);
	player.Update(deltaTime, this->hWnd, this->width, this->height);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
//  - Do this ONCE PER FRAME
//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	destructiveVertexShader->SetShader();
	destructivePixelShader->SetShader();

	destructivePixelShader->SetData("gameTime",
		&totalTime,
		sizeof(totalTime)
	);
	destructivePixelShader->SetData("screenSizeX",
		&width,
		sizeof(width)
	);
	destructivePixelShader->SetData("screenSizeY",
		&height,
		sizeof(height)
	);

	destructivePixelShader->SetData("lData1",
	&lData, 
	sizeof(DirectionalLight));


	destructivePixelShader->SetData("lData2",
		&lData2,
		sizeof(DirectionalLight));

	destructivePixelShader->SetData("lData3",
		&lData3,
		sizeof(DirectionalLight));

	
	destructivePixelShader->SetFloat3("pointLightPos", XMFLOAT3(0.0f, 0.2f, 0.5f));
	destructivePixelShader->SetFloat3("pointLightColor", XMFLOAT3(1.0f, 0.5f, 1.0f));
	//pixelShader->SetFloat3("lightDirection", XMFLOAT3(0, 0, 1));

	destructivePixelShader->SetFloat3("cameraPosition", cam->transform.GetPosition()); 
	destructivePixelShader->CopyAllBufferData();

	levelEntity->Draw(context, cam->getView(), cam->getProj(),  player.getPosition(), false);

	//Player-specific shader information
	vertexShader->SetShader();
	pixelShader->SetShader();

	pixelShader->SetData("lData1",
		&lData,
		sizeof(DirectionalLight));


	pixelShader->SetData("lData2",
		&lData2,
		sizeof(DirectionalLight));

	pixelShader->SetData("lData3",
		&lData3,
		sizeof(DirectionalLight));


	pixelShader->SetFloat3("pointLightPos", XMFLOAT3(0.0f, 0.2f, 0.5f));
	pixelShader->SetFloat3("pointLightColor", XMFLOAT3(1.0f, 0.5f, 1.0f));
	//pixelShader->SetFloat3("lightDirection", XMFLOAT3(0, 0, 1));

	pixelShader->SetFloat3("cameraPosition", cam->transform.GetPosition());
	pixelShader->CopyAllBufferData();
	
	pEntity->Draw(context, cam->getView(), cam->getProj(), player.getPosition(), true);


	// Render the sky AFTER all opaque (solid) geometry has been rendered
	RenderSky();

		// Present the back buffer to the user
		//  - Puts the final frame we're drawing into the window so the user can see it
		//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
		swapChain->Present(1, 0);

		// Due to the usage of a more sophisticated swap chain,
		// the render target must be re-bound after every call to Present()
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());


	
	

}