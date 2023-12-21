#include "pch.h"
#include "KeyMgr.h"

#include "GameEngine.h"


namespace ff7r
{
	int Mouse_Wheel_Value = 0;
	int key_type[(UINT)KEY::END]
		=
	{
		 VK_UP,
		 VK_DOWN,
		 VK_LEFT,
		 VK_RIGHT,

		 VK_SPACE,
		 VK_RETURN,
		 VK_ESCAPE,
		 VK_LMENU,
		 VK_LCONTROL,
		 VK_LSHIFT,
		 VK_TAB,

		 VK_LBUTTON,
		 VK_RBUTTON,

		 'Q',
		 'W',
		 'E',
		 'R',

		 'T',
		 'Y',
		 'U',
		 'I',
		 'O',
		 'P',

		 'A',
		 'S',
		 'D',
		 'F',

		 'Z',
		 'X',
		 'C',
		 'V',



		 '0',
		 '1',
		 '2',
		 '3',
		 '4',
		 '5',
		 '6',
		 '7',
		 '8',
		 '9',

	};

	KeyMgr::KeyMgr()
		: mouse_wheel(0)
	{
	}

	KeyMgr::~KeyMgr()
	{
	}

	void KeyMgr::Init()
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			key_info.push_back(KeyInfo{ (KEY)i  , KEY_STATE::NONE });
		}
	}

	void KeyMgr::Tick()
	{
		if (GetFocus())
		{
			mouse_wheel = Mouse_Wheel_Value;
			Mouse_Wheel_Value = 0;

			for (size_t i = 0; i < key_info.size(); ++i)
			{
				if (GetAsyncKeyState(key_type[(UINT)key_info[i].key]) & 0x8000)
				{
					// 이전에는 눌리지 않았다.
					if (false == key_info[i].prev)
					{
						key_info[i].state = KEY_STATE::TAP;
						key_info[i].prev = true;
					}
					else
					{
						// 지금도 눌려있고, 이전 프레임에서도 눌려있었다.
						key_info[i].state = KEY_STATE::PRESSED;
					}
				}
				else
				{
					// 눌려있지 않다.
					if (false == key_info[i].prev)
					{
						key_info[i].state = KEY_STATE::NONE;
					}
					else
					{
						key_info[i].state = KEY_STATE::RELEASE;
						key_info[i].prev = false;
					}
				}
			}

			// Mouse 위치 갱신
			//m_vPrevMousePos = m_vMousePos;

			//POINT ptMousePos = {};
			//GetCursorPos(&ptMousePos);		
			//ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
			//m_vMousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);

			//m_vMouseDir = m_vMousePos - m_vPrevMousePos;
			//m_vMouseDir.y *= -1;

			mouse_pos_prev = mouse_pos;

			POINT mouse_pos_cur = {};
			GetCursorPos(&mouse_pos_cur);
			ScreenToClient(Engine::GetInst()->GetMainWnd(), &mouse_pos_cur);
			mouse_pos = vec2((float)mouse_pos_cur.x, (float)mouse_pos_cur.y);

			mouse_dir = mouse_pos - mouse_pos_prev;
			mouse_dir.y *= -1;
		}

		// Window 가 focus 상태가 아니다
		else
		{
			Mouse_Wheel_Value = mouse_wheel = 0;
			mouse_dir = vec2(0.f, 0.f); 
			for (size_t i = 0; i < key_info.size(); ++i)
			{
				if (KEY_STATE::TAP == key_info[i].state || KEY_STATE::PRESSED == key_info[i].state)
				{
					key_info[i].state = KEY_STATE::RELEASE;
				}

				else if (KEY_STATE::RELEASE == key_info[i].state)
				{
					key_info[i].state = KEY_STATE::NONE;
				}
			}
		}
		

		
		
		
		

		
		
		//mouse_dir.Normalize();
	}
}