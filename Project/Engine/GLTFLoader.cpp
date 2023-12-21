#include "pch.h"
#include "GLTFLoader.h"
#include "ResourceMgr.h"

namespace ff7r
{
	GLTFLoader::GLTFLoader()
	{
	}

	GLTFLoader::~GLTFLoader()
	{
	}

	class StreamReader : public IStreamReader
	{
	public:
		StreamReader(path pathBase) : m_pathBase(std::move(pathBase))
		{
			// assert(m_pathBase.has_root_path());
		}

		// Resolves the relative URIs of any external resources declared in the glTF manifest
		std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
		{
			// In order to construct a valid stream:
			// 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
			//    correctly construct a path instance.
			// 2. Generate an absolute path by concatenating m_pathBase with the specified filename
			//    path. The filesystem::operator/ uses the platform's preferred directory separator
			//    if appropriate.
			// 3. Always open the file stream in binary mode. The glTF SDK will handle any text
			//    encoding issues for us.
			auto streamPath = m_pathBase / u8path(filename);
			auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

			// Check if the stream has no errors and is ready for I/O operations
			if (!stream || !(*stream))
			{
				throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
			}

			return stream;
		}

	private:
		path m_pathBase;
	};


	void GLTFLoader::LoadMesh(const path& _path)
	{
		//std::string directoryPath = filePath.substr(0, filePath.find_last_of("\\/") + 1);
		path a = _path.parent_path().root_path();

		auto streamReader = std::make_unique<StreamReader>(_path.parent_path());
		path pathFile = _path.filename();
		path pathFileExt = pathFile.extension();

		std::unique_ptr<GLTFResourceReader> resourceReader;

		//if (pathFileExt == MakePathExt(GLTF_EXTENSION));
		//if (pathFileExt == MakePathExt(GLB_EXTENSION));

		std::string manifest;

		auto gltfStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
		auto gltfResourceReader = std::make_unique<GLTFResourceReader>(std::move(streamReader));

		std::stringstream manifestStream;

		// Read the contents of the glTF file into a string using a std::stringstream
		manifestStream << gltfStream->rdbuf();
		manifest = manifestStream.str();

		resourceReader = std::move(gltfResourceReader);

		Document document;
		document = Deserialize(manifest);

		PrintDocumentInfo(document);
		PrintResourceInfo(document, *resourceReader, _path.stem());
	}

	void GLTFLoader::PrintDocumentInfo(const Document& document)
	{
		// Asset Info
		std::cout << "Asset Version:    " << document.asset.version << "\n";
		std::cout << "Asset MinVersion: " << document.asset.minVersion << "\n";
		std::cout << "Asset Generator:  " << document.asset.generator << "\n";
		std::cout << "Asset Copyright:  " << document.asset.copyright << "\n\n";

		// Scene Info
		std::cout << "Scene Count: " << document.scenes.Size() << "\n";

		if (document.scenes.Size() > 0U)
		{
			std::cout << "Default Scene Index: " << document.GetDefaultScene().id << "\n\n";
		}
		else
		{
			std::cout << "\n";
		}

		// Entity Info
		std::cout << "Node Count:     " << document.nodes.Size() << "\n";
		std::cout << "Camera Count:   " << document.cameras.Size() << "\n";
		std::cout << "Material Count: " << document.materials.Size() << "\n\n";

		// Mesh Info
		std::cout << "Mesh Count: " << document.meshes.Size() << "\n";
		std::cout << "Skin Count: " << document.skins.Size() << "\n\n";

		// Texture Info
		std::cout << "Image Count:   " << document.images.Size() << "\n";
		std::cout << "Texture Count: " << document.textures.Size() << "\n";
		std::cout << "Sampler Count: " << document.samplers.Size() << "\n\n";

		// Buffer Info
		std::cout << "Buffer Count:     " << document.buffers.Size() << "\n";
		std::cout << "BufferView Count: " << document.bufferViews.Size() << "\n";
		std::cout << "Accessor Count:   " << document.accessors.Size() << "\n\n";

		// Animation Info
		std::cout << "Animation Count: " << document.animations.Size() << "\n\n";

		for (const auto& extension : document.extensionsUsed)
		{
			std::cout << "Extension Used: " << extension << "\n";
		}

		if (!document.extensionsUsed.empty())
		{
			std::cout << "\n";
		}

		for (const auto& extension : document.extensionsRequired)
		{
			std::cout << "Extension Required: " << extension << "\n";
		}

		if (!document.extensionsRequired.empty())
		{
			std::cout << "\n";
		}
	}

