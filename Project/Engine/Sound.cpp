#include "pch.h"
#include "Sound.h"

#include "ResourceMgr.h"

namespace ff7r
{
	FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);
	FMOD::System* Sound::System = nullptr;

	Sound::Sound()
		: Resource(RES_TYPE::SOUND)
		, sound(nullptr)
	{
	}

	Sound::~Sound()
	{
		if (nullptr != sound)
		{
			sound->release();
			sound = nullptr;
		}
	}

	int Sound::Play(int _roopcount, float _volume, bool _overlap)
	{
		if (_roopcount <= -1)
		{
			assert(nullptr);
		}

		// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
		if (!_overlap && !channel_list.empty())
		{
			return -1;
		}

		_roopcount -= 1;

		FMOD::Channel* channel = nullptr;
		System->playSound(sound, nullptr, false, &channel);

		channel->setVolume(_volume);
		channel->setCallback(CHANNEL_CALLBACK);
		channel->setUserData(this);
		channel->setMode(FMOD_LOOP_NORMAL);
		channel->setLoopCount(_roopcount);

		channel_list.push_back(channel);

		int idx = -1;
		channel->getIndex(&idx);

		return idx;
	}

	void Sound::Stop()
	{
		list<FMOD::Channel*>::iterator iter;

		while (!channel_list.empty())
		{
			iter = channel_list.begin();
			(*iter)->stop();
		}
	}

	void Sound::SetVolume(float _volume, int _channel_idx)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();

		int iIdx = -1;
		for (; iter != channel_list.end(); ++iter)
		{
			(*iter)->getIndex(&iIdx);
			if (_channel_idx == iIdx)
			{
				(*iter)->setVolume(_volume);
				return;
			}
		}
	}

	void Sound::RemoveChannel(FMOD::Channel* _channel)
	{
		list<FMOD::Channel*>::iterator iter = channel_list.begin();
		for (; iter != channel_list.end(); ++iter)
		{
			if (*iter == _channel)
			{
				channel_list.erase(iter);
				return;
			}
		}
	}

	int Sound::Load(const wstring& _path)
	{
		string path(_path.begin(), _path.end());

		if (FMOD_OK != System->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound))
		{
			assert(nullptr);
		}

		return S_OK;
	}

	// =========
	// Call Back
	// =========
	FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2)
	{
		FMOD::Channel* channel = (FMOD::Channel*)channelcontrol;
		Sound* sound = nullptr;

		switch (controltype)
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
		{
			channel->getUserData((void**)&sound);
			sound->RemoveChannel(channel);
		}
		break;

		default: break;
		}

		return FMOD_OK;
	}
}


