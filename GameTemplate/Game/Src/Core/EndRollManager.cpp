#include "stdafx.h"
#include "EndRollManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"


#include "Src/Production/Fade.h"

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
            pFade_ = SceneManager::GetInstance()->GetFade();
            pFade_->StartFadeIn();

            // エンディング曲を再生する。
            app::core::SoundManager::GetInstance()->PlayBGM(GameSoudList_BGM_EndRoll);

            // フラグの初期化。
            isFadeOutStarted_ = false;

            // スキップ用のフォントを初期化。
            InitSkipFont(); 

            return true;
        }


        void EndRollManager::Update()
        {
            if (!isFadeOutStarted_)
                TrySkip();


            if (pText_ && pText_->IsEnd())
            {
                if (!isFadeOutStarted_)
                {
                    if (pFade_)
                        pFade_->StartFadeOut();

                    isFadeOutStarted_ = true;
                }
            }
        }


        void EndRollManager::Render(RenderContext& rc)
        {
            if (!isFadeOutStarted_)
                skipFont_.Draw(rc);
        }


        void EndRollManager::InitSkipFont()
        {
            skipFont_.SetText(L"A:Skip");
            skipFont_.SetPosition(Vector3(600.0f, -335.0f, 0.0f));
            skipFont_.SetScale(2.0f);
            skipFont_.SetColor(Vector4::White);
            skipFont_.SetPivot({1.0f, 0.5f});
            skipFont_.SetShadowParam(true, 2.0f, Vector4::Black);
        }


        void EndRollManager::TrySkip()
        {
            if (!g_pad[0]->IsTrigger(enButtonA))
                return;
            if (pText_)
                pText_->Skip();
        }


        bool EndRollManager::IsEnd() const
        {
            // 演出が全て終わっているか判定。
            if (isFadeOutStarted_ && pFade_ && pFade_->IsFadeOutEnd())
                return true;

            // 全て終わっているなら true
            return false;
        }
    }
}
