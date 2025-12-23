#pragma once
// 敵クラスのモデルをファクリパターンとして生成するクラス。
#include "Src/Actor/Character/Enemy/IEnemy.h"


// 生成する敵の種類。
enum class EnemyType : uint8_t
{
    type_Normal,   // 普通の敵。
    type_Tracking, // 追尾型の敵。
    type_Thwomp,   // 回転型の敵。 
    type_Boss,     // ボス型の敵。

    type_Num,      // 敵の種類数。
};


namespace app
{
    namespace enemy
    {
        class IEnemy;
        class EnemyFactory
        {
        public:
            // コンストラクタ。
            EnemyFactory() = default;
            // static関数を用いるためデストラクタは不要。


            // 敵を生成するメソッド。
            static IEnemy* CreateEnemy(EnemyType enemyType, const Vector3& pos);
        };
    }
}
