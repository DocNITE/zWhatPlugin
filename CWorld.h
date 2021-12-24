// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class CWorld
	{
	private:
		struct TVob
		{
			zSTRING visual;
		}TypeVob;

		zCArray<TVob> ArrayVob;
	public:
		CWorld();
		~CWorld();

		void InsertVob(zSTRING visual);
		void RemoveVob(zSTRING visual);
		void CreateVob(zSTRING visual, zVEC3 pos, zCQuat rot, zBOOL collision);
		void CreateVob(zCVisual* visual, zVEC3 pos, zCQuat rot, zBOOL collision);
	};
}