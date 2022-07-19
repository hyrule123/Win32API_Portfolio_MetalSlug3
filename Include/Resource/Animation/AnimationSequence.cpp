#include "AnimationSequence.h"
#include "../Texture/Texture.h"

CAnimationSequence::CAnimationSequence() :
	m_Texture(nullptr),
	m_AnimLoaded()
{
}

CAnimationSequence::~CAnimationSequence()
{
}

CTexture* CAnimationSequence::GetTexture() const
{
	return m_Texture;
}

ETextureType CAnimationSequence::GetTextureType() const
{
	return m_Texture->GetTextureType();
}

//Frame의 사이즈 정보를 얻어오는 함수.
const Sprite_FrameData& CAnimationSequence::GetFrameData(int index) const
{
	return m_vecFrame[index];
}

std::vector<Sprite_FrameData> CAnimationSequence::GetFrameDataAll()
{
	return m_vecFrame;
}

size_t CAnimationSequence::GetFrameCount() const
{
	return m_vecFrame.size();
}

void CAnimationSequence::SetAnimLoaded(bool isLoaded)
{
	m_AnimLoaded = isLoaded;
}

bool CAnimationSequence::CheckAnimLoaded() const
{
	return m_AnimLoaded;
}

void CAnimationSequence::AddSpriteFrame(
	const Vector2& start, const Vector2& end, const Vector2& Offset)
{
	Sprite_FrameData Frame = {};


	Frame.Start = start;
	Frame.End = end;
	Frame.Offset = Offset;

	m_vecFrame.push_back(Frame);

}

void CAnimationSequence::AddSpriteFrame(
	float PosX, float PosY, 
	float SizeX, float SizeY, 
	float OffsetX, float OffsetY)
{
	Sprite_FrameData Frame = {};


	Frame.Start.x = PosX;
	Frame.Start.y = PosY;
	Frame.End.x = PosX + SizeX;
	Frame.End.y = PosY + SizeY;
	Frame.Offset.x = OffsetX;
	Frame.Offset.y = OffsetY;

	m_vecFrame.push_back(Frame);
}



void CAnimationSequence::AddFrameFrame()
{
	int FrameNum = m_Texture->GetTextureNumbers();

	for (int i = 0; i < FrameNum; ++i)
	{
		Vector2 size = m_Texture->GetTextureSize(i);

		Sprite_FrameData Data;

		//Vector2 구조체는 초기화될 떄 0, 0으로 초기화되므로 Start는 굳이 안해도 됨.
		Data.End = size;

		m_vecFrame.push_back(Data);
	}

}
