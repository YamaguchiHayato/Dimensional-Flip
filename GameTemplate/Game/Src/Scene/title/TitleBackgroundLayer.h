#pragma once
#include "Src/Scene/title/TitleLayerBase.h"
class TitleBackgroundLayer : public TitleLayerBase
{
public:
    TitleBackgroundLayer() = default;
    virtual ~TitleBackgroundLayer() = default;

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    // 画像取得。
    inline const std::string Init(const std::string layerNane) override
    {
        return TitleLayerBase::Init(layerNane);
    }

private:
    // 一定周期で回転させる。
    void Rotation();

private:
    SpriteRender blackSpriteRender_;        // 黒背景スプライトレンダラー。
    Quaternion rot_ = Quaternion::Identity; // 回転情報
};
