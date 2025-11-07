#pragma once
class Fade;
class Loading 
{
public:
    bool Start() ;
    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender loadingSprite_;

};
