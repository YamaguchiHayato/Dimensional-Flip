#include "stdafx.h"
#include "CollisionManager.h"

#include "../../k2Engine/collision/CollisionObject.h"

namespace app
{
    namespace collision
    {
        // 静的メンバ変数の初期化。
        CollisionManager* CollisionManager::instance = nullptr;


        namespace
        {
            // 現在のモード。
            using Predicate = bool(*)(DimensionMode);


            // 各プロパティに対応する判定ロジックを配列に。
            // →インデックスがCollisionPropertyと対応する必要が前提。


            //  3は要素数。
            const std::array<Predicate, 3> TABLE =
            {
                [](DimensionMode) { return true; },
                [](DimensionMode mode) { return mode == DimensionMode::dim2D; },
                [](DimensionMode mode) { return mode == DimensionMode::dim3D; },
            };
        }


        void CollisionManager::NotifyObservers()
        {
            for (auto& entry : pObserver_)
            {
                if (entry.pObj)
                {
                    // 
                    ApplyCollisionState(entry);


                    // デバッグログが欲しい場合は記載。
                }
            }
        }


        void CollisionManager::ApplyCollisionState(CollisionEntry& entry)
        {
            if (!entry.pObj)
                return;

            // 
            size_t index = static_cast<size_t>(entry.property_);

            // テーブルから関数を取得して実行。
            bool enable = TABLE[index](currentMode_);
            entry.pObj->SetIsEnable(enable);
        }
    } 
} 
