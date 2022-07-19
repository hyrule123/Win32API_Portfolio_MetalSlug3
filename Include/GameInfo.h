#pragma once

//이 헤더파일은 게임에서 공통으로 사용되는 내용들을 담아두고
//Include해서 사용할 목적으로 제작되었다.

//WIN32API
#include <Windows.h>

//링크드 리스트
#include <list>

//배열
#include <vector>

//맵
#include <map>

//연관 컨테이너
#include <unordered_map>

//메모리 누수 탐지
#include <crtdbg.h>

//벡터2 구조체
#include "Vector2.h"

//참조카운트
#include "SharedPtr.h"

//문자열
#include <string>


namespace std
{
#ifdef _UNICODE
	typedef wstring tstring;
#define tsPrintf_s swprintf_s
#define _tsplitpath_s _wsplitpath_s
#define tscanf_s wscanf_s
#else
	typedef tstring tstring;
#define tsPrintf_s sprintf_s
#define _tsplitpath_s _splitpath_s
#define tscanf_s sscanf_s
#endif
}
//또는
//typedef std::basic_string<TCHAR> tstring;



//타입 정보
#include <typeinfo>

//함수 포인터
#include <functional>

//정렬
#include <algorithm>

//스택
#include <stack>

//플래그
#include "flag.h"

#include "fmod.hpp"

//gdi +
//GDIPVER 재정의 후 include
#ifdef GDIPVER
#undef GDIPVER
#define GDIPVER 0x0110
#else
#define GDIPVER 0x0110
#endif

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")





//이미지 처리 라이브러리
#pragma comment(lib,"msimg32.lib")

//사운드 처리 라이브러리 FMOD
#pragma comment(lib, "../Bin/fmod64_vc.lib")

//게임 해상도
#define ORIGINAL_GAME_RES_WIDTH		304
#define ORIGINAL_GAME_RES_HEIGHT	224
#define SCREEN_SCALE				4

#ifdef _DEBUG
#define STATUS_SPACE		1.2f
#else
#define STATUS_SPACE		1.f
#endif




//중력
#define GRAVITY 9.8f


//경로 지정 매크로 설정
#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"
#define SOUND_PATH		"SoundPath"
#define FONT_PATH		"FontPath"
#define DATA_PATH		"DataPath"

#define DELTA_TIME CGameManager::GetInst()->GetDeltaTime()
#define DEBUG_RENDER CGameManager::GetInst()->GetDebugRender()
#define SAFE_DELETE(p) if(p){ delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p){ delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }

float DegreeToRadian(float Degree);
float RadianToDegree(float Radian);
float FastSqrt(const float& n);

template<typename T, typename U>
T* Dcast(U* u) { return dynamic_cast<T*>(u); }
template <typename T>
T* Dcast(nullptr_t) { return nullptr; }




//창의 해상도 저장
struct Resolution
{
	int Width;
	int Height;
};


//Sprite 방식 애니메이션의 시작점과 끝점을 저장하기 위한 구조체
//+ 픽셀 오차를 바로잡기위한 오프셋 값
//GameObect에서 렌더링할때 위치가 변경되어 출력됨
struct Sprite_FrameData
{
	Vector2 Start;
	Vector2 End;
	Vector2 Offset;
};





//박스 콜라이더 정보 저장용
struct BoxInfo
{
	Vector2 LT;
	Vector2 RB;
};

struct CircleInfo
{
	Vector2 Center;
	float Radius;
};

struct LineInfo
{
	Vector2 Start;
	Vector2 End;
};


struct CollisionProfile
{
	ECollisionChannel Channel;
	bool Enable;
	bool Interaction[(int)ECollisionChannel::Max];

	CollisionProfile() :
		Channel(),
		Enable(true)
	{
		//미리 기본 충돌 설정을 false로 해놓는다.
		for (int i = 0; i < (size_t)ECollisionChannel::Max; ++i)
		{
			Interaction[i] = false;
		}
	}
};

struct CooltimeChecker
{
	float Cooltime;		//시간 정하기용 변수
	float CooltimeLeft;	//시간 재기용 변수
	bool isCooltime;	//쿨타임 여부 확인

	CooltimeChecker(float cooltime = 5.f) :
		Cooltime(cooltime),
		isCooltime(false)
	{
		CooltimeLeft = Cooltime;
	}


	bool UpdateCooltime(float DeltaTime)	//시간을 업데이트하고, 진입 가능 여부만 알려줌
	{
		if (!isCooltime)
			return true;

		CooltimeLeft -= DeltaTime;
		if (CooltimeLeft < 0.f)
		{
			isCooltime = false;
		}
		return false;
	}

	bool EnterCooltime()	//쿨타임인지 확인하고 스킬 진입 가능 여부를 반환하며 쿨타임 적용까지 해줌
	{
		if (isCooltime)
			return false;

		CooltimeLeft = Cooltime;
		isCooltime = true;
		return true;
	}

	void InitCooltime(float CoolTime)
	{
		isCooltime = false;
		Cooltime = CoolTime;
		CooltimeLeft = CoolTime;
	}

	void KeepCooltime()
	{
		if (isCooltime)
		{
			CooltimeLeft = Cooltime;
		}

	}
};
