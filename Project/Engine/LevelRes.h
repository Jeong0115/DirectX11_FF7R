#pragma once

#include "Resource.h"

namespace ff7r
{
	class LevelRes : public Resource
	{
	public:
		LevelRes();
		virtual ~LevelRes();

		virtual int Save(const wstring&) override { return S_OK; }

	private:
		virtual int Load(const wstring& _strFilePath) override;
		virtual void UpdateData() override {}
	};
}
