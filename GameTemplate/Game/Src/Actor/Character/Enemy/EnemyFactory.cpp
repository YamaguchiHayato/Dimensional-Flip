#include "stdafx.h"

#include <array>

#include "Src/Actor/Character/Enemy/EnemyFactory.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"

// 具体的な敵クラスのインクルード。
#include "Src/Actor/Character/Enemy/NormalEnemy.h"
#include "Src/Actor/Character/Enemy/Thwomp.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"

namespace app
{
    namespace enemy
    {
        // 生成関数の型定義（全て vector を返すように統一）。
        using CreateFn = std::vector<IEnemy*> (*)(const Vector3&);


        // --- 通常敵：3x3の隊列パターン ---
        std::vector<IEnemy*> CreateNormal(const Vector3& pos)
        {
            std::vector<IEnemy*> spawnedEnemies;
            const uint8_t num = 3;


            for (uint8_t i = 0; i < num; ++i)
            {
                for (uint8_t j = 0; j < num; j++)
                {
                    // pos を起点に座標計算。
                    Vector3 offsetPos = {(float) i * 100.0f, 0.0f, (float) j * 100.0f};
                    Vector3 finalPos = pos + offsetPos;


                    auto* pNormal = NewGO<NormalEnemy>(0);
                    pNormal->SetPos(finalPos);


                    // ここでサイン波移動などの初期設定を行う
                    // pNormal->SetAmplitude(50.0f);


                    pNormal->SetStompable(true);
                    spawnedEnemies.push_back(pNormal);
                }
            }
            return spawnedEnemies;
        }


        std::vector<IEnemy*> CreateTracking(const Vector3& pos)
        {
            std::vector<IEnemy*> list;
            auto* p = NewGO<TrackingEnemy>(0);


            p->SetPos(pos);
            list.push_back(p);


            return list;
        }


        std::vector<IEnemy*> CreateThwomp(const Vector3& pos)
        {
            std::vector<IEnemy*> list;
            auto* p = NewGO<Thwomp>(0, "thwomp");


            p->SetPos(pos);
            list.push_back(p);


            return list;
        }

        constexpr size_t enemyTypeCount = static_cast<size_t>(EnemyType::type_Num);


        constexpr std::array<CreateFn, enemyTypeCount> create = {
            &CreateNormal,   // 通常敵（隊列）。
            &CreateTracking, // 追跡敵。
            &CreateThwomp,   // 回転敵。
            nullptr,         // ボス（未実装）。
        };


        static_assert(create.size() == enemyTypeCount, "Create function table size mismatch");


        // 工場メソッド本体。
        std::vector<IEnemy*> EnemyFactory::CreateEnemy(EnemyType type, const Vector3& pos)
        {
            const auto index = static_cast<size_t>(type);

            if (index >= create.size() || !create[index])
                return {}; 

            return create[index](pos);
        }

    }
} 
