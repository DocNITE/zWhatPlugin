// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	string WIN_TEX = "INV_BACK.TGA";
	string WIN_VERT_TEX = "INV_TITLE.TGA";
	string BAR_BACK = "INV_TITEL.TGA";
	string BAR_BACK2 = "MENU_CHOICE_BACK.TGA";
	string BAR_TEX = "WHITE.TGA";

	zCOLOR expRGBA = zCOLOR(139, 0, 230, 170);
	zCOLOR hpRGBA = zCOLOR(230, 0, 0, 170);
	zCOLOR manaRGBA = zCOLOR(0, 0, 230, 170);
	zCOLOR stamRGBA = zCOLOR(0, 230, 0, 170);

	// Status window size
	static int winSizeX = 400;
	static int winSizeY = 80;

	static int expSizeX = 400;
	static int expSizeY = 10;

	static int hpSizeX = 135;
	static int hpSizeY = 19;

	CStatusBar::CStatusBar(int xscr, int yscr)
	{
		window = new zCView(zPixelX(xscr), zPixelY(yscr), zPixelX(xscr + winSizeX), zPixelY(yscr + winSizeY));
		window->InsertBack(WIN_TEX);
		/*vRight = new zCView(window->anx(0), window->any(0), window->anx(0 + winSizeX + 1), window->any(0 + winSizeY));
		vRight->InsertBack(WIN_VERT_TEX);
		window->InsertItem(vRight);
		vLeft = new zCView(window->anx(0+ winSizeX-1), window->any(0), window->anx(0 + winSizeX + 1), window->any(0 + winSizeY));
		vLeft->InsertBack(WIN_VERT_TEX);
		window->InsertItem(vLeft);
		vUp = new zCView(window->anx(0), window->any(0), window->anx(0 + winSizeX), window->any(0 + winSizeY + 1));
		vUp->InsertBack(WIN_VERT_TEX);
		window->InsertItem(vUp);
		vDown = new zCView(window->anx(0), window->any(0 + winSizeY - 1), window->anx(0+winSizeX), window->any(0+ winSizeY + 1));
		vDown->InsertBack(WIN_VERT_TEX);
		window->InsertItem(vDown);*/

		expBar = new oCViewStatusBar();

		expBar->Init(zPixelX(0), zPixelY(0), 1.0F);
		expBar->SetSize(window->anx(expSizeX), window->any(expSizeY));
		//expBar->SetColor(expRGBA);
		expBar->SetTextures(BAR_BACK, "BAR_tempmax.tga", "WHITE.TGA", "");
		expBar->value_bar->SetColor(expRGBA);
		expBar->value_bar->SetTransparency(100);
		expBar->value_bar->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
		window->InsertItem(expBar);

		hpBar = new oCViewStatusBar();

		hpBar->Init(window->anx(0), window->any(0+ expSizeY), 1.0F);
		hpBar->SetSize(window->anx(hpSizeX), window->any(hpSizeY));
		//hpBar->SetColor(hpRGBA);
		hpBar->SetTextures(BAR_BACK, "BAR_tempmax.tga", "BAR_HEALTH.TGA", "");
		hpBar->value_bar->SetTransparency(210);
		hpBar->value_bar->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
		window->InsertItem(hpBar);

		manaBar = new oCViewStatusBar();

		manaBar->Init(window->anx(0+ hpSizeX+hpSizeX ), window->any(0 + expSizeY), 1.0F);
		manaBar->SetSize(window->anx(hpSizeX-5), window->any(hpSizeY));
		//manaBar->SetColor(manaRGBA);
		manaBar->SetTextures(BAR_BACK, "BAR_tempmax.tga", "BAR_MANA.TGA", "");
		manaBar->value_bar->SetTransparency(210);
		manaBar->value_bar->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
		window->InsertItem(manaBar);

		stamBar = new oCViewStatusBar();

		stamBar->Init(window->anx(0+ hpSizeX), window->any(0 + expSizeY), 1.0F);
		stamBar->SetSize(window->anx(hpSizeX), window->any(hpSizeY));
		//stamBar->SetColor(stamRGBA);
		stamBar->SetTextures(BAR_BACK, "BAR_tempmax.tga", "BAR_MISC.TGA", "");
		stamBar->value_bar->SetTransparency(210);
		stamBar->value_bar->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
		window->InsertItem(stamBar);
	};
	CStatusBar::~CStatusBar()
	{
		zDELETE(window);
	};
	void CStatusBar::GameRender()
	{
		int currExp = player->experience_points;
		int currExpMax = player->experience_points_next_level;

		expBar->SetRange(0, currExpMax);
		expBar->SetMaxRange(0, currExpMax);
		expBar->SetValue(currExp);

		int currHp = player->attribute[NPC_ATR_HITPOINTS];
		int currHpMax = player->attribute[NPC_ATR_HITPOINTSMAX];

		hpBar->SetRange(0, currHpMax);
		hpBar->SetMaxRange(0, currHpMax);
		hpBar->SetValue(currHp);

		int currMana = player->attribute[NPC_ATR_MANA];
		int currManaMax = player->attribute[NPC_ATR_MANAMAX];

		manaBar->SetRange(0, currManaMax);
		manaBar->SetMaxRange(0, currManaMax);
		manaBar->SetValue(currMana);

		int currStam = 100;
		int currStamMax = 100;

		stamBar->SetRange(0, currStamMax);
		stamBar->SetMaxRange(0, currStamMax);
		stamBar->SetValue(currStam);
	};
	void CStatusBar::Close()
	{
		if (!toggle) return;

		screen->RemoveItem(window);
		toggle = false;
	};
	void CStatusBar::Open()
	{
		if (toggle) return;

		screen->InsertItem(window);
		toggle = true;
	};
}