#pragma once
#include "Src/Production/CutIn/CutInBase.h"
#include "Src/Production/CutIn/CutInLogoLayer.h"

namespace app
{
    namespace cutIn
    {
        class CutInMaskLayer;
        class CutInScrollLayer;
        class CutInSlideLayer;
        class CutInShadowLayer;
        class CutInLogoLayer;


        class CutInView : public CutInBase
        {
        public:
            CutInView() = default;
            virtual ~CutInView();

            // 初期化処理。
            bool Start() override;

            // 更新処理。
            void Update() override;

            // 描画処理。
            void Render(RenderContext& rc) override {};

            // レイヤーのパスを取得する。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }

        // セッター。
        public:
            // カットインの寿命を設定する。
            inline void SetLifeDuration(float time)
            {
                lifeDuration_ = time;
                timer_ = 0.0f;
            }


            // 稼働時間外かどうかをチェックする。
            inline bool IsTimeLimited() const
            {
                if (lifeDuration_ > 0.0f && timer_ >= lifeDuration_)
                    return true;
                return false;
            }


        // ゲッター。
        public:
            // カットインの稼働終了をチェックする。
            inline bool IsCutInFinished() const
            {
                return IsTimeLimited();
            }

        private:
            CutInMaskLayer* pMaskLayer_ = nullptr;
            CutInScrollLayer* pScrollLayer_ = nullptr;
            CutInSlideLayer* pSlideLayer_ = nullptr;
            CutInShadowLayer* pShadowLayer_ = nullptr;
            CutInLogoLayer* pLogoLayer_ = nullptr;

        private:
            float timer_ = 0.0f;
            float lifeDuration_ = -1.0f;
        };

    }
}

