#pragma once
#include "Resource.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

namespace ff7r
{
	class Sound : public Resource
	{
	public:
		Sound();
		virtual ~Sound();

		int				Play(int _roopcount, float _volume = 1.f, bool _overlap = false);
		void			Stop();
		void			SetVolume(float _volume, int _channel_idx);
		virtual void	UpdateData() {}

		virtual int		Save(const wstring&) override { return S_OK; }
		virtual int		Load(const wstring& _path) override;

		static FMOD::System* System;

	private:
		void RemoveChannel(FMOD::Channel* _channel);
		friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
			, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

		FMOD::Sound*			sound;
		list<FMOD::Channel*>	channel_list;
	};
}
