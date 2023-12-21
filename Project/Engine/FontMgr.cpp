#include "pch.h"

#include "Device.h"

#include "FontMgr.h"
namespace ff7r
{
	FontMgr::FontMgr()
		: fw1_factory(nullptr)
		, font_wrapper(nullptr)
	{
	}

	FontMgr::~FontMgr()
	{
		if (nullptr != fw1_factory)
			fw1_factory->Release();

		if (nullptr != font_wrapper)
			font_wrapper->Release();
	}

	void FontMgr::Init()
	{
		if (FAILED(FW1CreateFactory(FW1_VERSION, &fw1_factory)))
		{
			assert(NULL);
		}

		if (FAILED(fw1_factory->CreateFontWrapper(DEVICE, L"Arial", &font_wrapper)))
		{
			assert(NULL);
		}
	}

	void FontMgr::DrawFont(const wchar_t* _str, float _pos_x, float _pos_y, float _font_size, UINT _color)
	{
		font_wrapper->DrawString( CONTEXT, _str, _font_size, _pos_x, _pos_y, _color, FW1_RESTORESTATE);
	}
}
