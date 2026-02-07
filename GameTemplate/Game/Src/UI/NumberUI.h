#pragma once
#include "Src/UI/UIBase.h"
#include "stdint.h"
#include <string>

namespace app{
    namespace core{
        class StageManager;
    }
}

class NumberUI : public UIBase
{
public:
	NumberUI() {};
	virtual ~NumberUI() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;
	inline virtual const std::string InitUI(const std::string& UIname) override{
		return UIBase::InitUI(UIname);
	};

    // タイマー更新処理。
	void UpdateTimer();
    // タイマー開始フラグのゲッター。
	bool IsTimerStart() const
	{
		return isTimerStart_;
	}
    // 数字UIの初期化。
	void InitUINumber();

    // タイマーリセット処理。
    // 各ステージ開始時に呼ぶ。
    void ResetTimer()
    {
        timer_ = 90.0f;

        // リセットした瞬間表示用の各桁も更新し直す。
        uint8_t remainingSeconds = static_cast<uint8_t>(timer_);
        tenPlace_ = (remainingSeconds / 10) % 10;
        onePlace_ = remainingSeconds % 10;
    }
private:

    // 数字用スプライトレンダー。
	SpriteRender UINumber_[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UINUmber_TensDigit[static_cast<int>(enUINumber::enNumber_Num)];
	SpriteRender UINUmber_OnesDigit[static_cast<int>(enUINumber::enNumber_Num)];

// 
private:
    // タイマーUI描画フラグ。
	bool isTimeUIDrawing_ = true;
    // タイマー開始フラグ。
	bool isTimerStart_ = false;
    // タイムアップフラグ。
	bool timeUpFlag_ = false;
    // タイマー。
	float timer_ = 90.0f;
    // 色を変化。
	float colorChange_ = 0.0f;
    // 10の位。
	uint8_t tenPlace_ = 0;
    // 1の位。
	uint8_t onePlace_ = 0;								

};

