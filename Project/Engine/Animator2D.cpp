#include "pch.h"

#include "ptr.h"

#include "Animation2D.h"
#include "Material.h"
#include "MeshRender.h"

#include "Animator2D.h"
namespace ff7r
{
	Animator2D::Animator2D()
		: Component(COMPONENT_TYPE::ANIMATOR2D)
		, anim_map{}
		, cur_anim(nullptr)
		, is_repeat(false)
	{
	}

	Animator2D::~Animator2D()
	{
		Safe_Del_Map(anim_map);
	}

	void Animator2D::FinalTick()
	{
		if (nullptr != cur_anim)
		{
			if (is_repeat && cur_anim->IsFinish())
			{
				cur_anim->Reset();
			}

			cur_anim->FinalTick();
		}
	}

	void Animator2D::UpdateData()
	{
		Ptr<Material> mtrl = GetMeshRender()->GetMaterial();

		const Anim2DFrame& frm = cur_anim->GetCurFrame();
		vec2 back_size = cur_anim->GetBackSize();

		int anim_use = 1;
		mtrl->SetScalarParam(INT_0, &anim_use);
		mtrl->SetScalarParam(VEC2_0, &frm.left_top_uv);
		mtrl->SetScalarParam(VEC2_1, &frm.slice_uv);
		mtrl->SetScalarParam(VEC2_2, &frm.offset);
		mtrl->SetScalarParam(VEC2_3, &back_size);

		mtrl->SetTexParam(TEX_0, cur_anim->GetAtlasTex());
	}

	void Animator2D::Clear()
	{
		Ptr<Material> mtrl = GetMeshRender()->GetMaterial();

		int anim_use = 0;
		mtrl->SetScalarParam(INT_0, &anim_use);

		Ptr<Texture> tex = nullptr;
		mtrl->SetTexParam(TEX_0, tex);
	}

	void Animator2D::Play(const wstring& _anim_name, bool _is_repeat)
	{
		Animation2D* anim = FindAnim(_anim_name);
		assert(anim);

		cur_anim = anim;
		is_repeat = _is_repeat;
	}

	Animation2D* Animator2D::FindAnim(const wstring& _anim_name)
	{
		map<wstring, Animation2D*>::iterator iter = anim_map.find(_anim_name);

		if (iter == anim_map.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void Animator2D::CreateAnimation(const wstring& _anim_name, Ptr<Texture> _atlas_tex
		, vec2 _left_top, vec2 _slice, vec2 _back_size, int _frame_cnt, int _fps)
	{
		Animation2D* anim = new Animation2D;
		anim->Create(_anim_name, _atlas_tex, _left_top, _slice, _back_size, _frame_cnt, _fps);

		anim->owner = this;
		anim_map.insert(make_pair(_anim_name, anim));
	}

	void Animator2D::SaveToLevelFile(FILE* _file)
	{
		fwrite(&is_repeat, sizeof(bool), 1, _file);

		size_t AnimCount = anim_map.size();
		fwrite(&AnimCount, sizeof(size_t), 1, _file);

		for (const auto& pair : anim_map)
		{
			pair.second->SaveToLevelFile(_file);
		}

		wstring strCurAnimName;
		if (nullptr != cur_anim)
		{
			strCurAnimName = cur_anim->GetName();
		}
		SaveWString(strCurAnimName, _file);
	}

	void Animator2D::LoadFromLevelFile(FILE* _file)
	{
		fread(&is_repeat, sizeof(bool), 1, _file);

		size_t anim_cnt = 0;
		fread(&anim_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < anim_cnt; ++i)
		{
			Animation2D* anim = new Animation2D;
			anim->LoadFromLevelFile(_file);

			anim_map.insert(make_pair(anim->GetName(), anim));
			anim->owner = this;
		}

		wstring cur_anim_name;
		LoadWString(cur_anim_name, _file);

		cur_anim = FindAnim(cur_anim_name);
	}
}