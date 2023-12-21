#pragma once

#include "UI.h"
namespace ff7r
{
	class AddComponentUI
		: public UI
	{
	private:
		class ListUI* mCompList;

	public:
		virtual void Init() override;;
		virtual void FinalTick() override;
		virtual int RenderUpdate() override;

	public:
		AddComponentUI();
		virtual ~AddComponentUI();
	};

}