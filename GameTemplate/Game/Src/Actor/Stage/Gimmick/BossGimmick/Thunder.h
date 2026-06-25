#pragma once
#include "Src/Actor/Character/PlayerForward.h"


namespace app
{
    namespace gimmick
    {
        enum class ThunderMode : uint8_t
        {
            Stationary, // 上から下に。
            Moving      // 奥から手前に移動。
        };


        class Thunder : public IGameObject
        {
        private:
            enum class ThunderStep : uint8_t
            {
                Warning, // 警告UIを表示。
                Falling, // 雷エフェクトを落とす。
                Finish   // 終了。
            };


        public:
            Thunder() = default;
            virtual ~Thunder();


        public:
            bool Start();
            void Update();
            void Render(RenderContext& rc);

        public:
            // 初期パラメータを設定。
            inline void SetParam(const Vector3& pos, ThunderMode mode, float scale, float duration)
            {
                pos_ = pos;
                currentMode_ = mode;
                baseScale_ = scale;
                scale_ = Vector3::One * scale;
                warningDuration_ = duration;
            }


        public:
            const Vector3& GettTargetWorldPos() const
            {
                return targetPos_;
            }

        private:
            // ステート全体を更新。
            void UpdateAttackState();

            // 警告UIの表示。
            void UpdateWarningUI(float deltaTime);

           // 雷エフェクト落下。
           void UpdateStrike(float deltaTime);

            // 当たり判定の可視化。
            void CheckCollision();

            // 雷エフェクトの生成。
            void CreateThunderEffect();

            // 警告UIを描画する。
            void CreateWarningUI();


        private:
            EffectEmitter* pThunderEffect_ = nullptr;
            Player* pPlayer_ = nullptr;

        private:
            ThunderMode currentMode_ = ThunderMode::Stationary;
            ThunderStep currentStep_ = ThunderStep::Warning;

            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_ = Vector3::One;
            Vector3 targetPos_ = Vector3::Zero;
            Quaternion rot_ = Quaternion::Identity;

            SpriteRender warnigUIRender_;
         
            float timer_ = 0.0f;
            float baseScale_ = 1.0f;
            float warningDuration_ = 1.5f;

            bool hasDamaged_ = false; // ダメージを2重に与えないように。
            bool isWarningVisible_ = false; // 描画フラグ。
        };

    }
}

