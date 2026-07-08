#include "stdafx.h"

#include "BossAttackSpearState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Spear.h"

#include "Src/Core/SoundManager.h"
#include "Src/Presentation/UI/BossAttackHudHelper.h"

namespace
{
    const float SPAWN_TIME = 0.5f;        //! 攻撃の生成タイミング。
    const float STAGE_WIDTH_HALF = 35.0f; //! 横幅の半分。
    const float WALL_HEIGHT = 25.0f;      //! 壁の高さ。
    const float GRID_SIZE = 5.0f;         //! グリッドの間隔。
    const float SPAWN_Z_DIST = 80.0f;     //! 攻撃の生成Z座標。
    const float SAFE_GAP_Z = 7.0f;        //! 攻撃の停止Z座標。
} 

namespace app
{
    namespace enemyState
    {
        void BossAttackSpearState::Enter(app::enemy::Boss* pBoss)
        {
            /* 初期化。*/
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            /* 攻撃アニメーションの再生。*/
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, true, 0.1f);

            /* 攻撃タイプの設定。*/
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Spear);

            /* 攻撃タイプの通知。*/
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Spear);
        }


        void BossAttackSpearState::Update()
        {
            /* タイマーを取得。*/
            timer_ += g_gameTime->GetFrameDeltaTime();

            /* 攻撃の生成タイミングになったら攻撃を生成。*/
            if (timer_ >= SPAWN_TIME && !isAttackSpawned_)
            {
                /* 攻撃の生成フラグを立てる。*/
                isAttackSpawned_ = true;
                CreateSpearAttack();
            }
        }


        void BossAttackSpearState::Exit()
        {
            /* 攻撃の生成フラグをリセット。*/
            pBoss_->SettNextInterval(3.0f);

            /* 攻撃UIの設定。 */
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Spear);
        }


        void BossAttackSpearState::CreateSpearAttack()
        {
            /* 攻撃SEの再生。*/
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Spear, 2.0);

            /* 攻撃の生成位置を決定。*/
            centerX_ = 0.0f;
            if (auto* pPlayer = pBoss_->GetPlayer())
                centerX_ = pPlayer->GetPlayerPos().x;

            /* 攻撃の生成位置をランダムにずらす。*/
            randomOffset_ = (static_cast<float>(rand() % 200) / 10.0f) - 10.0f;
            centerX_ += randomOffset_;

            /* 攻撃の生成範囲を決定。*/
            startX_ = centerX_ - 25.0f;
            endX_ = centerX_ + 25.0f;
            pattern_ = rand() % 3;

            /* 攻撃の生成。*/
            for (float x = startX_; x <= endX_; x += GRID_SIZE)
            {
                // --- 生成範囲の制限: ステージの端を超えないようにする。 ---
                if (x < -STAGE_WIDTH_HALF || x > STAGE_WIDTH_HALF)
                    continue;

                // --- 生成範囲の制限: プレイヤーの位置を避ける。 ---
                for (float y = 2.0f; y <= WALL_HEIGHT; y += GRID_SIZE)
                {
                    // --- パターン判定: 奥の壁を出すか？ ---
                    if (pattern_ == 0 || pattern_ == 2)
                    {
                        // 奥の壁 (Z+ -> 手前へ)
                        auto spearBack = NewGO<app::gimmick::Spear>(0);
                        spearBack->SetType(SpearType::Cross);

                        posBackStart_ = Vector3(x, y, SPAWN_Z_DIST);
                        posBackTarget_ = Vector3(x, y, SAFE_GAP_Z); // +7.0fで止まる

                        spearBack->SetStartPos(posBackStart_);
                        spearBack->SetTargetPos(posBackTarget_);
                    }

                    // --- パターン判定: 手前の壁を出すか？ ---
                    if (pattern_ == 1 || pattern_ == 2)
                    {
                        // 手前の壁 (Z- -> 奥へ)
                        auto spearFront = NewGO<app::gimmick::Spear>(0);
                        spearFront->SetType(SpearType::Cross);

                        posFrontStart_ = Vector3(x, y, -SPAWN_Z_DIST);
                        posFrontTarget_ = Vector3(x, y, -SAFE_GAP_Z); // -7.0fで止まる

                        spearFront->SetStartPos(posFrontStart_);
                        spearFront->SetTargetPos(posFrontTarget_);
                    }
                }
            }
        }
    } // namespace enemyState
} // namespace app
