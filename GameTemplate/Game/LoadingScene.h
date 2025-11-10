#pragma once
class Fade;
class LoadingScene : public IGameObject
{
public:
    bool Start() ;
    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender loadingSprite_;

};
