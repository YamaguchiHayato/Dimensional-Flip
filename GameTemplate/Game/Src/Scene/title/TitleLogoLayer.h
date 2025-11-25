#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

class TitleLogoLayer : public TitleLayerBase
{
public:
    TitleLogoLayer() = default;
    virtual ~TitleLogoLayer() = default;
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string Init(const std::string layerName) override
    {
        return TitleLayerBase::Init(layerName);
    }

private:
    SpriteRender pressButtonRender_; // プレスボタンレンダラー。
};
