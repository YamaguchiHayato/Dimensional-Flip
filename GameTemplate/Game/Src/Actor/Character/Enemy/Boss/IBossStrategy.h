#pragma once


namespace app{
    namespace enemy{
        class Boss;
    }
} 

namespace app
{
    namespace enemyState
    {
        class IBossStrategy
        {
        public:
            explicit IBossStrategy() = default;
            virtual ~IBossStrategy() = default;


        public:
            // 攻撃開始時の初期化。
            virtual void Enter(app::enemy::Boss* pBoss) = 0 {};

            // 攻撃中の更新処理。
            virtual void Update() = 0 {};

            // 終了判定。
            virtual void Exit() = 0 {};

            // 攻撃が終わったかを判定する。
            virtual bool IsFinished() const = 0; 
        };

    }
}

