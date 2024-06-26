//-----------------------------------------------------------------------------
// Includes 
//-----------------------------------------------------------------------------
#include "main.h"
#include "Application.h"
#include "renderer.h"
#include <io.h>

using namespace DirectX::SimpleMath;

D3D_FEATURE_LEVEL       Renderer::s_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::s_pDevice{};
ID3D11DeviceContext* Renderer::s_pDeviceContext{};
IDXGISwapChain* Renderer::s_pSwapChain{};
ID3D11RenderTargetView* Renderer::s_pRenderTargetView{};
ID3D11DepthStencilView* Renderer::s_pDepthStencilView{};

ID3D11Buffer* Renderer::s_pWorldBuffer{};
ID3D11Buffer* Renderer::s_pViewBuffer{};
ID3D11Buffer* Renderer::s_pProjectionBuffer{};
ID3D11Buffer* Renderer::s_pMaterialBuffer{};
ID3D11Buffer* Renderer::s_pLightBuffer{};


ID3D11DepthStencilState* Renderer::s_pDepthStateEnable{};
ID3D11DepthStencilState* Renderer::s_pDepthStateDisable{};


ID3D11BlendState* Renderer::s_pBlendState{};
ID3D11BlendState* Renderer::s_pBlendStateATC{};

Application* Renderer::s_pApplication;

///////////////////////////////////////////////////////////////////////////////
// Renderer class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void Renderer::Init(Application* ap)
{
	HRESULT hr = S_OK;
	

	// ������I�u�W�F�N�g�̃A�h���X���Z�b�g
	s_pApplication = ap;

	// �f�o�C�X�A�X���b�v�`�F�[���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = s_pApplication->GetWidth();
	swapChainDesc.BufferDesc.Height = s_pApplication->GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = s_pApplication->GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&s_pSwapChain,
		&s_pDevice,
		&s_FeatureLevel,
		&s_pDeviceContext);
	if (FAILED(hr)) { MessageBox(NULL,TEXT("ERORR"), NULL, MB_OK); }





	// �����_�[�^�[�Q�b�g�r���[�쐬
	ID3D11Texture2D* renderTarget{};
	s_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	s_pDevice->CreateRenderTargetView(renderTarget, NULL, &s_pRenderTargetView);
	renderTarget->Release();


	// �f�v�X�X�e���V���o�b�t�@�쐬
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	s_pDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// �f�v�X�X�e���V���r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	s_pDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &s_pDepthStencilView);
	depthStencile->Release();


	s_pDeviceContext->OMSetRenderTargets(1, &s_pRenderTargetView, s_pDepthStencilView);


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)s_pApplication->GetWidth();
	viewport.Height = (FLOAT)s_pApplication->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	s_pDeviceContext->RSSetViewports(1, &viewport);



	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	s_pDevice->CreateRasterizerState(&rasterizerDesc, &rs);

	s_pDeviceContext->RSSetState(rs);




	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	s_pDevice->CreateBlendState(&blendDesc, &s_pBlendState);

	blendDesc.AlphaToCoverageEnable = TRUE;
	s_pDevice->CreateBlendState(&blendDesc, &s_pBlendStateATC);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	s_pDeviceContext->OMSetBlendState(s_pBlendState, blendFactor, 0xffffffff);





	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	s_pDevice->CreateDepthStencilState(&depthStencilDesc, &s_pDepthStateEnable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	s_pDevice->CreateDepthStencilState(&depthStencilDesc, &s_pDepthStateDisable);//�[�x�����X�e�[�g

	s_pDeviceContext->OMSetDepthStencilState(s_pDepthStateEnable, NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc{};
	//samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	s_pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	s_pDeviceContext->PSSetSamplers(0, 1, &samplerState);



	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	s_pDevice->CreateBuffer(&bufferDesc, NULL, &s_pWorldBuffer);
	s_pDeviceContext->VSSetConstantBuffers(0, 1, &s_pWorldBuffer);// ���_�V�F�[�_��0�ɃZ�b�g

	s_pDevice->CreateBuffer(&bufferDesc, NULL, &s_pViewBuffer);
	s_pDeviceContext->VSSetConstantBuffers(1, 1, &s_pViewBuffer);// ���_�V�F�[�_��1�ɃZ�b�g

	s_pDevice->CreateBuffer(&bufferDesc, NULL, &s_pProjectionBuffer);
	s_pDeviceContext->VSSetConstantBuffers(2, 1, &s_pProjectionBuffer);// ���_�V�F�[�_��2�ɃZ�b�g


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	s_pDevice->CreateBuffer(&bufferDesc, NULL, &s_pMaterialBuffer);
	s_pDeviceContext->VSSetConstantBuffers(3, 1, &s_pMaterialBuffer);// ���_�V�F�[�_��3�ɃZ�b�g
	s_pDeviceContext->PSSetConstantBuffers(3, 1, &s_pMaterialBuffer);// �s�N�Z���V�F�[�_��3�ɃZ�b�g


	bufferDesc.ByteWidth = sizeof(LIGHT);

	s_pDevice->CreateBuffer(&bufferDesc, NULL, &s_pLightBuffer);
	s_pDeviceContext->VSSetConstantBuffers(4, 1, &s_pLightBuffer);// ���_�V�F�[�_��4�ɃZ�b�g
	s_pDeviceContext->PSSetConstantBuffers(4, 1, &s_pLightBuffer);// �s�N�Z���V�F�[�_��4�ɃZ�b�g





	// ���C�g������
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(0.5f, -1.0f, 0.8f, 0.0f);
	light.Direction.Normalize();
	light.Ambient = Color(0.5f, 0.5f, 0.5f, 1.0f);
	light.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	SetLight(light);



	// �}�e���A��������
	MATERIAL material{};
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);




}


