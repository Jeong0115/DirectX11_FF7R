#pragma once


namespace ff7r
{
	class Engine : public Singleton<Engine>
	{
	public:
		Engine();
		~Engine();

		int		Init(HWND _hWnd, UINT _width, UINT _height);
		void	Progress();

		vec2	GetWindowResolution() { return resolutioin; }
		HWND	GetMainWnd() { return hWnd; }

	private:
		void	Tick();
		void	Render();

		HWND	hWnd;
		vec2	resolutioin;
	};
}

