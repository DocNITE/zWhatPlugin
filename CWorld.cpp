// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	CWorld::CWorld()
	{
	
	}

	CWorld::~CWorld()
	{

	}

	void CWorld::InsertVob(zSTRING visual)
	{
		TVob newTVob;
		newTVob.visual = visual;

		ArrayVob.Insert(newTVob);
	}

	void CWorld::RemoveVob(zSTRING visual)
	{
		TVob newTVob;
		newTVob.visual = visual;

		ArrayVob.Insert(newTVob);
	}

	void CWorld::CreateVob(zSTRING visual, zVEC3 pos, zCQuat rot, zBOOL collision)
	{
		zCVob* vob = new zCVob();
	
		vob->SetVisual(visual);
		ogame->GetGameWorld()->AddVob(vob);
		vob->SetPositionWorld(pos);
		vob->SetRotationWorld(rot);
		vob->SetCollDet(collision);
	};

	void CWorld::CreateVob(zCVisual* visual, zVEC3 pos, zCQuat rot, zBOOL collision)
	{
		zCVob* vob = new zCVob();

		vob->SetVisual(visual);
		ogame->GetGameWorld()->AddVob(vob);
		vob->SetPositionWorld(pos);
		vob->SetRotationWorld(rot);
		vob->SetCollDet(collision);
	};
}