//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void Renderer::Uninit()
{

	s_pWorldBuffer->Release();
	s_pViewBuffer->Release();
	s_pProjectionBuffer->Release();
	s_pLightBuffer->Release();
	s_pMaterialBuffer->Release();


	s_pDeviceContext->ClearState();
	s_pRenderTargetView->Release();
	s_pSwapChain->Release();
	s_pDeviceContext->Release();
	s_pDevice->Release();

}

//-----------------------------------------------------------------------------
// �����_�����O�O����
//-----------------------------------------------------------------------------
void Renderer::Begin()
{
	float clearColor[4] = { 0.7f, 0.8f, 1.0f, 1.0f };
	s_pDeviceContext->ClearRenderTargetView(s_pRenderTargetView, clearColor);
	s_pDeviceContext->ClearDepthStencilView(s_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//-----------------------------------------------------------------------------
// �����_�����O�㏈��
//-----------------------------------------------------------------------------
void Renderer::End()
{
	s_pSwapChain->Present(1, 0);
}

//-----------------------------------------------------------------------------
// �f�v�X�X�e���V���̗L����������
//-----------------------------------------------------------------------------
void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		s_pDeviceContext->OMSetDepthStencilState(s_pDepthStateEnable, NULL);
	else
		s_pDeviceContext->OMSetDepthStencilState(s_pDepthStateDisable, NULL);

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void Renderer::SetATCEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		s_pDeviceContext->OMSetBlendState(s_pBlendStateATC, blendFactor, 0xffffffff);
	else
		s_pDeviceContext->OMSetBlendState(s_pBlendState, blendFactor, 0xffffffff);

}

//-----------------------------------------------------------------------------
// 2D���̃��[���h�A�r���[�A�v���W�F�N�V�����̍s��ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetWorldViewProjection2D()
{
	Matrix world;
	world = Matrix::Identity;			// �P�ʍs��ɂ���
	world = world.Transpose();			// �]�u
	s_pDeviceContext->UpdateSubresource(s_pWorldBuffer, 0, NULL, &world, 0, 0);

	Matrix view;
	view = Matrix::Identity;
	view = view.Transpose();
	s_pDeviceContext->UpdateSubresource(s_pViewBuffer, 0, NULL, &view, 0, 0);

	Matrix projection;
	// 2D�`������㌴�_�ɂ��� 
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(s_pApplication->GetWidth()),					// �r���[�{�����[���̍ŏ��w
		static_cast<float>(s_pApplication->GetHeight()),					// �r���[�{�����[���̍ŏ��x
		0.0f,															// �r���[�{�����[���̍ő�x
		0.0f,
		1.0f);


	projection = projection.Transpose();

	s_pDeviceContext->UpdateSubresource(s_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//-----------------------------------------------------------------------------
// ���[���h�s��̐ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetWorldMatrix(Matrix* WorldMatrix)
{
	Matrix world;
	world = WorldMatrix->Transpose();					// �]�u

	s_pDeviceContext->UpdateSubresource(s_pWorldBuffer, 0, NULL, &world, 0, 0);
}

//-----------------------------------------------------------------------------
// �r���[�s��̐ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetViewMatrix(Matrix* ViewMatrix)
{
	Matrix view;
	view = ViewMatrix->Transpose();						// �]�u

	s_pDeviceContext->UpdateSubresource(s_pViewBuffer, 0, NULL, &view, 0, 0);
}

//-----------------------------------------------------------------------------
// �v���W�F�N�V�����̐ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetProjectionMatrix(Matrix* ProjectionMatrix)
{
	Matrix projection;
	projection = ProjectionMatrix->Transpose();

	s_pDeviceContext->UpdateSubresource(s_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

//-----------------------------------------------------------------------------
// �}�e���A���̐ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetMaterial(MATERIAL Material)
{
	s_pDeviceContext->UpdateSubresource(s_pMaterialBuffer, 0, NULL, &Material, 0, 0);
}

//-----------------------------------------------------------------------------
// ���C�g�̐ݒ�
//-----------------------------------------------------------------------------
void Renderer::SetLight(LIGHT Light)
{
	s_pDeviceContext->UpdateSubresource(s_pLightBuffer, 0, NULL, &Light, 0, 0);
}

//-----------------------------------------------------------------------------
// ���_�V�F�[�_�[�̐���
//-----------------------------------------------------------------------------
void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout,
	D3D11_INPUT_ELEMENT_DESC* layout, UINT elements, const char* FileName)
{
	FILE* file;
	long int fsize;

	fopen_s(&file, FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	s_pDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	s_pDevice->CreateInputLayout(layout,
		elements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}

//-----------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̐���
//-----------------------------------------------------------------------------
void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	fopen_s(&file, FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	s_pDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}