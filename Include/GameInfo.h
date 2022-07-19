#pragma once

//�� ��������� ���ӿ��� �������� ���Ǵ� ������� ��Ƶΰ�
//Include�ؼ� ����� �������� ���۵Ǿ���.

//WIN32API
#include <Windows.h>

//��ũ�� ����Ʈ
#include <list>

//�迭
#include <vector>

//��
#include <map>

//���� �����̳�
#include <unordered_map>

//�޸� ���� Ž��
#include <crtdbg.h>

//����2 ����ü
#include "Vector2.h"

//����ī��Ʈ
#include "SharedPtr.h"

//���ڿ�
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
//�Ǵ�
//typedef std::basic_string<TCHAR> tstring;



//Ÿ�� ����
#include <typeinfo>

//�Լ� ������
#include <functional>

//����
#include <algorithm>

//����
#include <stack>

//�÷���
#include "flag.h"

#include "fmod.hpp"

//gdi +
//GDIPVER ������ �� include
#ifdef GDIPVER
#undef GDIPVER
#define GDIPVER 0x0110
#else
#define GDIPVER 0x0110
#endif

#include <gdiplus.h>
#pragma comment(lib, "gdiplus")





//�̹��� ó�� ���̺귯��
#pragma comment(lib,"msimg32.lib")

//���� ó�� ���̺귯�� FMOD
#pragma comment(lib, "../Bin/fmod64_vc.lib")

//���� �ػ�
#define ORIGINAL_GAME_RES_WIDTH		304
#define ORIGINAL_GAME_RES_HEIGHT	224
#define SCREEN_SCALE				4

#ifdef _DEBUG
#define STATUS_SPACE		1.2f
#else
#define STATUS_SPACE		1.f
#endif




//�߷�
#define GRAVITY 9.8f


//��� ���� ��ũ�� ����
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




//â�� �ػ� ����
struct Resolution
{
	int Width;
	int Height;
};


//Sprite ��� �ִϸ��̼��� �������� ������ �����ϱ� ���� ����ü
//+ �ȼ� ������ �ٷ�������� ������ ��
//GameObect���� �������Ҷ� ��ġ�� ����Ǿ� ��µ�
struct Sprite_FrameData
{
	Vector2 Start;
	Vector2 End;
	Vector2 Offset;
};





//�ڽ� �ݶ��̴� ���� �����
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
		//�̸� �⺻ �浹 ������ false�� �س��´�.
		for (int i = 0; i < (size_t)ECollisionChannel::Max; ++i)
		{
			Interaction[i] = false;
		}
	}
};

struct CooltimeChecker
{
	float Cooltime;		//�ð� ���ϱ�� ����
	float CooltimeLeft;	//�ð� ���� ����
	bool isCooltime;	//��Ÿ�� ���� Ȯ��

	CooltimeChecker(float cooltime = 5.f) :
		Cooltime(cooltime),
		isCooltime(false)
	{
		CooltimeLeft = Cooltime;
	}


	bool UpdateCooltime(float DeltaTime)	//�ð��� ������Ʈ�ϰ�, ���� ���� ���θ� �˷���
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

	bool EnterCooltime()	//��Ÿ������ Ȯ���ϰ� ��ų ���� ���� ���θ� ��ȯ�ϸ� ��Ÿ�� ������� ����
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
