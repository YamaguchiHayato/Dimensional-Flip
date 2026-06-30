#pragma once
#include "Src/Production/CutIn/CutInBase.h"

namespace nsApp
{
    namespace nsProduction
    {
        class CutInSlideLayer : public CutInBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            CutInSlideLayer() = default;
            virtual ~CutInSlideLayer() = default;


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
             * @param rc レンダングコンテキスト。
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
             * @brief スライドアニメーションを更新する。
             */
            void Slide();

            /**
             * @brief 呼吸アニメーションを更新する。
             */
            void Breathing();

            /**
             * @brief スライド開始位置と終了位置を初期化する。
             */
            inline void InitStartPos()
            {
                startPos_ = Vector3(500.0f, 0.0f, 0.0f);
                endPos_ = Vector3(-300.0f, 0.0f, 0.0f);
                currentPos_ = startPos_;
            }

            /**
             * @brief スライドアニメーションのパラメータを初期化する。
             */
            inline void InitSlideParam()
            {
                duration_ = 0.5f;
                timer_ = 0.0f;
                isMoving_ = true;
            }

            /**
             * @brief 待機タイマーを初期化する。
             */
            inline void InitWaitTimer()
            {
                waitTimer_ = 0.0f;
                waitTime_ = 0.8f;
            }


        public:
            /**
             * @brief 現在の位置を取得する。
             * @return 現在の位置。
             */
            inline const Vector3& GetCurrentPos() const { return currentPos_; }

            /**
             * @brief スライド中かどうかを取得する。
             * @return スライド中なら true。
             */
            inline bool IsMoving() const { return isMoving_; }


        private:
            Vector3 startPos_ = Vector3::Zero; //! < スライド開始位置。
            Vector3 endPos_ = Vector3::Zero;   //! < スライド終了位置。
            Vector3 currentPos_ = Vector3::Zero; //! < 現在の位置。
            float timer_ = 0.0f; //! < スライドアニメーションの経過時間。
            float duration_ = 1.0f; //! < スライドアニメーションの継続時間。
            float breathTimer_ = 0.0f; //! < 呼吸アニメーションの経過時間。
            bool isMoving_;
            float waitTimer_ = 0.0f; //! < 待機タイマー。
            float waitTime_ = 0.6f;  //! < 待機時間。
        };
    } // namespace nsProduction
} // namespace nsApp
