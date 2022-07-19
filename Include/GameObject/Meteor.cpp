#include "Meteor.h"

CMeteor::CMeteor()
{
	SetTypeID<CMeteor>();
}

CMeteor::CMeteor(const CMeteor& Obj) :
	CEnemy(Obj)
{

}

CMeteor::~CMeteor()
{



}

bool CMeteor::LoadResource()
{
	if (!LoadCSVPreset(TEXT("Enemy/Meteor.csv")))
		return false;



	return true;
}

bool CMeteor::Init(CGameObject* Obj)
{
	CEnemy::Init(Obj);

	//하단만 컬링 제거 기능 활성화
	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::Bottom, true);

	return true;
}


