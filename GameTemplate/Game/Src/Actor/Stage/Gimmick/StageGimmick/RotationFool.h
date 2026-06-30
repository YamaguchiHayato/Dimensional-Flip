#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

enum class FoolState : uint8_t
{
    UP,
    TOP,
    DOWN,
    BOTTOM
};

class RotationFool : public IGimmic
{
public:
    RotationFool() {};
    virtual ~RotationFool();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string InitGimmick(const std::string& gimmickname) override
    {
        return IGimmic::InitGimmick(gimmickname);
    };

private:
    void Move(float deltaTime);
    void InitPhysics();
    void SyncPhysicsPosition();

public:
    inline const void SetStopTime(const float& stopTime) { stopTime_ = stopTime; }
    inline const void SetInitPos(const Vector3& initPos) { initGimmickPos_ = initPos; }
    inline const void SetTopPos(const Vector3& topPos) { topPos_ = topPos; }
    inline const void SetPos(const Vector3& rotFoolPos) { gimmickPos_ = rotFoolPos; }
    inline void SetScale(const Vector3& scale) { scale_ = scale; }
    inline const void SetMoveSpeed(const float& moveSpeed) { moveSpeed_ = moveSpeed; }
    inline void SetDirection(const Quaternion& direction) { direction_ = direction; }

    inline const Vector3 GetPos() const { return gimmickPos_; }
    inline const Vector3 GetTopPos() const { return topPos_; }
    inline const Vector3 GetInitPos() const { return initGimmickPos_; }

private:
    Vector3 scale_ = Vector3::One;
    Vector3 topPos_ = Vector3::Zero;
    Player* pPlayer = nullptr;
    float moveSpeed_ = 100.0f;
    float stopTime_ = 2.0f;
    FoolState foolState_ = FoolState::TOP;
    Quaternion direction_ = Quaternion::Identity;
};
