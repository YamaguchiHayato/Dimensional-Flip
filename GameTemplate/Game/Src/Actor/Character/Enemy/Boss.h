#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

enum BossAnimation : uint8_t
{
    bossAnim_Idle = 0,
    bossAnim_Attack,
    bossAnim_Run,
    bossAnim_Hit,
    bossAnim_Num,
};

class Player;
class Boss : public IEnemy
{
public:
    Boss() =default;
    virtual ~Boss() = default;

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const virtual std::string InitModel(const std::string& enemyName)override
    {
        std::string enemyPath = "Assets/modelData/" + enemyName + ".tkm";
        return enemyPath;
    };

public:
    inline void SetPos(const Vector3& pos) { pos_ = pos; }
    inline void SetRot(const Quaternion& rot) { rot_ = rot; }

    inline Vector3 GetPos() const { return pos_; }
    inline Quaternion GetRot() const { return rot_; }

private:
    void Attack(Player* target);
    void PlayAnimatipn();
    void SetAnimation();
    void Rotaition();
    void Move();
    const std::string GetAnimation(BossAnimation, const std::string animationName, bool flag);

private:
    Player* pPlayer_ = nullptr;

private:
    AnimationClip animationClip_[BossAnimation::bossAnim_Num];
    ModelRender render_;

    Vector3 pos_ = Vector3::Zero;
    Vector3 moveSpeed_ = Vector3::Zero;
    Quaternion rot_ = Quaternion::Identity;

    uint8_t state_ = 0;
};
