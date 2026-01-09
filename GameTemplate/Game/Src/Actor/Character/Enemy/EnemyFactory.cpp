#include "stdafx.h"

#include <cstdlib>
#include <array>
#include <ctime>

#include "Src/Actor/Character/Enemy/EnemyFactory.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"

// 具体的な敵クラスのインクルード。
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"
#include "Src/Actor/Character/Enemy/Thwomp.h"
#include "Src/Actor/Character/Enemy/TrackingEnemy.h"


namespace ThwompSpawnPosition
{
    // 生成位置。。
    // 上空から落ちてくるように高いY座標を指定。
    const Vector3 spawnPosition = Vector3(1150.0f, 100.0f, 0.0f);

    // トリガー座標。
    // Playerがこの座標付近に来るとフラグを立てる。
    const Vector3 triggerPosition = Vector3(950.0f, 0.0f, 0.0f);

    // 坂道の方向ベクトル。
    // 斜面にあわせてY座標を調整する。
    const Vector3 slopeDirection = Vector3(-1.0f, -0.1f, 0.0f);
}

namespace
{
    const auto SLOPE_WIDTH = 18.0;
}

namespace
{
    // NormalEnemyを1体だけ生成するヘルパー。
    app::enemy::IEnemy* CreateNormalSingle(const Vector3& pos)
    {
        auto* e = NewGO<app::enemy::NormalEnemy>(0);
        // パラメータは適宜調整してください
        app::enemy::NormalEnemy::SpawnParam param(pos, Vector3(0.25f, 0.25f, 0.25f), 80.0f, true);
        e->InitParam(param);
        e->SetStompable(true);
        return e;
    }


    // TrackingEnemyを1体だけ生成するヘルパー。
    app::enemy::IEnemy* CreateTrackingSingle(const Vector3& pos)
    {
        auto* pTracking = NewGO<app::enemy::TrackingEnemy>(0);
        pTracking->SetPos(pos);
        return pTracking;
    }


    // NormalEnemyを行列生成する際の定数をまとめた構造体。
    struct NormalEnemyCreateStatus
    {
        // 内側の量。
        static constexpr auto RECT_W = 25.0f;   // 壁の内側の「横幅」(X方向)  ※壁の厚みや内側幅
        static constexpr auto RECT_D = 25.0f;   // 壁の内側の「奥行き」(Z方向)

        // 寄せる量。
        static constexpr auto MARGIN_X = 20.0f; // 壁から内側へ寄せる量（X）
        static constexpr auto MARGIN_Z = 20.0f; // 壁から内側へ寄せる量（X）

        // 密度。
        static constexpr auto DESIRED_SPACING_X = 80.0f; // X成分。
        static constexpr auto DESIRED_SPACING_Z = 60.0f; // X成分。

    };
}


namespace app
{
    namespace enemy
    {
        // 生成関数の型定義（全て vector を返すように統一）。
        using CreateFn = std::vector<IEnemy*> (*)(const Vector3&);

        // 
        using Status = NormalEnemyCreateStatus;


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
            const auto usableW = Status::RECT_W - Status::MARGIN_X * 2.0f;
            const auto usableD = Status::RECT_D - Status::MARGIN_Z * 2.0f;

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

            // 複数体生成させる。
            for (uint8_t i = 0; i < 15; ++i)
            {
                auto* spawnInstance = NewGO<Thwomp>(0, "thwomp");

                // 生成時にZ座標をランダムに決定する。
                // 0 〜 1.0の乱数を作成。
                auto randomSeed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

                // Z座標の範囲を計算。
                auto randomZscore = (randomSeed * SLOPE_WIDTH) - (SLOPE_WIDTH * 0.5f);

                // 座標をセット。
                Vector3 finalPos = pos;
                finalPos.z += randomZscore;

                // 生成位置をセット 
                spawnInstance->SetPos(finalPos);

                // 感知用トリガー座標をセット
                spawnInstance->SetTriggerPos(ThwompSpawnPosition::triggerPosition);

                // 進行方向をセット
                spawnInstance->InitMoveDir(ThwompSpawnPosition::slopeDirection);

                // スポーンするクールタイムを設定する。
                spawnInstance->SetSpawnDelay(static_cast<float>(i) * 1.5f);

                // リストに追加。
                list.push_back(spawnInstance);

            }
            return list;
        }


        std::vector<IEnemy*> EnemyFactory::CreateRandom(uint8_t count, Vector3& minPos, const Vector3& maxPos)
        {
            std::vector<IEnemy*> list;

            for (int i = 0; i < count; ++i)
            {
                Vector3 spawnPos;

                float tX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                spawnPos.x = minPos.x + tX * (maxPos.x - minPos.x);

                // Y座標（高さ）は最小値を基準にする
                spawnPos.y = minPos.y;

                // Z座標のランダム計算
                float tZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                spawnPos.z = minPos.z + tZ * (maxPos.z - minPos.z);

                int type = rand() % 2;

                if (type == 0)
                    list.push_back(CreateNormalSingle(spawnPos));


                else
                    list.push_back(CreateTrackingSingle(spawnPos));
            }
            return list;
        }


        // 敵タイプの数。
        constexpr size_t enemyTypeCount = static_cast<size_t>(EnemyType::type_Num);


        constexpr std::array<CreateFn, enemyTypeCount> create = {
            &CreateNormal,   // 通常敵（行列）。
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
