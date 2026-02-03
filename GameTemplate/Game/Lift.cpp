#include "stdafx.h"
#include "Lift.h"


namespace app
{
    namespace gimmick
    {
        bool Lift::Start()
        {
            const std::string path = InitGimmick("Assets/stage/Stage1/Lift.tkm");
            render_.Init(path.c_str());

            render_.SetScale(scale_);
            render_.SetPosition(gimmickPos_);
            return true;
        }


        void Lift::Update()
        {
            render_.SetScale(scale_);
            render_.SetPosition(gimmickPos_);
            render_.Update();
        }


        void Lift::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
