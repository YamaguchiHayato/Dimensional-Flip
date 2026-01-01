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

            const auto rectW = 25.0f;           // 壁の内側の「横幅」(X方向)  ※壁の厚みや内側幅
            const auto rectD = 25.0f;           // 壁の内側の「奥行き」(Z方向)

            const auto marginX = 20.0f;         // 壁から内側へ寄せる量（X）
            const auto marginZ = 20.0f;         // 壁から内側へ寄せる量（Z）

            // 密度。
            const auto desiredSpacingX = 80.0f; // X成分。
            const auto desiredSpacingZ = 60.0f; // Z成分。

            // 使える範囲
            const auto usableW = rectW - marginX * 2.0f;
            const auto usableD = rectD - marginZ * 2.0f;

            // 個数を決める（最低1）
            uint8_t nx = 6;
            uint8_t nz = 6;
            if (usableW > 0.0f)
                nx = static_cast<int>(usableW / desiredSpacingX) + 1;
            if (usableD > 0.0f)
                nz = static_cast<int>(usableD / desiredSpacingZ) + 1;
            if (nx < 1)
                nx = 1;
            if (nz < 1)
                nz = 1;

            const auto stepX = (nx <= 1) ? 0.0f : (usableW / (nx - 1));
            const auto stepZ = (nz <= 1) ? 0.0f : (usableD / (nz - 1));


            const auto startX = (-rectW * 0.5f) + marginX;
            const auto startZ = (-rectD * 0.5f) + marginZ;

            for (uint8_t ix = 0; ix < nx; ++ix)
            {
                for (uint8_t iz = 0; iz < nz; ++iz)
                {
                    const auto x = startX + ix * stepX;
                    const auto z = startZ + iz * stepZ;

                    // 地面にめり込まないようにするためy座標を上げる。
                    Vector3 finalPos = pos + Vector3(x, 0.0f, z);

                    auto* e = NewGO<app::enemy::NormalEnemy>(0);
                    app::enemy::NormalEnemy::SpawnParam param(finalPos, Vector3(0.25f, 0.25f, 0.25f), 80.0f, true);
                    e->InitParam(param);
                    e->SetStompable(true);
                    spawnedEnemies.push_back(e);
                }
            }
            return spawnedEnemies;
        }


        std::vector<IEnemy*> CreateTracking(const Vector3& pos)
        {   
            std::vector<IEnemy*> spawnList;
            const uint8_t num = 6;
            const auto interval = 150.0f;

            for (uint8_t i = 0; i < num; ++i)
            {
                Vector3 offsetPos = {(float) i * interval, 0.0f, 0.0f};
                Vector3 finalPos = pos + offsetPos;

                auto* pTracking = NewGO<TrackingEnemy>(0);
                pTracking->SetPos(finalPos);

                spawnList.push_back(pTracking);
            }

            return spawnList;
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
