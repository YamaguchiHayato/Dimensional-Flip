#pragma once
#include "Src/Actor/Stage/IStage.h"

class Fade : public IGameObject
{
public:
    Fade();
    virtual ~Fade() = default;


public:
    void Update();
    void Render(RenderContext& rc);


public:
    // FadeOutを開始する
    void StartFadeOut();
    // FadeInを開始する
    void StartFadeIn();

  
// ゲッター。
public:
    // 
    inline FadeState GetFadeState() const
    {
        return fadeState_;
    }

    // 
    inline SpriteRender& GetFadeSprite()
    {
        return fadeSprite_;
    }

    // 
    inline bool IsFadeInEnd() const
    {
        return fadeInEnd_;
    }

    // 
    inline bool IsFadeOutEnd() const
    {
        return fadeOutEnd_;
    }


private:
    void ChangeFadeState();

private:
    FadeState fadeState_ = FadeState::Fade_In;
    SpriteRender fadeSprite_;
    Sprite sprite_;

private:
    uint8_t fadeCounter_ = 0;

    bool fadeInEnd_ = false;
    bool fadeOutEnd_ = false;
    bool fadeTransitionFlag_ = false;
    bool loadingFlag_ = false;
    bool transitionInProgress_ = false;
};
