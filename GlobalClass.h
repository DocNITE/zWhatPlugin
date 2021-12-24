// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

	typedef zVEC2 zPOINT2;
	typedef zVEC3 zPOINT3;
	typedef zMAT4 zMATRIX4;

	const zSTRING SndMaterialName[6] = {
	"WOOD", "STONE", "METAL", "LEATHER", "CLAY", "GLAS"
	};
	
	Cursor* cursor;
	CStatusBar* statusBar;
	CWorld* worldVob;

	template <class T>
	inline void zClamp(T& x, const T min, const T max) {
		if (x < min) x = min; else
			if (x > max) x = max;
	}
}