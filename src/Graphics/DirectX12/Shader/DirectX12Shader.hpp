#pragma once
#include "IShader.hpp"
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>
#include <string>

namespace EngineCore
{
    class DirectX12Shader : public IShader
    {
    public:
        DirectX12Shader() = default;
        DirectX12Shader(ID3D12Device* device, const std::string& fp);

        int load(const std::string& vp, const std::string& fp);
    
        ID3D12RootSignature* getRootSignatire() const { return rootSignature.Get(); }
        const D3D12_SHADER_BYTECODE getVertexBytecode() const;
        const D3D12_SHADER_BYTECODE getFragmentBytecode() const;
    private:

        bool compileStage(ID3D12Device* device, const std::wstring& path, const std::string& entryPoint,
                            const std::string& targe, Microsoft::WRL::ComPtr<ID3DBlob>& out);   
        bool createRootSigniture(ID3D12Device* device);
        
        std::string name;
        ShaderErrors shaderErrorCode = ShaderErrors::SHADER_OK;

        Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;
        Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
    };
}