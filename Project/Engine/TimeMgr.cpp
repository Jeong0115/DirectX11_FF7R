#include "pch.h"
#include "TimeMgr.h"

#include "GameEngine.h"
#include "FontMgr.h"

namespace ff7r
{
	TimeMgr::TimeMgr()
		: prev_count{}
		, cur_count{}
		, frequency{}
		, call_count(0)
		, delta_time(0.f)
		, accumulated_time(0.f)
	{
	}

	TimeMgr::~TimeMgr()
	{
	}

	void TimeMgr::Init()
	{
		// 1초당 카운팅 증가량
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&cur_count);
		QueryPerformanceCounter(&prev_count);
	}

	void TimeMgr::Tick()
	{
		QueryPerformanceCounter(&cur_count);

		// tick 간격 시간
		delta_time = (float)(cur_count.QuadPart - prev_count.QuadPart) / (float)frequency.QuadPart;

		// 누적 시간
		accumulated_time += delta_time;

		// 함수 호출 횟수
		++call_count;

		// 이전 카운트 값을 현재 카운트로 갱신
		prev_count = cur_count;

		// GlobalData 갱신
		GlobalData.delta_time = delta_time;
		GlobalData.accumulated_time += delta_time;
	}

	void TimeMgr::Render()
	{
		static wchar_t szBuff[256] = {};

		if (1.f <= accumulated_time)
		{
			swprintf_s(szBuff, L"FPS : %d, DT : %f", call_count, delta_time);
			//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);	

			accumulated_time = 0.f;
			call_count = 0;
		}

		FontMgr::GetInst()->DrawFont(szBuff, 10, 20, 16, FONT_RGBA(255, 0, 0, 255));
	}
}