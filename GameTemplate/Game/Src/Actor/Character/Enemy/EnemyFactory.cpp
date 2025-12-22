#include "stdafx.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"


// 生成する敵の種類。
#include "Src/Actor/Character/Enemy/NormalEnemy.h"    // 通常敵。 
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"  // 追跡敵。
#include "Src/Actor/Character/Enemy/Thwomp.h"         // 回転敵。



namespace app
{
    namespace enemy
    {
        IEnemy* EnemyFactory::CreateEnemy(EnemyType type, const Vector3& pos)
        {
            IEnemy* pEnemy = nullptr;


            // 生成分岐処理。
            switch (type)
            {
            // 通常敵。
            case EnemyType::type_Normal:
            {
                // 生成。
                pEnemy = NewGO<app::enemy::NormalEnemy>(0, "normal");

                // 座標設定。
                pEnemy->SetPos(pos);

                // 踏みつけて倒せるかどうか。
                pEnemy->SetStompable(true);
                break;
            }


            // 追跡敵。
            case EnemyType::type_Tracking:
            {
                // 生成。
                pEnemy = NewGO<app::enemy::TrackingEnemy>(0, "tracking");

                // 座標設定。
                pEnemy->SetPos(pos);

                // 踏みつけて倒せるかどうか。
                pEnemy->SetStompable(true);
            }


            // 回転敵。
            case EnemyType::type_Thwomp:
            {
                // 生成。
                pEnemy = NewGO<app::enemy::Thwomp>(0, "thwomp");

                // 座標設定。
                pEnemy->SetPos(pos);

                // 踏みつけて倒せるかどうか。
                // pEnemy->SetStompable(false);

            }
            default:
                break;

            // 生成した敵を返す。
            return pEnemy;
            }
        }
    }
}
