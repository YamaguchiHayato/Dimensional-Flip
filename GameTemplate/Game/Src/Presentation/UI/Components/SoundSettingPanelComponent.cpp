#include "stdafx.h"

#include "SoundSettingPanelComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void SoundSettingPanelComponent::OnBuild()
        {
            /* 二重初期化防止。 */
            if (isBuilt_)
                return;

            /* 背景パネルを初期化する。 */
            baseSprite_.Init(pathBase_, baseW_, baseH_);
            baseSprite_.SetPivot({0.5f, 0.5f});
            baseSprite_.SetPosition(basePos_);
            baseSprite_.Update();

            isBuilt_ = true;
        }


        void SoundSettingPanelComponent::OnUpdate(float /*deltaTime*/)
        {
            /* 位置・見た目の更新。 */
            if (!isBuilt_)
                return;

            baseSprite_.SetPosition(basePos_);
            baseSprite_.Update();
        }


        void SoundSettingPanelComponent::OnDraw(RenderContext& rc, const Matrix& /*ownerWorld*/)
        {
            /* 背景を描画する。 */
            if (!isBuilt_)
                return;

            baseSprite_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
