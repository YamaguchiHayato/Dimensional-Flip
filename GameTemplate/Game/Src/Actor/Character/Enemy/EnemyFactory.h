#pragma once
// 敵クラスのモデルをファクリパターンとして生成するクラス。
#include "Src/Actor/Character/Enemy/IEnemy.h"


// 生成する敵の種類。
enum class EnemyType : uint8_t
{
    type_Normal,   // 普通の敵。
    type_Tracking, // 追尾型の敵。
    type_Thwomp,   // 回転型の敵。
    type_Fall,     // 落下型の敵。

    type_Boss,     // ボス型の敵。
    type_Num,      // 敵の種類数。
};

#include "Src/Actor/Character/PlayerForward.h"

namespace app
{
    namespace enemy
    {
        using CreateFn = std::vector<IEnemy*> (*) (const Vector3&, Player*);

        class IEnemy;
        class EnemyFactory
        {
        public:
            // コンストラクタ。
            EnemyFactory() = default;


            // 敵を生成するメソッド。
            static std::vector<IEnemy*> CreateEnemy(EnemyType enemyType, const Vector3& pos, Player* pPlayer);


            // NormalEnemyを1体だけ生成するヘルパー。
            static IEnemy* CreateNormalSingle(const Vector3& pos);

        };
    }
}
