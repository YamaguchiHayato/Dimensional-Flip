#pragma once

#include "Src/Actor/Character/IState.h"
#include "Src/Actor/Character/Status.h"
#include "Src/Actor/Character/Character.h"

#include "Src/Core/CameraManager.h"

#include "Src/Actor/Character/Player/PlayerIdleState.h"
#include "Src/Actor/Character/Player/PlayerRunState.h"
#include "Src/Actor/Character/Player/PlayerJumpState.h"
#include "Src/Actor/Character/Player/PlayerFallState.h"


enum PlayerState : uint8_t
{
    sIdle,
    sRun,
    sJump,
    sNum,
};


enum EnPlayerState : uint8_t
{
    enState_Idle,
    enState_Run,
    enState_Jump,
    enState_Fall,
    enState_Num,
};


enum EnAnimationClip : uint8_t
{
    animIdle,
    animJump,
    animRun,
    animNum,
};


class IState;
class CameraManager;
class Player : public Character
{
public:
    Player() = default;
    virtual ~Player() = default;

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rendercontext) override;

    bool IsDimensionSwitchAction();

    // 敵を踏みつけた際に行うバウンド処理。
    inline void Bound()
    {
        moveSpeed_.y = jumpPower_;
        moveSpeed_.x = 0.0f;
        moveSpeed_.z = 0.0f;

        state_ = PlayerState::sJump;

        // バウンド時は空中での操作を可能にする。
        canAirControl_ = true;
    }

    // ジャンプ処理を管轄。
    void UpdateJumpAndGravity();
    // 移動処理を加える。
    void ApplyMovement();

    void Rotation();
    void AddMoveSpeed(const Vector3& addMoveSpeed) { moveSpeed_ += addMoveSpeed; }


    // セッター。
public:
    // CameraManagerの初期化。
    inline void InitCameraManager(CameraManager* pCameraManager) { pCameraManager_ = pCameraManager; }


    // ジャンプ力の設定。
    inline void SetJumpPower(float jumpPower) { jumpPower_ = jumpPower; }


    // トリガーエリア内で呼ばれる処理。
    inline void EnterTriggerArea() { triggerOverlapCount_++; }


    // トリガーエリア外で呼ばれる処理。
    inline void ExitTriggerArea()
    {
        // 減算処理。
        triggerOverlapCount_--;
        // 0未満にならないように補正。
        if (triggerOverlapCount_ < 0)
            triggerOverlapCount_ = 0;
    }


    // 座標。
    inline void SetPlayerPos(const Vector3& pos)
    {
        // 座標設定。
        pos_ = pos;
        // キャラコンを移動。
        charaCon_.SetPosition(pos_);
        // モデルの移動。
        render_.SetPosition(pos_);
    }


    // 一時停止フラグ。
    inline void SetPaused(bool isPaused)
    {
        isPaused_ = isPaused;
    }


    // ステージタイプに合わせてパラメータを設定する
    void SetStageParam(bool isStageEX);


    // リスポーン座標の設定。
    inline void SetRespwanPos(const Vector3& pos)
    {
        respwanPos_ = pos;
    }


    // 空中での操作が可能かどうかの設定。
    inline void SetCanAirControl(bool flag)
    {
        canAirControl_ = flag;
    }


    // 今フレームでジャンプしたかどうかの設定。
    inline void SetJumpedThisFrame(bool flag)
    {
        didJumpThisFrame_ = flag;
    }


    // 回転軸。
    inline void SetRotation(const Quaternion& rot)
    {
        rot_ = rot;
        render_.SetRotation(rot_);
    }


    // リスポーンフラグのセット。
    inline void SetRespawnFlag(bool flag)
    {
        respawnFlag_ = flag;
    }

    // ゲッター。
public:
    // ジャンプ力の取得。
    inline const float& GetJumpPower() const
    {
        return jumpPower_;
    }


    // プレイヤーの座標の取得。
    inline const Vector3& GetPlayerPos() const
    {
        return pos_;
    }


    // プレイヤーの前方向ベクトル取得。
    inline const Vector3 GetForward() const
    {
        return forward_;
    }


    // トリガーエリア内かどうかの取得。
    inline bool GetInTriggerArea() const
    {
        return triggerOverlapCount_ > 0;
    }


    // カメラモードを取得。
    inline CameraManager* GetCameraManager()
    {
        return pCameraManager_;
    }


    // リスポーンしたか。
    inline bool IsRespawn()
    {
        return respawnFlag_;
    }


    // walkSpeedの取得。
    inline const float GetWalkSpeed() const
    {
        return walkSpeed_;
    }


    // 今フレームでジャンプしたかどうか。
    inline bool DoJumpCheck() const
    {
        return didJumpThisFrame_;
    };


    // リスポーン座標の取得。
    inline const Vector3& GetRespwanPos() const
    {
        return respwanPos_;
    }


    // リスポーン時の回転軸の取得。
    inline const Quaternion& GetRespwanRot() const
    {
        return respwanRot_;
    }


    // 
 private:
    // アニメーションを取得して再生する関数。
    const std::string FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName, bool flag);
    // モデルを取得して再生する関数。
    const std::string FetchPlayerModel(const std::string& modelName, AnimationClip animationClip,EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag);
    // アニメーションを格納
    void SetAnimation();

private:
    Player* player_ = nullptr;
    CameraManager* pCameraManager_ = nullptr;
    app::status::Status status_;


public:
    AnimationClip animationClip_[EnAnimationClip::animNum];

    Vector3 diff_ = Vector3::Zero;
    Vector3 moveSpeed_ = Vector3::Zero;
    Vector3 pos_ = Vector3::Zero;
    Vector3 respwanPos_ = Vector3::Zero;
    Vector3 forward_ = Vector3::Front;
    Quaternion rot_ = Quaternion::Identity;
    Quaternion respwanRot_ = Quaternion::Identity;
    FontRender posFont_;

private:
    uint8_t state_;
    // トリガーエリア内フラグ
    uint8_t triggerOverlapCount_ = 0; /// いくつのエリアに重なっているかカウント。

    float jumpPower_ = 0.0f;
    float walkSpeed_ = 0.0f;

    bool canAirControl_ = false;
    bool didJumpThisFrame_ = false;
    bool isPaused_ = false;
    bool is3DMode_ = false;
    bool respawnFlag_ = false;






    //////////////////////////////////////////////////
public:
    friend app::state::PlayerIdleState;
    friend app::state::PlayerRunState;
    friend app::state::PlayerJumpState;
    friend app::state::PlayerFallState;

public:
    ModelRender render_;
    CharacterController charaCon_;

private:
private:
    // ステートマシン用
    IState* pCurrentState_ = nullptr;
    IState* pNextState = nullptr;
    IState* pStateArray_[enState_Num];
    // std::map<uint8_t,IState*> pStateArray_;

public:
    // モデルレンダーのゲッター
    inline ModelRender& GetModelRender() { return render_; }
    // キャラクターコントローラーのゲッター
    inline CharacterController& GetCharacterController() { return charaCon_; }
    // 移動速度のゲッター
    inline Vector3& GetMoveSpeed() { return moveSpeed_; }

public:
    /**
     * ステートの追加
     */
    template <typename T> void RegisterState(EnPlayerState state)
    {
        pStateArray_[state] = new T(this);
    }
};
