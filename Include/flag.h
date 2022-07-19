#pragma once

enum class ETextureType
{
	Sprite,
	Frame
};


enum class ERenderLayer
{
	ScrollMap,
	Back,
	BackFront,
	RugnameLower,
	Default,
	RugnameUpper,
	Enemy,
	Slug,
	Player,
	EffectLow,
	EffectHigh,
	Max
};

enum class EReactionChannel
{	
	Normal,
	HitReaction,
	InvincibleReaction,
	Laser1,
	Laser2,
	Laser3,
	MAX
};


//�� ������Ʈ�� ���� ���� �ִϸ��̼��� ��� �������� �����Ƿ� ���� ���̾�� ����.
//���� ���� ���̾ ����ϴ� ���ӿ�����Ʈ�� ��� ���� Ŭ������ ����ü�� �����ؼ�
//���ϴ� �̸����� �����Ͽ� ����ص� �ɵ�.
enum class EAnimationLayer
{
	Layer1,
	Layer2,
	Layer3,
	Layer4,
	Layer5,
	Layer6,
	Max
};

enum class ECollisionChannel
{
	Default,
	BackAlly,
	BackEnemy,
	Player,
	Enemy,
	PlayerAttack,
	EnemyAttack,
	Mouse,
	Max
};


enum class EColliderType
{
	Box,
	Circle,
	Line
};

enum class EBrushType
{
	Black,
	Red,
	Green,
	Blue,
	Yellow,
	Max

};

enum class EEffectType
{
	Once,
	Duration,
	Loop
};

enum class EPlayType
{
	KeepPlay,
	Once,
	Duration
};

//��ư ��������Ʈ �̹��� ��¿� ����ü
enum class EButtonState
{
	Normal,
	MouseHovered,
	Click,
	Disabled,
	Max
};

//��ư �Ҹ� ����� ����ü
enum class EButtonSoundState
{
	MouseHovered,
	Click,
	Max
};

enum class EColInfo
{
	AnimSeqName,
	PlayTime,
	LoopFlag,
	Reverse,
	PlayScale,
	Layer,
	OffsetX,
	OffsetY,
	PivotX,
	PivotY,
	FramePosStart
};

enum class ELoopFlag
{
	NoLoop,
	Loop,
	LoopRound
};

enum class ERowInfo
{
	PathDescription,
	TexInfo,
	ColInfoDescription,
	AnimSeqInfoStart
};

enum class EDirection
{
	Left,
	Right,
	Mid,//���� �������� ����
	Max
};

enum class ETileOption
{
	Normal,
	Blocked
};

//���� ����Ʈ ���� �� ũ�� ���� �뵵
enum class EExplosionSize
{
	Back,
	Small,
	Midium,
	Big,
	Max
};

//WidgetTextImage ���� ��� ����
enum class EAlignMode
{
	Left,
	Mid,
	Right
};


enum class EItemList
{
	HMG,
	Rocket,
	Laser,
	Shotgun,
	Gas,
	Bomb,
	Carrot,
	Apple,
	BananaOne,
	BananaThree,
	Avocado,
	Cabbage,
	WaterMelon,
	Pig,
	Medal1,
	Medal2,
	Letter,
	DollBlue,
	DollRed,
	Coin,
	Key,
	Bone,
	End
};