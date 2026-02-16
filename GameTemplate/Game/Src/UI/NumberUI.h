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


public:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;


// ゲッター。
public:
	inline virtual const std::string InitUI(const std::string& UIname) override{
		return UIBase::InitUI(UIname);
	};

    // タイマー開始フラグのゲッター。
    inline bool IsTimerStart() const
    {
        return isTimerStart_;
    }


    // タイマー更新処理。
	void UpdateTimer();
    // 数字UIの初期化。
	void InitUINumber();

    // タイマーリセット処理。
    // 各ステージ開始時に呼ぶ。
    inline void ResetTimer()
    {
        timer_ = 90.0f;
    }


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


    FontRender timerFont_;
    wchar_t timerText_[32];
};

