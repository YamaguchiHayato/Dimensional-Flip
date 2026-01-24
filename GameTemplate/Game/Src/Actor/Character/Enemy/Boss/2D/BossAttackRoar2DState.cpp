#include "stdafx.h"
#include "BossAttackRoar2DState.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"



namespace app
{
    namespace enemyState
    {
        void BossAttackRoar2DState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;

            // 咆哮アニメーションを設定する。
            pBoss_->LoadAnimation(app::enemyStatus::bossAnim_AttackRoar, false, 0.1f);

            // 攻撃タイプの設定。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Roar2D);

            // 移動速度を初期化する。
            pBoss_->SetMoveSpeed(Vector3::Zero);

            // Playerの方を向かせるように補正。
            LookAtPlayerDirection();
        }


        void BossAttackRoar2DState::Update()
        {
            // 咆哮中もPlayerの方向を向いてほしいので、補正。
            LookAtPlayerDirection();
        }


        void BossAttackRoar2DState::Exit()
        {
            // 咆哮後のインターバルを設定。
            pBoss_->SettNextInterval(3.0f);
        }


        bool BossAttackRoar2DState::IsFinished() const
        {
            return !pBoss_->IsPlayingAnimation();
        }


        void BossAttackRoar2DState::LookAtPlayerDirection()
        {
            // 初期のボスモデルの向きを設定する。(2DなのでX軸をあわせる。)
            auto* pPlayer = pBoss_->GetPlayer();

            if (!pPlayer)
                return;

            // X座標の差分を計算。
            auto diffX = pPlayer->GetPlayerPos().x - pBoss_->GetPos().x;

            Quaternion targetRot;

            if (diffX > 0.0f)
                targetRot.SetRotationDegY(-90.0f);

            else
                targetRot.SetRotationDegY(90.0f);

            // 回転を合成して適応。
            pBoss_->SetRot(targetRot);
        }
    }
}
