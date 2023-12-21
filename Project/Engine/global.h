#pragma once

#include <d3d11.h>					
#include <d3dcompiler.h>			
#include <DirectXMath.h>			
#include <DirectXPackedVector.h>	

#include <assert.h>
#include <wrl.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <list>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;

using namespace std::experimental;
using namespace std::experimental::filesystem;

using namespace Microsoft::WRL;


#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;
typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;

#include "define.h"
#include "struct.h"
#include "func.h"
#include "Singleton.h"