	// Uses the Document and GLTFResourceReader classes to print information about various glTF binary resources
	void GLTFLoader::PrintResourceInfo(const Document& document, const GLTFResourceReader& resourceReader, wstring name)
	{
		vector<Vtx> vertexData;
		vector<UINT> index;

		vector<float> positionData;
		vector<float> normalData;
		vector<float> uvData;
		vector<uint32_t>  indexData_32;
		vector<uint16_t>  indexData_16;

		bool is_index_16 = true;

		int accumCnt = 0;
		int co = 0;
		// Use the resource reader to get each mesh primitive's position data
		for (const auto& mesh : document.meshes.Elements())
		{
			for (const auto& meshPrimitive : mesh.primitives)
			{
				std::string accessorId;

				if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_POSITION, accessorId))
				{
					const Accessor& accessor = document.accessors.Get(accessorId);

					positionData = resourceReader.ReadBinaryData<float>(document, accessor);
				}
				// 노말 데이터 추출
				std::string normalAccessorId;
				if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_NORMAL, normalAccessorId))
				{
					const Accessor& normalAccessor = document.accessors.Get(normalAccessorId);
					normalData = resourceReader.ReadBinaryData<float>(document, normalAccessor);
				}

				// UV 좌표 데이터 추출
				std::string uvAccessorId;
				if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_TEXCOORD_0, uvAccessorId))
				{
					const Accessor& uvAccessor = document.accessors.Get(uvAccessorId);
					uvData = resourceReader.ReadBinaryData<float>(document, uvAccessor);
				}

				// 인덱스 버퍼
				if (!meshPrimitive.indicesAccessorId.empty())
				{
					const Accessor& indexAccessor = document.accessors.Get(meshPrimitive.indicesAccessorId);
					

					if (indexAccessor.componentType == COMPONENT_UNSIGNED_SHORT)
					{
						is_index_16 = true;
						indexData_16 = resourceReader.ReadBinaryData<uint16_t>(document, indexAccessor);
					}
					else
					{
						is_index_16 = false;
						indexData_32 = resourceReader.ReadBinaryData<uint32_t>(document, indexAccessor);
					}
					// 정점에 연결된 본 인덱스(최대 4개까지)
					if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_JOINTS_0, uvAccessorId))
					{
						const Accessor& uvAccessor = document.accessors.Get(uvAccessorId);
						auto data = resourceReader.ReadBinaryData<uint16_t>(document, uvAccessor);
					}

					// 본에 대한 가중치
					if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_WEIGHTS_0, uvAccessorId))
					{
						const Accessor& uvAccessor = document.accessors.Get(uvAccessorId);
						auto data = resourceReader.ReadBinaryData<uint8_t>(document, uvAccessor);
					}

					int vertexCnt = 0;
					if ((positionData.size() / 3 == normalData.size() / 3) && (positionData.size() / 3 == uvData.size() / 2))
					{
						vertexCnt = (int)positionData.size() / 3;
					}
					else
					{
						assert(false);
					}

					for (int i = 0; i < vertexCnt; i++)
					{
						Vtx vertex;

						vertex.pos.x = positionData[i * 3];
						vertex.pos.y = positionData[i * 3 + 1];
						vertex.pos.z = positionData[i * 3 + 2];

						if (name == L"scene")
						{
							vertex.pos.x *= -1.f;
						}
						
						vertex.normal.x = normalData[i * 3];
						vertex.normal.y = normalData[i * 3 + 1];
						vertex.normal.z = normalData[i * 3 + 2];

						switch (co)
						{
						case 0:
							vertex.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
							break;
						case 1:
							vertex.color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
							break;
						case 2:
							vertex.color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
							break;
						default: break;
						}
						//vertex.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
						vertex.uv.x = uvData[i * 2];
						vertex.uv.y = uvData[i * 2 + 1];

						vertexData.push_back(vertex);
					}

					if(is_index_16)
					{
						for (int i = 0; i < (int)indexData_16.size(); i++)
						{
							index.push_back((UINT)indexData_16[i] + (UINT)accumCnt);
						}
						indexData_16.clear();
					}
					else
					{
						for (int i = 0; i < (int)indexData_32.size(); i++)
						{
							index.push_back((UINT)indexData_32[i] + (UINT)accumCnt);
						}
						indexData_32.clear();
					}

					accumCnt += vertexCnt;

					positionData.clear();
					normalData.clear();
					uvData.clear();
				}


			}

			ResourceMgr::GetInst()->CreatMeshFbx_Test(vertexData.data(), (UINT)vertexData.size(), index.data(), (UINT)index.size(), name + std::to_wstring(co));
			//vertexData.clear();
			//index.clear();

			//애니메이션 정보
			std::cout << "Animation Count: " << document.animations.Size() << "\n";

			for (const auto& animation : document.animations.Elements())
			{
				std::cout << "Animation: " << animation.name << "\n";

				for (const auto& sampler : animation.samplers.Elements()) {
					const Accessor& inputAccessor = document.accessors.Get(sampler.inputAccessorId);
					const auto& keyframeTimes = resourceReader.ReadBinaryData<float>(document, inputAccessor);

					std::cout << "Keyframe Times for Sampler " << sampler.id << ":";
					for (const auto& time : keyframeTimes) {
						std::cout << " " << time;
					}
					std::cout << std::endl;
				}

				for (const auto& channel : animation.channels.Elements())
				{
					const auto& node = document.nodes.Get(channel.target.nodeId); // target에서 nodeId 사용
					const auto& sampler = animation.samplers.Get(channel.samplerId);

					const Accessor& inputAccessor = document.accessors.Get(sampler.inputAccessorId);
					const Accessor& outputAccessor = document.accessors.Get(sampler.outputAccessorId);

					// 위치 데이터 읽기
					if (channel.target.path == TARGET_TRANSLATION)
					{
						const auto& positionData = resourceReader.ReadBinaryData<float>(document, outputAccessor);
						const auto& positionKeyFrame = resourceReader.ReadBinaryData<float>(document, inputAccessor);
						// positionData 처리
					}
					// 회전 데이터 읽기
					else if (channel.target.path == TARGET_ROTATION)
					{
						const auto& rotationData = resourceReader.ReadBinaryData<float>(document, outputAccessor);
						const auto& rotationKeyFrame = resourceReader.ReadBinaryData<float>(document, inputAccessor);

						// rotationData 처리
					}

					//const Accessor& outputAccessor = document.accessors.Get(sampler.outputAccessorId);

					std::cout << "Interpolation: " << sampler.interpolation << std::endl;
				}
			}

			//메테리얼 정보
			std::cout << "Material Count: " << document.materials.Size() << "\n";

			for (const auto& material : document.materials.Elements()) {
				std::cout << "Material: " << material.name << "\n";
				// 여기에 재질의 다른 속성들을 출력할 수 있습니다.
				// 예: material.pbrMetallicRoughness.baseColorFactor, material.emissiveTexture, 등

				std::cout << "\n";
			}

			//본 정보
			std::cout << "Skin Count: " << document.skins.Size() << "\n";
			for (const auto& skin : document.skins.Elements()) {
				std::cout << "Skin: " << skin.name << "\n";
				std::cout << "  Inverse Bind Matrices: " << skin.inverseBindMatricesAccessorId << "\n";
				std::cout << "  Skeleton Root Node: " << skin.skeletonId << "\n";

				for (const auto& jointId : skin.jointIds)
				{
					std::cout << "  Joint: " << jointId << "\n";

					const Node& jointNode = document.nodes.Get(jointId);

					// 본의 위치 정보
					const auto& translation = jointNode.translation;
					std::cout << "Bone Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")\n";

					// 본의 회전 정보
					const auto& rotation = jointNode.rotation;
					std::cout << "Bone Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << ")\n";

					// 본의 크기 정보
					const auto& scale = jointNode.scale;
					std::cout << "Bone Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")\n";
				}

				std::cout << "\n";
			}

			co++;

			// Use the resource reader to get each image's data
			for (const auto& image : document.images.Elements())
			{
				std::string filename;

				if (image.uri.empty())
				{
					assert(!image.bufferViewId.empty());

					auto& bufferView = document.bufferViews.Get(image.bufferViewId);
					auto& buffer = document.buffers.Get(bufferView.bufferId);

					filename += buffer.uri; //NOTE: buffer uri is empty if image is stored in GLB binary chunk
				}
				else if (IsUriBase64(image.uri))
				{
					filename = "Data URI";
				}
				else
				{
					filename = image.uri;
				}

				auto data = resourceReader.ReadBinaryData(document, image);

				std::cout << "Image: " << image.id << "\n";
				std::cout << "Image: " << data.size() << " bytes of image data\n";

				if (!filename.empty())
				{
					std::cout << "Image filename: " << filename << "\n\n";
				}
			}
		}
	}
}