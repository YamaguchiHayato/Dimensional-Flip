#pragma once
#include "Src/Actor/Stage/IStage.h"

class Player;
class Stage2 : public IStage
{
public:
	Stage2() {};
	virtual ~Stage2();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
    // モデルのファイルパスをセット。
    inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};


private:
    Player* pPlayer_ = nullptr;
};

