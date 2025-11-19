#pragma once
#include "Src/Actor/Stage/IStage.h"

class Stage3 : public IStage
{
public:
    Stage3() {};
    virtual ~Stage3() {};

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    // モデルのファイルパスをセット。
    inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};

private:
    ModelRender render_;
};
