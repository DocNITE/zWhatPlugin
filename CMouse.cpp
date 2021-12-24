// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	Cursor::Cursor()
	{
		texture = new zCView(
			screen->anx(0),
			screen->any(0),
			screen->anx(16),
			screen->any(16));

		memset((void*)&m_State, 0, sizeof(m_State));
		m_pMouseDevice = GetMouseDevice();
		//texture->InsertBack("LO.TGA");
	}

	void Cursor::SetTexture(const zSTRING& filename)
	{
		texture->InsertBack(filename);
	}

	bool Cursor::IsLeftButtonPressed()
	{
		return zinput->GetMouseButtonPressedLeft();
	}

	bool Cursor::IsWheelPressed()
	{
		return zinput->GetMouseButtonPressedMid();
	}

	bool Cursor::IsRightButtonPressed()
	{
		return zinput->GetMouseButtonPressedRight();
	}

	void Cursor::Render()
	{
		static bool pressedLeft_before = false;
		static bool pressedRight_before = false;

		if (!visible)
		{
			return;
		}
		
		oldposition.x = position.x;
		oldposition.y = position.y;
		oldposition.wheel = position.wheel;

		if (m_pMouseDevice->GetDeviceState(sizeof(m_State), (LPVOID)&m_State) == DIERR_INPUTLOST)
			m_pMouseDevice->Acquire();

		position.x += m_State.lX * sensitivity * 5.0f;
		position.y += m_State.lY * sensitivity * ((float)VID_MAX_XDIM / VID_MAX_YDIM) * 5.0f;

		if (position.x > 8192.0f) position.x = 8192.0f;
		else if (position.x < 0.0f) position.x = 0.0f;

		if (position.y > 8192.0f) position.y = 8192.0f;
		else if (position.y < 0.0f) position.y = 0.0f;

		texture->SetPos(int(position.x), int(position.y));
		texture->Blit(); // show it on the screen

		bool pressedLeft = IsLeftButtonPressed();
		bool pressedRight = IsRightButtonPressed();
		if (pressedLeft_before != pressedLeft)
		{
			pressedLeft_before = pressedLeft;
		}
		else if (pressedRight_before != pressedRight)
		{
			pressedRight_before = pressedRight;
		}
	}
}