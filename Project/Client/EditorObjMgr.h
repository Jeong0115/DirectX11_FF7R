#pragma once

namespace ff7r
{
	class GameObjectEx;
	class TransformTool;
	class EditorObjMgr : public Singleton<EditorObjMgr>
	{
		SINGLE(EditorObjMgr);

	private:
		vector<GameObjectEx*>	m_vecEditorObj;
		TransformTool*			transform_tool;

		GameObjectEx* m_DebugShape[(UINT)SHAPE_TYPE::END];
		vector<DebugShapeInfo> m_DebugShapeInfo;

	public:
		void Init();
		void Progress();

		GameObjectEx* GetEditorCamera() { return m_vecEditorObj[0]; }
		void SetTransformTarget(GameObject* _object);

	private:
		void Tick();
		void Render();
	};
}
