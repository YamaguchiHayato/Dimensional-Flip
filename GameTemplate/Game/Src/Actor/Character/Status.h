#pragma once

namespace app
{
    namespace status
    {
        class Status
        {
        public:
            Status() = default;
            virtual ~Status() = default;

            // ステータスの初期化関数。
            // ここでは体力・移動速度・攻撃力を設定する。
            void Initial(uint8_t maxHP, const float speed, const float power);


             // ダメージを受ける処理。
            inline void Damage(uint8_t damage)
            {
                // HPを減らす。
                currentHP_ -= damage;
                if (currentHP_ > maxHP_)
                    currentHP_ = maxHP_;
            }


            // 死亡したかチェックする。
            inline bool IsDead() const
            {
                return currentHP_ <= 0;
            }


        // ゲッター。
        public:
            // 現在HPを取得。
            inline uint8_t GetHP()const 
            {
                return currentHP_;
            }


            // 最大HPを取得。
            inline uint8_t GetMaxHP()const 
            {
                return maxHP_;
            }


            // 移動速度を取得。
            inline const float GetSpeed() const
            {
                return speed_;
            }


            // 攻撃力を取得。
            inline const float GetPoser() const
            {
                return power_;
            }

        private:
            // HP。
            uint8_t currentHP_ = 0;   // HPの現在地。
            uint8_t maxHP_ = 0;       // HPの最大値。

            float speed_ = 0.0f;      // 移動速度。
            float power_ = 0.0f;      // 攻撃力。
        };

    }
}

