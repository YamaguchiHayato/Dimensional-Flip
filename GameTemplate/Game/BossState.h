#pragma once

class Boss;


namespace app
{
    namespace state
    {

        // ボスのステート基底クラス。
        class BossState
        {
        public:
            // 暗黙的な型変換を禁止する。
            explicit BossState(Boss* pBoss);
            // デストラクタ。
            virtual ~BossState() = default;

            // ステート開始時に1度呼ばれる。
            virtual void Enter(Boss* actor) {};

            // 毎フレーム呼ばれる処理。
            virtual void Update(Boss* actor) = 0;

            // ステート終了後に呼ばれる。
            virtual void Exit(Boss* actor) {};


        protected:
            Boss* pBoss_ = nullptr;
        };


        // 待機ステート。
        class BossIdleState : public BossState
        {
        public:
            BossIdleState(Boss* pBoss);
            void Enter(Boss* actor) override;
            void Update(Boss* actor) override;
            void Exit(Boss* actor) override;

        private:
            float timer_ = 0.0f;
            const float waitTime_ = 10.0f;
        };


        // 攻撃ステート。
        class BossAttackState : public BossState
        {
        public:
            BossAttackState(Boss* pBoss);
            void Enter(Boss* actor) override;
            void Update(Boss* actor) override;
            void Exit(Boss* actor) override;
        };


        // ダメージヒットステート。
        class BossDamageHitState : public BossState
        {
        public:
            BossDamageHitState(Boss* pBoss);
            void Enter(Boss* actor) override;
            void Update(Boss* actor) override;
            void Exit(Boss* actor) override;
        };
    }
}

