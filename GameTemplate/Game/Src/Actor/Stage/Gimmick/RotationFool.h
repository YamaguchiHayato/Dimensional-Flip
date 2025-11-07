#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

class RotationFool : public IGimmic 
{
public:
    RotationFool() {};
    virtual ~RotationFool() {};

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string InitGimmick(const std::string& gimmickname) override
    {
        return IGimmic::InitGimmick(gimmickname);
    };

// セッター。
public:
    const void SetRotFoolPosition(const Vector3& rotFoolPos)
	{
		gimmickPos_ = rotFoolPos;
	}

    inline void SetScale(const Vector3& scale)
    {
        scale_ = scale;
	}

private:
    Vector3 scale_ = Vector3::One;

private:
    Player* pPlayer = nullptr;  
};
