#pragma once

namespace app
{
    namespace enemyStatus
    {
        // アニメーションの種類
        enum BossAnimation : uint8_t
        {
            bossAnim_Idle = 0,   // 待機
            bossAnim_AttackCast, // 攻撃
            bossAnim_AttackRoar, // 咆哮
            bossAnim_Tumble,     // 転倒
            bossAnim_Run,        // 走る
            bossAnim_Hit,        // ダメージ
            bossAnim_Jump,       // ジャンプ
            bossAnim_Land,       // 着地
            bossAnim_Dead,       // 死亡
            bossAnim_Num,        // 総数
        };


        // 攻撃タイプ
        enum class AttackType : uint8_t
        {
            // 3D技
            Meteor,   // 隕石
            Spear,    // 槍
            Roar3D,   // 咆哮(3D)

            // 2D技
            FireBall, // 火の玉
            Jump,     // ジャンプ
            Roar2D,   // 咆哮(2D)

            Num
        };


        // 攻撃タイプ: 3D
        enum class Attack3DType : uint8_t
        {
            type_Meteor, // 隕石
            type_Spear,  // 槍
            type_3DRoar,   // 咆哮
            type_Num
        };


        // 攻撃タイプ: 2D
        enum class Attack2DType : uint8_t
        {
            type_FireBall, // 火の玉
            type_Jump,     // ジャンプ攻撃
            type_2DRoar,     // 咆哮
            type_Num
        };


        // ステートの種類
        enum BossState : uint8_t
        {
            state_Idle = 0, // 待機
            state_Attack,   // 攻撃
            state_Hit,      // ダメージ
            state_Tumble,   // 疲労
            state_Dead,     // 死亡
            state_Num       // 総数
        };


        // ジャンプの挙動。
        enum class JumpStep : uint8_t
        {
            Prepare, 
            Jumping, 
            Landing, 
            Finish,
            Num
        };


        // エフェクトをID管理。
        enum EffectID : uint8_t
        {
            effect_FireBall = 0, // 火の玉
            effect_Brest,        // ブレス
            effect_Spear,        // 槍エフェクト
            effect_Meteor,       // 隕石エフェクト
            effect_Roar,         // 咆哮エフェクト
            effect_ShockWave,    // 衝撃波エフェクト
            effect_thunder,      // 雷エフェクト
            effect_Hit,          // ヒットエフェクト
        };


        // ボス戦の進行フェーズを管理。
        enum class BossPhase : uint8_t
        {
            phase_One = 0, // フェーズ1
            phase_Two,     // フェーズ2
            phase_Three,   // フェーズ3
            phase_Num
        };

    }
}
