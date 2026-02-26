#include "stdafx.h"
#include "EndRollManager.h"

#include "Src/Production/Fade.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/EndRoll/EndRollImage.h"
#include "Src/Production/EndRoll/EndRollText.h"

namespace app
{
    namespace production
    {
        EndRollManager::~EndRollManager()
        {
            for (auto* component : components_)
            {
                if (component)
                    DeleteGO(component);
            }
            components_.clear();

        }


        bool EndRollManager::Start()
        {
            // 画像クラスを生成する。
            pImage_ = NewGO<EndRollImage>(0, "Image");
            components_.push_back(pImage_);

            // テキストクラスを生成する。
            pText_ = NewGO<EndRollText>(0, "Text");
            components_.push_back(pText_);

            // フェードクラスを生成する。
            auto* pGlobalFade = SceneManager::GetInstance()->GetFade();
            if (pGlobalFade)
                pGlobalFade->StartFadeIn();

            return true;
        }


        bool EndRollManager::IsEnd() const
        {
            // 演出が全て終わっているか判定。
            for (const auto* component : components_)
            {
                if (component && !component->IsEnd())
                    return false;
            }

            // 全て終わっているなら true
            return true;
        }
    }
}
