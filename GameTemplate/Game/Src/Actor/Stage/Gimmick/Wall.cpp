#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"

namespace app
{
    namespace stage
    {
        bool Wall::Start()
        {
            render_.Init("Assets/stage/Stage1/DimensionWall.tkm");

            return true;
        }


        void Wall::Update()
        {
            render_,SetPos(pos_);
            render_.Update();
        }


        void Wall::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }

    }
}
