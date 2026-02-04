#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossTumbleState : public IEnemyState
        {
        public:
            BossTumbleState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {};
            virtual ~BossTumbleState() = default;


        public:
            void Enter() override;
            void Update() override;
            void Exit() override;
            bool RequestID(uint8_t& request) override;


        public:
            // 弱点ポイントにヒットしたかどうかをチェックする。
            bool CheckWeakPointHit();


        private:
            // ボスの頭が下がるアニメーションイベント。
            void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

        private:
            app::enemy::Boss* pBoss_ = nullptr;


        private:
            float timer_ = 0.0f; // 転倒時間を計測。
            float delayTimer_ = 0.0f;// 倒れこむまでの無敵時間タイマー。

            bool canBeAttacked_ = true; // ダメージを受け付けるかどうか。

            bool isWeakCollision_;

            bool isAbove_;

            Vector3 headPos_ = Vector3::Zero;
            Vector3 playerPos_ = Vector3::Zero;
            Vector3 diff_ = Vector3::Zero;


            float currentWeakHeight_ = 22.0f;
            float targetHeight_ = 22.0f;
        };

    }
}
