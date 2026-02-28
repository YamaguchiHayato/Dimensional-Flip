#pragma once

class Fade;
class LoadingScene : public IGameObject
{
public:
    LoadingScene() = default;
    ~LoadingScene() = default;


public:
    bool Start();
    void Update();


public:
    // ローディング画面の描画。
    void Render(RenderContext& rc);


private:
    // 「Now Loading」の文字を初期化。
    void CreateLoadingFont();


private:
    // 「Now Loading」と表記。
    FontRender loadingFont_;

    // 点滅計算用タイマー。
    float timer_ = 0.0f;
};
