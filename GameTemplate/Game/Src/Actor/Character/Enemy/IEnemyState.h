#pragma once


namespace app
{
    namespace enemy
    {
        class IEnemyState
        {
        public:
            IEnemyState() = default;
            virtual ~IEnemyState() = default;


        // ステートのサイクル。
        public:
            // 開始時に1度だけ呼ばれる。
            virtual void Enter() = 0;
            // 毎フレーム呼ばれる。
            virtual void Update() = 0;
            // 終了時(別の状態へ映る瞬間)に1度だけ呼ばれる。
            virtual void Exit() = 0;
            // 次のステートへ遷移するかどうかを判断する。
            virtual bool RequestID(uint8_t& request) = 0;
        };

    }
}
