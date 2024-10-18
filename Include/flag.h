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


//한 오브젝트가 여러 장의 애니메이션을 들고 있을수도 있으므로 다중 레이어로 구성.
//여러 장의 레이어를 사용하는 게임오브젝트의 경우 각자 클래스에 열거체를 생성해서
//원하는 이름으로 지정하여 사용해도 될듯.
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

//버튼 스프라이트 이미지 출력용 열거체
enum class EButtonState
{
	Normal,
	MouseHovered,
	Click,
	Disabled,
	Max
};

//버튼 소리 재생용 열거체
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
	Mid,//우주 스테이지 전용
	Max
};

enum class ETileOption
{
	Normal,
	Blocked
};

//폭발 이팩트 생성 시 크기 지정 용도
enum class EExplosionSize
{
	Back,
	Small,
	Midium,
	Big,
	Max
};

//WidgetTextImage 정렬 모드 설정
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