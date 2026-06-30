#pragma once

namespace app
{
    namespace enemyStatus
    {
        /**
         * @enum BossAnimation
         * @brief ボスモデルのアニメーション ID。
         */
        enum BossAnimation : uint8_t
        {
            bossAnim_Idle = 0,   ///< 待機
            bossAnim_AttackCast, ///< 攻撃詠唱
            bossAnim_AttackRoar, ///< 咆哮
            bossAnim_Tumble,     ///< 転倒
            bossAnim_Run,        ///< 走行
            bossAnim_Hit,        ///< 被弾
            bossAnim_Jump,       ///< ジャンプ
            bossAnim_Land,       ///< 着地
            bossAnim_Dead,       ///< 死亡
            bossAnim_Num,        ///< 総数
        };

        /**
         * @enum AttackType
         * @brief ボスが選択する攻撃種別（2D/3D 共通）。
         */
        enum class AttackType : uint8_t
        {
            Meteor,   ///< 隕石（3D）
            Spear,    ///< 槍（3D）
            Roar3D,   ///< 咆哮（3D）
            FireBall, ///< 火の玉（2D）
            Jump,     ///< ジャンプ攻撃（2D）
            Roar2D,   ///< 咆哮（2D）
            Num       ///< 総数
        };

        /**
         * @enum Attack3DType
         * @brief 3D フェーズ専用の攻撃インデックス。
         */
        enum class Attack3DType : uint8_t
        {
            type_Meteor, ///< 隕石
            type_Spear,  ///< 槍
            type_3DRoar, ///< 咆哮
            type_Num     ///< 総数
        };

        /**
         * @enum Attack2DType
         * @brief 2D フェーズ専用の攻撃インデックス。
         */
        enum class Attack2DType : uint8_t
        {
            type_FireBall, ///< 火の玉
            type_Jump,     ///< ジャンプ攻撃
            type_2DRoar,   ///< 咆哮
            type_2DMeteor, ///< 隕石（2D）
            type_Num       ///< 総数
        };

        /**
         * @enum BossState
         * @brief ボス本体のステートマシン ID。
         */
        enum BossState : uint8_t
        {
            state_Idle = 0, ///< 待機
            state_Attack,   ///< 攻撃
            state_Hit,      ///< 被弾
            state_Tumble,   ///< 疲労・転倒
            state_Dead,     ///< 死亡
            state_Num       ///< 総数
        };

        /**
         * @enum JumpStep
         * @brief ジャンプ攻撃の内部ステップ。
         */
        enum class JumpStep : uint8_t
        {
            Prepare, ///< 予備動作
            Jumping, ///< 空中
            Landing, ///< 着地
            Finish,  ///< 終了
            Num      ///< 総数
        };

        /**
         * @enum EffectID
         * @brief ボス攻撃で使うエフェクト ID。
         */
        enum EffectID : uint8_t
        {
            effect_FireBall = 0, ///< 火の玉
            effect_Brest,        ///< ブレス
            effect_Spear,        ///< 槍
            effect_Meteor,       ///< 隕石
            effect_Roar,         ///< 咆哮
            effect_ShockWave,    ///< 衝撃波
            effect_thunder,      ///< 雷
            effect_Hit,          ///< ヒット
        };
    } // namespace enemyStatus
} // namespace app
