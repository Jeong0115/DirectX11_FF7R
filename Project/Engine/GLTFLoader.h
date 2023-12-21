#pragma once

#include "GLTFSDK/GLTF.h"
#include "GLTFSDK/GLBResourceReader.h"
#include "GLTFSDK/GLTFResourceReader.h"
#include "GLTFSDK/Deserialize.h"

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#pragma comment(lib, "GLTFSDK/GLTFSDK.lib")
#else
#pragma comment(lib, "GLTFSDK/GLTFSDK.lib")
#endif

using namespace Microsoft::glTF;

namespace ff7r
{
	class GLTFLoader : public Singleton<GLTFLoader>
	{
	private:

	public:
		void LoadMesh(const path& _path);
		void PrintDocumentInfo(const Document& document);
		void PrintResourceInfo(const Document& document, const GLTFResourceReader& resourceReader, wstring name);

	public:
		GLTFLoader();
		~GLTFLoader();
	};
}
