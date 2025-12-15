#pragma once
#include "Src/Actor/Character/Status.h"

enum PlayerState : uint8_t
{
    sIdle,
    sRun,
    sJump,
    sNum,
};

enum EnAnimationClip : uint8_t
{
    animIdle,
    animJump,
    animRun,
    animNum,
};

class CameraManager;
class Player : public IGameObject
{
public:
    Player() = default;
    virtual ~Player() = default;

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rendercontext) override;
    bool DoJumpCheck() const { return didJumpThisFrame_; };

    void Action();
    //
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

private:
    ////////////////////////////
    // 移動処理をまとめる処理 // 
    void Move();
    ////////////////////////////
    ////////////////////////////

    // @TODO : 後ほど、ステートマシン化する予定。//

    // 移動処理を分割。//
    // XZ成分の移動を管轄。
    void MoveHorizontal();
    // ジャンプ処理を管轄。
    void UpdateJumpAndGravity();
    // 移動処理を加える。
    void ApplyMovement();

    void Rotation();
    void PlayAnimation();
    void ManageState();
    void AddMoveSpeed(const Vector3& addMoveSpeed) { moveSpeed_ += addMoveSpeed; }
    void Move3Dmode();
    void Move2_5Dmode();

    // 3Dカメラと2Dカメラの切り替え
    void ChangeDimensionCamera();

    // リスポーン処理。
    void ReSpwan();


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
    inline void SetPaused(bool isPaused) { isPaused_ = isPaused; }
    // CameraMaangerの初期化。
    inline void InitCameraMnager(CameraManager* pCameraManager)
    {
        pCameraManager_ = pCameraManager;
    }
    // ステージタイプに合わせてパラメータを設定する
    void SetStageParam(bool isStageEX);
    // リスポーン座標の設定。
    void SetRespwanPos(const Vector3& pos)
    {
        respwanPos_ = pos;
    }

    // ゲッター。
public:
    // ジャンプ力の取得。
    inline const float& GetJumpPower() const { return jumpPower_; }
    // プレイヤーのCC取得。
    inline CharacterController& GetPlayerCC() { return charaCon_; }
    // プレイヤーの座標の取得。
    inline const Vector3 GetPlayerPos() const { return pos_; }
    // プレイヤーの前方向ベクトル取得。
    inline const Vector3 GetForward() const { return forward_; }
    // トリガーエリア内かどうかの取得。
    inline bool GetInTriggerArea() const { return triggerOverlapCount_ > 0; }
    // カメラモードを取得。
    inline CameraManager* GetCameraManager() { return pCameraManager_; }
    // リスポーンしたか。
    inline bool IsRespawn()
    {
        return respawnFlag_;
    }
    // 現在の移動速度を取得。
    inline const Vector3& GetMoveSpeed() const
    {
        return moveSpeed_;
    }

private:
    // アニメーションを取得して再生する関数。
    const std::string FetchPlayAnimation(EnAnimationClip enAnimationClip, const std::string& animationName, bool flag);
    // モデルを取得して再生する関数。
    const std::string FetchPlayerModel(const std::string& modelName, AnimationClip animationClip,EnAnimationClip enAnimationClip, EnModelUpAxis enModelUpAxis, bool flag);
    // アニメーションを格納
    void SetAnimation();

private:
    ::Player* player_ = nullptr;
    CameraManager* pCameraManager_ = nullptr;
    app::status::Status status_;

public:
    AnimationClip animationClip_[EnAnimationClip::animNum];
    CharacterController charaCon_;

    ModelRender render_;
    Vector3 diff_ = Vector3::Zero;
    Vector3 moveSpeed_ = Vector3::Zero;
    Vector3 pos_ = Vector3::Zero;
    Vector3 respwanPos_ = Vector3::Zero;
    Vector3 forward_ = Vector3::Front;
    Quaternion rot_ = Quaternion::Identity;
    Quaternion respwanRot_ = Quaternion::Identity;

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
};
