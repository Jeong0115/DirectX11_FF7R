#include "pch.h"

#include "TimeMgr.h"

#include "Animation2D.h"
namespace ff7r
{
	Animation2D::Animation2D()
		: owner(nullptr)
		, cur_frame_idx(0)
		, is_finish(false)
		, time(0.f)
	{
	}

	Animation2D::~Animation2D()
	{
	}

	void Animation2D::FinalTick()
	{
		if (is_finish)
			return;

		time += DT;

		if (frames[cur_frame_idx].duration < time)
		{
			time = time - frames[cur_frame_idx].duration;
			++cur_frame_idx;

			if (frames.size() <= cur_frame_idx)
			{
				cur_frame_idx = frames.size() - 1;
				is_finish = true;
			}
		}
	}

	void Animation2D::Create(const wstring& _anim_name, Ptr<Texture> _atlas_tex
		, vec2 _left_top, vec2 _slice, vec2 _back_size, int _frame_cnt, int _fps)
	{
		assert(_atlas_tex.Get());

		SetName(_anim_name);

		atlas_tex = _atlas_tex;

		vec2 resolution = vec2(atlas_tex->GetWidth(), atlas_tex->GetHeight());
		back_size = _back_size / resolution;

		for (size_t i = 0; i < _frame_cnt; ++i)
		{
			Anim2DFrame frm = {};

			frm.duration = 1.f / (float)_fps;
			frm.left_top_uv = vec2(_left_top.x + _slice.x * i, _left_top.y) / resolution;
			frm.slice_uv = _slice / resolution;

			frames.push_back(frm);
		}
	}

	void Animation2D::SaveToLevelFile(FILE* _file)
	{
		SaveWString(GetName(), _file);

		size_t frame_cnt = frames.size();
		fwrite(&frame_cnt, sizeof(size_t), 1, _file);
		fwrite(frames.data(), sizeof(Anim2DFrame), frame_cnt, _file);
		fwrite(&back_size, sizeof(vec2), 1, _file);

		SaveResRef(atlas_tex.Get(), _file);
	}

	void Animation2D::LoadFromLevelFile(FILE* _file)
	{
		wstring name;
		LoadWString(name, _file);
		SetName(name);

		size_t frame_cnt = 0;
		fread(&frame_cnt, sizeof(size_t), 1, _file);

		for (size_t i = 0; i < frame_cnt; ++i)
		{
			Anim2DFrame frm = {};
			fread(&frm, sizeof(Anim2DFrame), 1, _file);
			frames.push_back(frm);
		}

		fread(&back_size, sizeof(vec2), 1, _file);

		LoadResRef(atlas_tex, _file);
	}
}