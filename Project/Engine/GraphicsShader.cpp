#include "pch.h"

#include "Device.h"
#include "PathMgr.h"

#include "GraphicsShader.h"
namespace ff7r
{
	GraphicsShader::GraphicsShader()
		: Shader(RES_TYPE::GRAPHICS_SHADER)
		, topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, rasterizer_type(RS_TYPE::CULL_BACK)
		, depthstencil_type(DS_TYPE::LESS)
		, blend_type(BS_TYPE::DEFAULT)
		, domain(SHADER_DOMAIN::DOMAIN_UNDEFINED)
		, scalar_param{}
		, texture_param{}
	{
	}

	GraphicsShader::~GraphicsShader()
	{
	}

	void GraphicsShader::CreateVertexShader(const wstring& _file_name, const string& _func_name)
	{
		// Shader 파일 경로
		wstring shader_file_path = PathMgr::GetInst()->GetContentPath();
		shader_file_path += _file_name;

		// VertexShader Compile
		if (FAILED(D3DCompileFromFile(shader_file_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _func_name.c_str(), "vs_5_0", 0, 0, vertex_shader_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Vertex Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
		DEVICE->CreateVertexShader(vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize()
			, nullptr, vertex_shader.GetAddressOf());



		// InputLayout 생성
		D3D11_INPUT_ELEMENT_DESC LayoutDesc[6] = {};

		LayoutDesc[0].SemanticName = "POSITION";
		LayoutDesc[0].SemanticIndex = 0;
		LayoutDesc[0].InputSlot = 0;
		LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[0].InstanceDataStepRate = 0;
		LayoutDesc[0].AlignedByteOffset = 0;
		LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;


		LayoutDesc[1].SemanticName = "COLOR";
		LayoutDesc[1].SemanticIndex = 0;
		LayoutDesc[1].InputSlot = 0;
		LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[1].InstanceDataStepRate = 0;
		LayoutDesc[1].AlignedByteOffset = 12;
		LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		LayoutDesc[2].SemanticName = "TEXCOORD";
		LayoutDesc[2].SemanticIndex = 0;
		LayoutDesc[2].InputSlot = 0;
		LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[2].InstanceDataStepRate = 0;
		LayoutDesc[2].AlignedByteOffset = 28;
		LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;

		LayoutDesc[3].SemanticName = "NORMAL";
		LayoutDesc[3].SemanticIndex = 0;
		LayoutDesc[3].InputSlot = 0;
		LayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[3].InstanceDataStepRate = 0;
		LayoutDesc[3].AlignedByteOffset = 36;
		LayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[4].SemanticName = "TANGENT";
		LayoutDesc[4].SemanticIndex = 0;
		LayoutDesc[4].InputSlot = 0;
		LayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[4].InstanceDataStepRate = 0;
		LayoutDesc[4].AlignedByteOffset = 48;
		LayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[5].SemanticName = "BINORMAL";
		LayoutDesc[5].SemanticIndex = 0;
		LayoutDesc[5].InputSlot = 0;
		LayoutDesc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[5].InstanceDataStepRate = 0;
		LayoutDesc[5].AlignedByteOffset = 60;
		LayoutDesc[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;


		if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 6
			, vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize()
			, input_layout.GetAddressOf())))
		{
			assert(nullptr);
		}
	}

	void GraphicsShader::CreateGeometryShader(const wstring& _file_name, const string& _func_name)
	{
		// Shader 파일 경로
		wstring shader_file_path = PathMgr::GetInst()->GetContentPath();
		shader_file_path += _file_name;

		// Shader Compile	
		if (FAILED(D3DCompileFromFile(shader_file_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _func_name.c_str(), "gs_5_0", 0, 0, geomentry_shader_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 Shader 를 만든다.
		DEVICE->CreateGeometryShader(geomentry_shader_blob->GetBufferPointer(), geomentry_shader_blob->GetBufferSize()
			, nullptr, geomentry_shader.GetAddressOf());
	}

	void GraphicsShader::CreatePixelShader(const wstring& _file_name, const string& _func_name)
	{
		// Shader 파일 경로
		wstring shader_file_path = PathMgr::GetInst()->GetContentPath();
		shader_file_path += _file_name;


		// PixelShader Compile	
		if (FAILED(D3DCompileFromFile(shader_file_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _func_name.c_str(), "ps_5_0", 0, 0, pixel_shader_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			MessageBoxA(nullptr, (const char*)error_blob->GetBufferPointer()
				, "Pixel Shader Compile Failed!!", MB_OK);
		}

		// 컴파일된 객체로 PixelShader 를 만든다.
		DEVICE->CreatePixelShader(pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize()
			, nullptr, pixel_shader.GetAddressOf());
	}

	void GraphicsShader::UpdateData()
	{
		CONTEXT->IASetInputLayout(input_layout.Get());
		CONTEXT->IASetPrimitiveTopology(topology);

		CONTEXT->VSSetShader(vertex_shader.Get(), nullptr, 0);
		CONTEXT->HSSetShader(hull_shader.Get(), nullptr, 0);
		CONTEXT->DSSetShader(domain_shader.Get(), nullptr, 0);
		CONTEXT->GSSetShader(geomentry_shader.Get(), nullptr, 0);
		CONTEXT->PSSetShader(pixel_shader.Get(), nullptr, 0);

		CONTEXT->RSSetState(Device::GetInst()->GetRSState(rasterizer_type).Get());
		CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(depthstencil_type).Get(), 0);
		CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(blend_type).Get(), vec4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	}
}