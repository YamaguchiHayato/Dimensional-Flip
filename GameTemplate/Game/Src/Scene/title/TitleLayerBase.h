#pragma once

class TitleLayerBase : public IGameObject
{
public:
    TitleLayerBase() {};
    virtual ~TitleLayerBase() {};

    // 初期化処理。
    virtual bool Start() = 0 { return true; };
    // 更新処理。
    virtual void Update() = 0 {};;
    // 描画処理。
    virtual void Render(RenderContext& rc) = 0 {};
    // 画像取得。
    inline virtual const std::string Init(const std::string layerNane) = 0
    {
        std::string titleLayerPath = "Assets/title/" + layerNane + ".DDS";
        return titleLayerPath;
    }

protected:
    SpriteRender titleLayerRender_;
};

