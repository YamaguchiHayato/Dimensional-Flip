#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

class Player;   
class Box : public IGimmic
{
public:
    Box() {};
    virtual ~Box();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string InitGimmick(const std::string& gimmickname) override
    {
        return IGimmic::InitGimmick(gimmickname);
    };

public:
    inline void SetPos(const Vector3& pos)
    {
        gimmickPos_ = pos;
    };


private:
    Quaternion boxRot_ = Quaternion::Identity;
    Player* pPlayer = nullptr;

};
