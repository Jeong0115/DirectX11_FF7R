#pragma once

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper"))
#endif

namespace ff7r
{
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

	class FontMgr : public Singleton<FontMgr>
	{
	public:
		FontMgr();
		~FontMgr();

		void Init();
		void DrawFont(const wchar_t* _str, float _pos_x, float _pos_y, float _font_size, UINT _color);

	private:
		IFW1Factory*		fw1_factory;
		IFW1FontWrapper*	font_wrapper;
	};
}