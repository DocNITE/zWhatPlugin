// Supported with union (c) 2020 Union team
// Union HEADER file
#include <stdint.h>
#include "dx7sdk/inc/dinput.h"

namespace GOTHIC_ENGINE {
	class Cursor
	{
	public:
		 Cursor();

		 void SetVisible(bool toggle)
		{
			 if (toggle == false)
			 {
				 screen->RemoveItem(texture);
			 }
			 else if (toggle == true)
			 {
				 screen->InsertItem(texture);
			 }
			visible = toggle;
		}
		 void SetPosition(int32_t x, int32_t y)
		{
			position.x = x;
			position.y = y;
		}
		 void SetSensitivity(float sens)
		{
			sensitivity = sens;
		}
		 void SetTexture(const zSTRING& filename);
		 bool IsLeftButtonPressed();
		 bool IsWheelPressed();
		 bool IsRightButtonPressed();
		 void Render();

		 bool visible = false;
		 struct OldPosition
		{
			zREAL x = 4096;
			zREAL y = 4096;
			zREAL wheel = 0;
		} oldposition;
		 struct Position
		{
			 zREAL x = 4096;
			 zREAL y = 4096;
			 zREAL wheel = 0;
		} position;
		 float sensitivity = 1.0;
		 zCView* texture;
		 LPDIRECTINPUTDEVICEA m_pMouseDevice;
		 DIMOUSESTATE2 m_State;
	private:
		inline static LPDIRECTINPUTDEVICEA GetMouseDevice() { return *(LPDIRECTINPUTDEVICEA*)0x008D1D70; };
	};
}