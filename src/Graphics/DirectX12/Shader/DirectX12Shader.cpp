#include "DirectX12Shader.hpp"
#include "FileManagement.hpp"
#include <d3dcompiler.h>
#include "Log.hpp"

using Microsoft::WRL::ComPtr;

namespace EngineCore
{
    static const std::string LOGGER_TAG = "DirectX12 Shader";

    DirectX12Shader::DirectX12Shader(ID3D12Device* d, const std::string& fp)
    {
        name = fp.substr(fp.find_last_of('/') + 1);

        namespace FM = FileManagement;

        std::vector<std::pair<std::string, std::string>> fps;
        if (!FM::getFilesInFolder(&fps, fp))
        {
            Log::error(LOGGER_TAG, "Failed to load shader from: ", fp);
            shaderErrorCode = ShaderErrors::SHADER_FAILED_LOAD;
            return;
        }
        
        std::wstring vertPath, fragPath;
        std::string vPath, fPath;
        for (const std::pair<std::string, std::string>& e : fps)
        {
            if (e.second == ".hlsl_ps") 
            {
                fragPath = std::wstring(e.first.begin(), e.first.end());
                fPath = e.first;
            }
            else if (e.second == ".hlsl_vs") 
            {
                vertPath = std::wstring(e.first.begin(), e.first.end());
                vPath = e.first;
            }
        }
       
        if (!compileStage(d, vertPath, "VSMain", "vs_5_1", vertexBlob))
        {
            shaderErrorCode = ShaderErrors::SHADER_FAILED_COMPILE;
            Log::error(LOGGER_TAG, "Vertex Shader Compile Failed: ", vPath);
            return;
        }

        if (!compileStage(d, fragPath, "PSMain", "ps_5_1", pixelBlob))
        {
            shaderErrorCode = ShaderErrors::SHADER_FAILED_COMPILE;
            Log::error(LOGGER_TAG, "Fragment Shader Compile Failed: ", fPath);
            return;
        }
        bool rootCreate = createRootSigniture(d);
        if (!rootCreate)
        {
            shaderErrorCode = ShaderErrors::SHADER_FAILED_LINK;
            Log::error(LOGGER_TAG, "Root signiture creation failed");
            return;
        }

        Log::info(LOGGER_TAG, "DirectX12 Shader loaded: ", name);
    }

    bool DirectX12Shader::compileStage(ID3D12Device* d, const std::wstring& path, const std::string& entryPoint,
                                        const std::string& target, ComPtr<ID3DBlob>& out)
    {
        UINT compileFlags = 0;
#ifdef _DEBUG
        compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ComPtr<ID3DBlob> error;
        HRESULT res = D3DCompileFromFile(
            path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.c_str(), target.c_str(), compileFlags,
            0, &out, &error
        );

        if (FAILED(res))
        {
            if (error)
            {
                Log::error(LOGGER_TAG, "HLSL ERROR: ", static_cast<const char*>(error->GetBufferPointer()));
            }
            else
            {
                Log::error(LOGGER_TAG, "HLSL ERROR: No error blob. Check file location");
            }
            return false;
        }

        return true;
    }

    D3D12_ROOT_PARAMETER1 DirectX12Shader::createRootParameter(D3D12_ROOT_PARAMETER_TYPE t, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY v)
    {
        D3D12_ROOT_PARAMETER1 p = {};
        p.ParameterType = t;
        p.Descriptor.ShaderRegister = shaderRegister;
        p.Descriptor.RegisterSpace = registerSpace;
        p.ShaderVisibility = v;
        return p;
    }

    bool DirectX12Shader::createRootSigniture(ID3D12Device* d)
    {
        //ROOT SIGNITURE:
        //b0 = camera UBO
        //b1 = model matrix
        //b2 = test register (to be replaced with DrawData)
        //t0 = model matricies

        D3D12_ROOT_PARAMETER1 rootParam[3] = {};
        
        //camera (b0)
        rootParam[0] = createRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL);

        //DrawData (b1)
        rootParam[1] = createRootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL);

        //models storage (t0)
        D3D12_DESCRIPTOR_RANGE1 srvRange = {};
        srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        srvRange.NumDescriptors = 1;
        srvRange.BaseShaderRegister = 0;
        srvRange.RegisterSpace = 0;
        srvRange.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;

        rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParam[2].DescriptorTable.NumDescriptorRanges = 1;
        rootParam[2].DescriptorTable.pDescriptorRanges = &srvRange;
        rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsd = {};
        rsd.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
        rsd.Desc_1_1.NumParameters = 3;
        rsd.Desc_1_1.pParameters = rootParam;
        rsd.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ComPtr<ID3DBlob> signiture;
        ComPtr<ID3DBlob> error;
        HRESULT res = D3D12SerializeVersionedRootSignature(&rsd, &signiture, &error);

        if (FAILED(res))
        {
            if (error)
            {
                Log::error(LOGGER_TAG, "Root Signiture serialisation error: ", static_cast<const char*>(error->GetBufferPointer()));
            }
            else
            {
                Log::error(LOGGER_TAG, "Root Signiture serialisation error, no error blob!");
            }
            return false;
        }

        res = d->CreateRootSignature(
            0, 
            signiture->GetBufferPointer(),
            signiture->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature)
        );
        bool createdRoot = SUCCEEDED(res);

        return createdRoot;
    }

    const D3D12_SHADER_BYTECODE DirectX12Shader::getVertexBytecode() const
    {
        D3D12_SHADER_BYTECODE code = {};
        if (vertexBlob)
        {
            code.pShaderBytecode = vertexBlob->GetBufferPointer();
            code.BytecodeLength = vertexBlob->GetBufferSize();
        }
        return code;
    }

    const D3D12_SHADER_BYTECODE DirectX12Shader::getFragmentBytecode() const
    {
        D3D12_SHADER_BYTECODE code = {};
        if (pixelBlob)
        {
            code.pShaderBytecode = pixelBlob->GetBufferPointer();
            code.BytecodeLength = pixelBlob->GetBufferSize();
        }
        return code;
    }
}