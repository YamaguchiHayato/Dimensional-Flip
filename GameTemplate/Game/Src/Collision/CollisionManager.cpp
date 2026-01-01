#include "stdafx.h"
#include "CollisionManager.h"

#include "../../k2Engine/collision/CollisionObject.h"

namespace app
{
    namespace collision
    {
        // 静的メンバ変数の初期化。
        // SingletonInstance。
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
            for (auto& entry : pObserver_){
                ApplyCollisionState(entry);
            }


            // オブザーバ通知が必要なクラスに通知。
            for (auto* obs : pDimensionObservers_){
                obs->IDimensionChanged(currentMode_);
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


        void CollisionManager::RegisterObject(nsK2Engine::CollisionObject* obj, CollisionProperty prop)
        {
            if (!obj)
                return;


            // 既に追加されているか確認する。
            auto it = std::find_if(pObserver_.begin(), pObserver_.end(),
                [obj](const CollisionEntry& e) { return e.pObj == obj; });

            // 存在確認。
            if (it != pObserver_.end())
            {
                // 既に存在する場合はプロパティだけ更新
                it->property_ = prop;
                ApplyCollisionState(*it);

            }


            else
            {
                // 新規登録
                pObserver_.push_back({prop, obj});

                // 登録直後に状態を適用。
                ApplyCollisionState(pObserver_.back());

            }
        }


        void CollisionManager::RegisterObserver(IDimensionObserver* obs)
        {
            if (!obs)
                return;

            auto it = std::find(pDimensionObservers_.begin(), pDimensionObservers_.end(), obs);


            if (it != pDimensionObservers_.end())
                return;


            pDimensionObservers_.push_back(obs);
        }


        void CollisionManager::UnRegisterObserver(IDimensionObserver* obs)
        {
            pDimensionObservers_.erase(std::remove
            (
                pDimensionObservers_.begin(),
                pDimensionObservers_.end(), obs),
                pDimensionObservers_.end()
            );
        }
    } 
} 
