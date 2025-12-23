#include "stdafx.h"
#include <type_traits>
#include <array>

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
        // 敵生成関数の型定義。
        using CreateFn = IEnemy* (*) (const Vector3&);

        // 各種敵を生成するメソッド。
        // 各敵が連番の列挙であることが前提。
        // 通常敵。
        IEnemy* CreateNormal(const Vector3& pos)
        {
            // 生成。
            auto* pNormal = NewGO<NormalEnemy>(0, "normal");

            // 座標設定。
            pNormal->SetPos(pos);

            // 踏みつけて倒せるかどうか。
            pNormal->SetStompable(true);

            return pNormal;
        }


        // 追跡敵。
        IEnemy* CreateTracking(const Vector3& pos)
        {
            // 生成。
            // 生成。
            auto* pTracking = NewGO<TrackingEnemy>(0, "tracking");

            // 座標設定。
            pTracking->SetPos(pos);

            // 踏みつけて倒せるかどうか。
            pTracking->SetStompable(true);

            return pTracking;
        }


        // 回転敵。
        IEnemy* CreateThwomp(const Vector3& pos)
        {
            auto* pThwomp = NewGO<Thwomp>(0, "thwomp");

            // 座標設定。
            pThwomp->SetPos(pos);

            // 踏みつけて倒せるかどうか。
            // pEnemy->SetStompable(false);

            return pThwomp;
        }


        // EnemyType::type_Numを配列サイズにする。
        constexpr size_t enemyTypeCount = static_cast<size_t>(EnemyType::type_Num);


        // 生成関数テーブル。
        constexpr std::array<CreateFn, enemyTypeCount> create =
        {
            &CreateNormal,   // 通常敵。
            &CreateTracking, // 追跡敵。
            &CreateThwomp,   // 回転敵。
            nullptr,         // 生成関数が未実装の敵。
        };


        // デバッグ用。
        // 生成関数テーブルの要素数がEnemyType::type_Numと一致することを確認。
        // ※ 一致しなければ以下のメッセージを表示。
        static_assert(create.size() == enemyTypeCount, "Create function table size mismatch");


        // // 敵を生成するメソッド。
        IEnemy* EnemyFactory::CreateEnemy(EnemyType type, const Vector3& pos)
        {
            // 生成関数テーブルから生成関数を取得。
            const auto index = static_cast<size_t>(type);

            // 生成範囲外ならガードする
            // 不正な値が入った場合の対策。
            if (index >= create.size())
                return nullptr;

            // 未実装な関数が入った場合も同様にガードする。
            const auto fn = create[index];
            if (!fn)
                return nullptr;

            // 敵生成関数を呼んで敵を生成する。
            return fn(pos);
        } 
    }
}
