#pragma once

#include <DirectXTex\DirectXTex.h>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

#include "Resource.h"

namespace ff7r
{
    class Texture : public Resource
    {
    public:
        Texture(bool _engine_resource = false);
        ~Texture();

        int Create(UINT _width, UINT height, DXGI_FORMAT _pixel_format, UINT _bind_flag, D3D11_USAGE _usage);
        int Create(ComPtr<ID3D11Texture2D> _tex2d);
    
        float GetWidth()    { return (float)desc.Width; }
        float GetHeight()   { return (float)desc.Height; }

        ComPtr<ID3D11Texture2D>             GetTex2D()  { return texture2d; }
        ComPtr<ID3D11ShaderResourceView>    GetSRV()    { return shader_resource_view; }
        ComPtr<ID3D11RenderTargetView>	    GetRTV()    { return render_target_view; }
        ComPtr<ID3D11DepthStencilView>	    GetDSV()    { return depth_stencil_veiw; }
        ComPtr<ID3D11UnorderedAccessView>   GetUAV()    { return unordered_access_view; }

        void        UpdateData(int _register_number, int _stage);
        void        UpdateDataCS(int _register_number, bool _is_shader_res);
        static void Clear(int _register_number);
        void        Clear();
        void        ClearCS(bool _is_shader_res);
        bool        IsCubeTex() { return desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? true : false; }

        virtual int Save(const wstring& _path) override;

    private:
        virtual int Load(const wstring& _path) override;
        virtual void UpdateData() override;

    private:
        ComPtr<ID3D11Texture2D>             texture2d;
        ComPtr<ID3D11ShaderResourceView>    shader_resource_view;
        ComPtr<ID3D11RenderTargetView>	    render_target_view;
        ComPtr<ID3D11DepthStencilView>	    depth_stencil_veiw;
        ComPtr<ID3D11UnorderedAccessView>   unordered_access_view;

        D3D11_TEXTURE2D_DESC                desc;
        ScratchImage                        image;

        UINT                                register_number;
    };
}
