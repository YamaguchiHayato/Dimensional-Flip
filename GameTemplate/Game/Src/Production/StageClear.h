#pragma once
#include "stdint.h"
#include "Src/Core/Game.h"
#include "Src/Actor/Stage/IStage.h"


class StageClear : public IGameObject
{
public:
    StageClear() {}
    ~StageClear() {}


public:
    bool Start() ;
    void Update();
    void Render(RenderContext& rc);


public:
    // 座標をセット。
    inline void SetPosition(const Vector3& pos)
    {
        position_ = pos;
    }



private:
    StageID DecideStage();


private:
    SpriteRender stageClearSprite_;

    Vector3 position_ = Vector3::Zero;

    float timer_ = 0.0f;
};
