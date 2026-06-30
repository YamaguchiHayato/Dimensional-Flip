#pragma once
#include "Src/Scene/title/TitleLayerBase.h"

enum class FlipState : uint8_t
{
    Idle,      // 待機中。      (ゆっくりと回転している状態)
    PreFlip,   // 呼び回転動作。(高速回転に入る前の状態)
    Flipping,  // 反転中。      (高速回転中。)
    PostFlip,  // 反転完了。    (回転直後の余韻。)
};

namespace nsApp
{
    namespace nsTitle
    {
        class TitleBackgroundLayer : public TitleLayerBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleBackgroundLayer() = default;
            virtual ~TitleBackgroundLayer() = default;


        public:
            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            void Update() override;

            /**
             * @brief 毎フレームのシーン描画。
             * @param rc 
             */
            void Render(RenderContext& rc) override;


        public:
            /**
             * @brief マニュアルモードかどうかを設定する。
             * @param isManual : マニュアルモードかどうかのフラグ。
             */
            inline void SetManualMode(bool isManual) { isManualMode_ = isManual; }


        public:
            /**
             * @brief 初期化処理。
             * @param layerNane : レイヤー名。
             * @return 初期化に成功した場合 true を返す。
             */
            inline const std::string Init(const std::string layerNane) override
            {
                return TitleLayerBase::Init(layerNane);
            }

            // イージング処理。
            /**
             * @brief イージング関数（InBack）。
             * @param t : 0.0f ~ 1.0f の範囲で補間値を指定する。
             * @return 補間後の値を返す。
             */ 
            inline float EaseInBack(float t) { return t * t; }

            /**
             * @brief イージング関数（InOutCubic）。
             * @param t : 0.0f ~ 1.0f の範囲で補間値を指定する。
             * @return 補間後の値を返す。
             */
            inline float EaseInOutCubic(float t)
            {
                return t < 0.5f ? 4.0f * t * t * t : 1.0 - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
            }


        private:
            /**
             * @brief 回転パラメータの初期化。
             */
            void InitRotParam();

            /**
             * @brief 回転処理。
             */
            void Rotation();


        private:
            SpriteRender blackSpriteRender_;        //! 背景黒画像レンダラー。
            Quaternion rot_ = Quaternion::Identity; //! 回転クォータニオン。
            FlipState state_ = FlipState::Idle;     //! 回転状態。
            float currentAngle_ = 0.0f; //! 回転開始角度。
            float stateTimer_ = 0.0f;   //! 状態遷移用タイマー。
            float startAngle_ = 0.0f;   //! 補完用 : 開始角度。
            float targetAngle_ = 0.0f;  //! 補完用 : 目標角度。
            bool isManualMode_ = false; //! マニュアルモードかどうかのフラグ。
        };

    }
}

