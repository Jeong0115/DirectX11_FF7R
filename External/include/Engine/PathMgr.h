#pragma once

namespace ff7r
{
	class PathMgr : public Singleton<PathMgr>
	{
		SINGLE(PathMgr);
	public:
		void Init();
		const wchar_t* GetContentPath() { return content_path; }

	private:
		wchar_t		content_path[256];
	};
}

