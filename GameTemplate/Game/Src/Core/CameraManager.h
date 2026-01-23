#pragma once
#include <memory>

#include "Src/Camera/ICameraStrategy.h"

// 前方宣言
class Player;
class Boss;

// カメラモード列挙型
enum class CameraMode : uint8_t
{
    mode2D,    // 横スクロール
    mode3D,      // 3D自由視点
    modeBoss,    // ボス戦演出
};


class CameraManager : public IGameObject
{
public:
    CameraManager() = default;
    virtual ~CameraManager() = default;


public:
    bool Start() override;
    void Update() override;


public:
    // カメラモード。
    void Request2DMode();
    void Request3DMode();
    void RequestBossMode(float targetAngleDegrees = 0.0f);


// セッター。
public:
    // カメラの移動範囲を設定。
    void SetCameraRange(const Vector3& min, const Vector3& max);


    // ボタンアクションの有効/無効設定。
    inline void SetButtonActionControl(bool buttonAction)
    {
        isButtonControlFlag = buttonAction;
    }

private:
    template <typename CameraType>
    void RequestCameraMode(const float angle, CameraMode cameraMode, const nsK2EngineLow::Camera::EnUpdateProjMatrixFunc mode);


// ゲッター
public:
    // 現在のカメラモードを取得。
    inline CameraMode GetCurrentCameraMode() const
    {
        return currentMode_;
    }

    // 現在のカメラストラテジーを取得。
    inline ICameraStrategy* GetActiveStrategy() const
    {
        return pCameraStrategy_.get();
    }

private:
    Player* pPlayer_ = nullptr;
    std::unique_ptr<ICameraStrategy> pCameraStrategy_;
    CameraMode currentMode_ = CameraMode::mode2D;


private:
    // カメラ移動範囲。
    bool isLimitSet_ = false;
    Vector3 limitMin_ = Vector3::Zero;
    Vector3 limitMax_ = Vector3::Zero;


    // ボタンアクションの許可フラグ。
    bool isButtonControlFlag = true;
};

