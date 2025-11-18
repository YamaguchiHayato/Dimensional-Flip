#pragma once
#include "stdint.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"

enum class ThwompState : uint8_t
{
    Idle,
    Fall,
    Return,
    Erase,
    Spawn
};

class Player;

/// 斜面の上を移動しながらクルクル回転するトゲ敵
class Thwomp : public IGameObject
{
public:
    Thwomp() = default;
    virtual ~Thwomp() = default;

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    inline const std::string InitModel(const std::string& gimmickname)
    {
        return "Assets/stage/" + gimmickname + ".tkm";
    }

//====================
// セッター
//====================
public:
    void InitPos(const Vector3& pos);
    void InitMoveDir(const Vector3& dir);
    inline void SetMoveSpeed(float speed)
    {
        moveSpeed_ = speed;
    }

//====================
// 内部処理
//====================
private:
    void UpdateMove(float deltaTime);

//====================
// メンバー変数
//====================
private:
    Player* pPlayer_ = nullptr;
    CollisionObject* pCollisionObject_ = nullptr;
    ModelRender render_;
    Quaternion rot_ = Quaternion::Identity; // 回転
    Vector3 pos_ = Vector3::Zero;          // 位置
    Vector3 initPos_ = Vector3::Zero;

    Vector3 moveDir_ = Vector3::Zero;     // 移動方向
    Vector3 velocity_ = Vector3::Zero;    // (将来使うかも)
    float moveSpeed_ = 200.0f;            // 移動スピード

    float spinSpeedDeg_ = 180.0f;           // 1秒間に180度回転するスピード
    bool isRespawning_ = false;            // 描画フラグ
};
// ★★★ ここにあった余計な } を削除しました ★★★
