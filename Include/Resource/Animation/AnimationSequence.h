#pragma once

#include "../../Ref.h"
#include "../../GameInfo.h"

class CAnimationSequence :
    public CRef
{
    friend class CAnimationManager;

    //�����⿡�� �����Ӱ� ������ �� �ֵ���
    friend class CSceneEdit;

private:
    CSharedPtr<class CTexture> m_Texture;
    std::vector<Sprite_FrameData> m_vecFrame;

    CAnimationSequence();
    ~CAnimationSequence();


public:
    class CTexture* GetTexture()   const;
    ETextureType GetTextureType()   const;
    const Sprite_FrameData& GetFrameData(int index)   const;
    std::vector<Sprite_FrameData> GetFrameDataAll();
    bool m_AnimLoaded;
    size_t GetFrameCount() const;
    

private:
    void SetAnimLoaded(bool isLoaded = true);
    bool CheckAnimLoaded() const;
    //Sprite ������ �ؽ�ó Ÿ���� ����� �� �������� ����� �޼ҵ�
    void AddSpriteFrame(const Vector2& start, const Vector2& end, const Vector2& Offset);
    void AddSpriteFrame(float PosX, float PosY, float SizeX, float SizeY, float OffsetX = 0.f, float OffsetY = 0.f);


    //Frame ������ �ؽ�ó Ÿ���� ����� �� �ִϸ��̼� �������� �̹��� ����� �°� �ڵ� �������ִ� �޼ҵ�.
    void AddFrameFrame();
    



};

