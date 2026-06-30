#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace nsApp
{
    namespace nsProduction
    {
        class CutInLogoLayer : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInLogoLayer() = default;
            virtual ~CutInLogoLayer() = default;


        public:
            /**
             * @brief 初期化処理。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。
             */
            void Update() override;

            /**
             * @brief 描画処理。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief レイヤー DDS のパスを組み立てる。
             * @param layerName ファイル名（拡張子なし）。
             * @return Assets 配下のフルパス。
             */
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }


        private:
            /**
             * @brief ロゴの移動アニメーションを更新する。
             */
            void Move();

            /**
             * @brief ロゴの表示準備を行う。
             */
            void Preparation();

            /**
             * @brief ロゴの表示を開始する。
             */
            inline void CanDrawEmblem()
            {
                isVisible = true;
                isAnimating_ = true;
                moveTimer_ = 0.0f;
            }

            /**
             * @brief ロゴの表示が終了したかを判定する。
             * @return 表示が終了していれば true。
             */
            inline bool IsFinished() const { return isVisible && !isAnimating_; }


        private:
            float delayTimer_ = 0.0f; //! < ロゴ表示までの遅延タイマー。
            float delayTime_ = 0.5f;  //! < ロゴ表示までの遅延時間。
            bool isVisible = false;   //! < ロゴが表示されているかどうか。

            float moveTimer_ = 0.0f; //! < ロゴの移動アニメーションタイマー。
            float duration_ = 0.6f;  //! < ロゴの移動アニメーションの継続時間。
            bool isAnimating_ = false; //! < ロゴの移動アニメーション中かどうか。

            Quaternion currentRot_ = Quaternion::Identity; //! < ロゴの現在の回転。
        };
    } // namespace nsProduction
} // namespace nsApp
