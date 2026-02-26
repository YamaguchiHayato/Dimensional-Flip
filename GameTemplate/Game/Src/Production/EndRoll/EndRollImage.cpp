#include "stdafx.h"
#include "EndRollImage.h"

namespace
{
    const float CHANGE_INTERVAL = 3.0f;        // 3秒ごとに切り替え
    const Vector3 POS = {-400.0f, 0.0f, 0.0f}; // 画面左側に配置
    const float IMG_WIDTH = 800.0f;            // 画像の幅
    const float IMG_HEIGHT = 450.0f;           // 画像の高さ


    const char* paths[] =
    {
        {"Assets/sprite/staffRoll/staffRoll_01.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_02.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_03.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_04.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_05.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_06.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_07.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_08.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_09.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_10.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_11.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_12.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_13.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_14.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_15.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_16.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_17.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_18.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_19.DDS"},
        {"Assets/sprite/staffRoll/staffRoll_20.DDS"},
    };
}

namespace app
{
    namespace production
    {
        bool EndRollImage::Start()
        {
            // スライドショーとして使う画像を初期化する。
            InitSlideShow();

            return true;
        }


        void EndRollImage::InitSlideShow()
        {
            // スライドショー用画像のパス。
            for (auto path : paths)
            {
                auto image = std::make_unique<SpriteRender>();

                // 初期化。
                image->Init(path, IMG_WIDTH, IMG_HEIGHT);
                // 座標をセット。
                image->SetPosition(POS);
                // 中心地をセット。
                image->SetPivot({0.5f, 0.5f});

                // リストに追加する。
                images_.push_back(std::move(image));
            }

            // 変数を初期化する。
            currentIndex_ = 0;
            timer_ = 0.0f;

        }


        void EndRollImage::Update()
        {
            if (images_.empty())
                return;

            // タイマーを更新する。
            timer_ += g_gameTime->GetFrameDeltaTime();

            // 一定時間経過すると、次の画像に切り替える。
            if (timer_ >= CHANGE_INTERVAL)
            {
                timer_ = 0.0f;
                currentIndex_++;

                // 20枚目の画像に行くと最初に戻る。
                if (currentIndex_ >= images_.size())
                {
                    currentIndex_ = 0;
                }
            }

            if (images_[currentIndex_])
            {
                images_[currentIndex_]->Update();
            }
        }


        void EndRollImage::Render(RenderContext& rc)
        {
            // 画像がない場合、何もしない。
            if (images_.empty())
                return;

            // 現在選ばれている画像のみを描画する。
            if (images_[currentIndex_])
            {
                images_[currentIndex_]->Draw(rc);
            }
        }
    }
}
