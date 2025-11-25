#pragma once
#include "Src/Scene/title/TitleLayerBase.h"
class TitleButtonActionLayer : public TitleLayerBase
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline const std::string Init(const std::string layerName)override
    {
        return TitleLayerBase::Init(layerName);
    }

private:       
    // ボタンの点滅処理。
    void Blinking();


private:
    SpriteRender howToPlayText_;
    SpriteRender endText_;
};  
