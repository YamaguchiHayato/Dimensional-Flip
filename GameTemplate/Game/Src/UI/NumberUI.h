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
	NumberUI() = default;
	virtual ~NumberUI() = default;


public:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;


public:
    // タイマー更新処理。
    void UpdateTimer();
    // 数字UIの初期化。
    void InitUINumber();


// ゲッター。
public:
    // UIの初期化。
	inline virtual const std::string InitUI(const std::string& UIname) override{
		return UIBase::InitUI(UIname);
	};

    // タイマー開始フラグのゲッター。
    inline bool IsTimerStart() const
    {
        return isTimerStart_;
    }

    // タイムを取得する。
    inline float GetTimer() const
    {
        return timer_;
    }

// セッター。
public:
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


private:
    static NumberUI* instance_;


public:
    //
    inline static NumberUI* GetInstance()
    {
        return instance_;
    }
};

