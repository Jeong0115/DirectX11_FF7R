#pragma once

namespace ff7r
{
	class TimeMgr : public Singleton<TimeMgr>
	{
		SINGLE(TimeMgr);
	public:
		void Init();
		void Tick();
		void Render();

		float GetDeltaTime() { return delta_time; }

	private:
		LARGE_INTEGER	prev_count;
		LARGE_INTEGER	cur_count;
		LARGE_INTEGER	frequency;

		UINT			call_count;
		float			delta_time;
		float			accumulated_time;
	};
}	
