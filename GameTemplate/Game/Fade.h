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
    FadeState GetFadeState() const { return fadeState_; }
    SpriteRender& GetFadeSprite() { return fadeSprite_; }
    bool IsFadeEnd() const { return fadeInEnd_; }

private:
    void ChangeFadeState();

private:
    FadeState fadeState_ = FadeState::FadeStart;
    SpriteRender fadeSprite_;
    SpriteRender loadingSprite_;
    Sprite sprite_;

private:
    bool fadeInEnd_ = false;
    bool fadeTransitionFlag_ = false;
    bool loadingFlag_ = false;	
};
