// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CStatusBar
	{
	private:
		 zBOOL toggle;

		 zCView* window;
		 zCView* vRight;
		 zCView* vLeft;
		 zCView* vUp;
		 zCView* vDown;

		 oCViewStatusBar* hpBar;
		 oCViewStatusBar* manaBar;
		 oCViewStatusBar* stamBar;
		 oCViewStatusBar* expBar;

	public:
		CStatusBar(int, int);
		~CStatusBar();

		 void GameRender();
		 void Close();
		 void Open();
	};
}