#pragma once
#include "Src/Production/CutIn/CutInBase.h"
#include "Src/Production/CutIn/CutInLogoLayer.h"

/**
 * @file   CutInView.h
 * @brief  ボス戦カットイン演出のルート GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        class CutInMaskLayer;
        class CutInScrollLayer;
        class CutInSlideLayer;
        class CutInShadowLayer;
        class CutInLogoLayer;

        /**
         * @class CutInView
         * @brief  カットイン各レイヤーを束ね、寿命管理する。
         */
        class CutInView : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInView() = default;
            virtual ~CutInView();


        public:
            /**
             * @brief 初期化処理。各レイヤーを生成する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。各レイヤーを更新し、寿命を管理する。
             */
            void Update() override;

            /**
             * @brief 描画処理。各レイヤーを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc) override {}

            /**
             * @brief レイヤー DDS のパスを組み立てる。
             * @param layerName ファイル名（拡張子なし）。
             * @return Assets 配下のフルパス。
             */
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }


        public:
            /**
             * @brief 寿命を設定する。0 以下の値は無限寿命扱い。
             * @param time 寿命時間（秒）。
             */
            inline void SetLifeDuration(float time)
            {
                lifeDuration_ = time;
                timer_ = 0.0f;
            }

            /**
             * @brief 寿命が尽きたかを判定する。
             * @return 寿命が尽きていれば true。
             */
            inline bool IsTimeLimited() const
            {
                if (lifeDuration_ > 0.0f && timer_ >= lifeDuration_)
                    return true;
                return false;
            }

            /**
             * @brief カットイン演出が終了したかを判定する。
             * @return 終了していれば true。
             */
            inline bool IsCutInFinished() const
            {
                return IsTimeLimited();
            }


        private:
            CutInMaskLayer* pMaskLayer_ = nullptr; //!< カットインマスクレイヤー    
            CutInScrollLayer* pScrollLayer_ = nullptr; //!< カットインスクロールレイヤー
            CutInSlideLayer* pSlideLayer_ = nullptr;   //!< カットインスライドレイヤー
            CutInShadowLayer* pShadowLayer_ = nullptr; //! < カットインシャドウレイヤー
            CutInLogoLayer* pLogoLayer_ = nullptr;     //! < カットインロゴレイヤー

            float timer_ = 0.0f; //! < 寿命管理用タイマー
            float lifeDuration_ = -1.0f; //! < 寿命時間（秒）。0 以下は無限寿命扱い
        };
    } // namespace nsProduction
} // namespace nsApp

using CutInView = nsApp::nsProduction::CutInView;
