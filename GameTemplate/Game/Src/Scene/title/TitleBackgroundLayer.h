#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

enum class FlipState : uint8_t
{
    Idle,      // 待機中。      (ゆっくりと回転している状態)
    PreFlip,   // 呼び回転動作。(高速回転に入る前の状態)
    Flipping,  // 反転中。      (高速回転中。)
    PostFlip,  // 反転完了。    (回転直後の余韻。)
};



class TitleBackgroundLayer : public TitleLayerBase
{
public:
    TitleBackgroundLayer() = default;
    virtual ~TitleBackgroundLayer() = default;


public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

// セッター。
public:
    // マニュアルモード。
    inline void SetManualMode(bool isManual)
    {
        isManualMode_ = isManual;
    }


// ゲッター。
public:
    // 画像取得。
    inline const std::string Init(const std::string layerNane) override
    {
        return TitleLayerBase::Init(layerNane);
    }

    // イージング処理。
    // 予備動作用。
    inline float EaseInBack(float t)
    {
        return t * t;
    }

    // 本回転用。
    inline float EaseInOutCubic(float t)
    {
        return t < 0.5f ? 4.0f * t * t * t : 1.0 - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    } 


private:
    // 回転パラメータの初期化。
    void InitRotParam();

    // 一定周期で回転させる。
    void Rotation();



private:
    SpriteRender blackSpriteRender_;        // 黒背景スプライトレンダラー。

    Quaternion rot_ = Quaternion::Identity; // 回転情報

    FlipState state_ = FlipState::Idle;     // 現在の反転状態。

    float currentAngle_ = 0.0f;             // 回転開始角度。
    float stateTimer_ = 0.0f;               // 状態遷移用タイマー。
    float startAngle_ = 0.0f;               // 補完用 : 開始角度。
    float targetAngle_ = 0.0f;              // 補完用 : 目標角度。

    bool isManualMode_ = false;
};
