#pragma once

class Fade : public IGameObject
{
public:
    Fade() {};
    virtual ~Fade() {};

    bool Start();
    void Update();
    void Render(RenderContext& rc);

    void LoadingProcess();
    void FadeTransition(FadeState fadeState);

// ゲッター。
public:
    inline FadeState GetFadeState() const { return fadeState_; }
    inline SpriteRender& GetFadeSprite() { return fadeSprite_; }
    inline bool IsFadeInEnd() const { return fadeInEnd_; }
    inline bool IsFadeOutEnd() const { return fadeOutEnd_; }
// ゲッターここまで。

private:
    void ChangeFadeState();

private:
    FadeState fadeState_ = FadeState::FadeStart;
    SpriteRender fadeSprite_;
    SpriteRender loadingSprite_;
    Sprite sprite_;

private:
    uint8_t fadeCounter_ = 0;

    bool fadeInEnd_ = false;
    bool fadeOutEnd_ = false;
    bool fadeTransitionFlag_ = false;
    bool loadingFlag_ = false;	
};
