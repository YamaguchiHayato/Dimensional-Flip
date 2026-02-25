#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

class TitleLogoLayer : public TitleLayerBase
{
public:
    TitleLogoLayer() = default;
    virtual ~TitleLogoLayer() = default;


public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;


// セッター。
public:
    // 表示フラグをセット。
    inline void SetVisible(bool isVisble)
    {
        isVisible_ = isVisble;
    }


// ゲッター。
public:
    // 画像をセット。
    inline const std::string Init(const std::string layerName) override
    {
        return TitleLayerBase::Init(layerName);
    }


private:
    bool isVisible_ = true;
};
