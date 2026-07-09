#include "stdafx.h"

#include "BossBriefingSlideComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void BossBriefingSlideComponent::OnBuild()
        {
            if (isBuilt_)
                return;

            slideSprite_.SetPivot({0.5f, 0.5f});
            isBuilt_ = true;
        }


        void BossBriefingSlideComponent::ApplySlidePath(const char* path)
        {
            if (!isBuilt_)
                OnBuild();

            /* パスが無い／同じなら再読込しない。 */
            if (path == nullptr || path[0] == '\0')
            {
                currentPath_.clear();
                return;
            }

            if (currentPath_ == path)
                return;

            currentPath_ = path;
            slideSprite_.Init(currentPath_.c_str(), slideW_, slideH_);
            slideSprite_.SetPivot({0.5f, 0.5f});
        }

        void BossBriefingSlideComponent::OnUpdate(float /*deltaTime*/)
        {
            if (!isBuilt_ || currentPath_.empty())
                return;

            slideSprite_.SetPosition(slidePos_);
            slideSprite_.Update();
        }

        void BossBriefingSlideComponent::OnDraw(RenderContext& rc, const Matrix& /*ownerWorld*/)
        {
            if (!isBuilt_ || currentPath_.empty())
                return;

            OnUpdate(0.0f);
            slideSprite_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
