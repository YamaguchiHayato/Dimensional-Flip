#pragma once
#include "Src/UI/BossUI/BossUIBase.h"

namespace app
{
    namespace nsUI
    {
        // ボス戦のフェーズの種類。
        enum class BossPhaseKind : uint8_t
        {
            Phase1 = 0, // フェーズ1 : 2D視点。
            Phase2,     // フェーズ2 : 3D視点。
            Phase3,     // フェーズ3 : ミックス視点。
            GO,
            None,
            Num
        };


        // ボス戦開始までの演出の進行状態。
        enum class PhaseAnimState : uint8_t
        {
            None = 0, // 非表示。
            Phase,    // フェーズ表示中。
            Moving,   // 移動中。
            GO,       // GO表示中。
            Finish,   // 演出終了。
            Num       // 状態数。
        };


        class BossPhaseUI : public BossUIBase
        {
        public:
            BossPhaseUI() = default;
            virtual ~BossPhaseUI() = default;


        public:
            void Initialize();
            void Update()override;
            void Draw(RenderContext& rc) override;


        public:
            // 現在のフェーズをセットする。
            inline void SetPhase(BossPhaseKind phase)
            {
                currentPhase_ = phase;
            }

            // ボス戦開戦時に表示する画像の描画時間。
            void PlayGOSprite();

            // GOTImerを更新。
            void UpdateGOTimer(Vector3& basePos);

            // 演出を開始する。
            void StartPhaseAninm(BossPhaseKind phase);

            // 演出アニメーションを更新する。
            void UpdatePhaseAnim();


        // 個々の演出ステートを更新する処理。
        private:
            // フェーズ表示中の更新。
            void UpdatePhaseState(float deltaTime);

            // 移動中の更新。
            void UpdateMovingState(float deltaTime, const Vector3& basePos);

            // GO表示中の更新。
            void UpdateGOState();

            // 演出終了中の更新。
            void UpdateFinishState(const Vector3& basePos);

            // ステート後の更新処理。
            void UpdateAfterState(const Vector3& basePos);

            // フェーズ1用(全て動かす。)
            void UpdateLayOut();

            // フェーズ数のUIのみを更新。」
            void UpdateLayOutLater();

        // ゲッター。
        public:
            inline bool IsPhasePlaying() const
            {
                return phaseState_ != PhaseAnimState::None && phaseState_ != PhaseAnimState::Finish;
            }


        // セッター。
        public:
            inline bool ShouldStopActors() const
            {
                return phaseState_ == PhaseAnimState::Phase;
            }


        private:
            SpriteRender phaseIcons_[(int) BossPhaseKind::Num]; // フェーズ数UI。
            SpriteRender phaseIconRender_;                      // フェーズアイコンUI。
  
           
            BossPhaseKind currentPhase_ = BossPhaseKind::Phase1; // 現在のフェーズ。


        // ボス戦開始までの演出用変数群。
        private:
            float goTimer_ = 0.0f; // GO画像の描画タイマー。
            bool isGOPlaying_ = false; // GO画像の描画中かどうか。

            Vector3 basePosition_ = Vector3::Zero; // 基準位置。

        // アニメーション用。
        private:
            // フェーズ演出の状態管理。
            PhaseAnimState phaseState_ = PhaseAnimState::None;
            float animTimer_ = 0.0f;
            Vector3 currentPosition_ = Vector3::Zero;
            Vector3 currentScale_ = Vector3::Zero;
            Vector3 currentNumberOffset_ = Vector3::Zero;
        };

    }
}

