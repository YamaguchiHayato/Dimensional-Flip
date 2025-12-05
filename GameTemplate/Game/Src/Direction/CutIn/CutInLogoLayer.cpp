#include "stdafx.h"
#include "CutInLogoLayer.h"

namespace layerParam
{
    const auto WIDHT = 1280.0f;                     // カットインロゴの横幅。
    const auto HEIGHT = 648.0f;                    // カットインロゴの縦幅。
                                                   
    const auto START_ROT = 720.0f;                // 開始回転角度。
    const auto END_ROT = 0.0f;                     // 終了回転角度。
    Vector3 CURRENT_ROT(300.0f, 0.0f, 0.0f);       // 現在の回転角度。
    Vector3 POSITION(600.0f, 0.0f, 0.0f);        // ロゴの位置。

    const float STARTSCALE = 6.0f;
    const float ENDSCALE = 2.0f;
};

namespace app
{
    namespace cutIn
    {
        bool CutInLogoLayer::Start()
        {
            const std::string layerPath = FindLayerPath("Emblem");
            layer_.Init(layerPath.c_str(), layerParam::WIDHT, layerParam::WIDHT);

            layer_.SetPosition(Vector3(200.0f, 100.0f, 0.0f));

            // ステータスの初期化。
            Preparation();
            return true;
        }


        void CutInLogoLayer::Update()
        {
            // 経過時間で描画の許可を判断。 
            Move();

            // 回転設定。
            layer_.SetRotation(currentRot_);

        //    layer_.SetScale(Vector3(2.0f, 2.0f, 2.0f));
            layer_.SetPosition(layerParam::POSITION);
            layer_.Update();
        }


        void CutInLogoLayer::Move()
        {
            // 表示されていない場合。
            if (!isVisible)
            {
                float deltaTime = 1.0f / 60.0f;
                delayTimer_ += deltaTime;

                if (delayTimer_ >= delayTime_)
                    CanDrawEmblem();
            }

            if (isVisible && isAnimating_)
            {
                float deltaTime = 1.0f / 60.0f;
                moveTimer_ += deltaTime;

                float rate = moveTimer_ / duration_;
                if (rate >= 1.0f)
                {
                    rate = 1.0f;
                    isAnimating_ = false;
                }

                // イージング (EaseOutCubic)
                float t = 1.0f - rate;
                float easeRate = 1.0f - (t * t * t);

                // 角度計算 (Degree)
                float zAngleDeg = layerParam::START_ROT + (layerParam::END_ROT - layerParam::START_ROT) * easeRate;
                // ラジアン変換してZ軸回転を作成
                float zAngleRad = zAngleDeg * (3.14159265f / 180.0f);
                currentRot_.SetRotationZ(zAngleRad);

                // スケール計算
                float currentScale = layerParam::STARTSCALE + (layerParam::ENDSCALE - layerParam::STARTSCALE) * easeRate;
                layer_.SetScale(Vector3(currentScale, currentScale, currentScale));

            }

            // 終了後。
            else if (isVisible && !isAnimating_)
            {
                // 終了後の大きさ。
                layer_.SetScale(Vector3::One);
            }
        }


        void CutInLogoLayer::Preparation()
        {
            // 変数を初期化する。
            delayTimer_ = 0.0f;
            delayTime_ = 0.0f;  // n秒後に登場する。

            moveTimer_ = 0.0f;
            delayTimer_ = 0.6f; // n秒かけて回転して止まる。

            isVisible = false;
            isAnimating_ = false;
        }


        void CutInLogoLayer::Render(RenderContext& rc)
        {
            if (isVisible)
                layer_.Draw(rc);
        }
    }
}
