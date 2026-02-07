/*
 * @file TimerUI.h
 * @dote 10/06 TimerUI 最終更新日。
 */

#pragma once
#include "Src/UI/UIBase.h"
class TimerUI : public UIBase
{
public:
	TimerUI() = default;
	virtual ~TimerUI() = default;


public:
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc)override;


public:
	inline const std::string InitUI(const std::string& UIname) override{
		return UIBase::InitUI(UIname);
	};


public:
    void Reset();

};

