#pragma once
#include <array>     // std::array
#include <vector>
#include <algorithm> // std::remove 
#include "NonCopyable.h"


namespace nsK2Engine{
    class CollisionObject;
}


namespace app
{
    namespace collision
    {
        // 現在のモード。
        enum class DimensionMode : uint8_t {
            dim2D,
            dim3D
        };


        // 衝突プロパティ。
        enum class CollisionProperty : uint8_t {
            AlwaysSolid, // 常に有効
            SolidOnly2D, // 2Dモード時のみ有効
            SolidOnly3D  // 3Dモード時のみ有効
        };


        struct CollisionEntry
        {
            CollisionProperty property_;
            nsK2Engine::CollisionObject* pObj;
        };


        struct IDimensionObserver
        {
            virtual ~IDimensionObserver() = default;
            virtual void IDimensionChanged(DimensionMode mode) = 0;
        };

        // エンジン内蔵のNonCopyableではなく、自作。
        class CollisionManager : public NonCopyable
        {
        public:
            // シングルトン登録。
            static CollisionManager& GetInstance()
            {
                if (!instance) 
                    instance = new CollisionManager();

                return *instance;
            }


            // オブザーバーへの登録。
            void RegisterObject(nsK2Engine::CollisionObject* obj,CollisionProperty prop = CollisionProperty::AlwaysSolid);


            // オブザーバーの登録解除。
            inline void UnRegisterObject(nsK2Engine::CollisionObject* unObj)
            {
                pObserver_.erase(std::remove_if(pObserver_.begin(), pObserver_.end(),[unObj](const CollisionEntry& entry) { return entry.pObj == unObj; }), pObserver_.end());
            }


            // コリジョンのON/OFF/適用する関数。
            void ApplyCollisionState(CollisionEntry& entry);


            // 通知。
            void RegisterObserver(IDimensionObserver* obs);
            void UnRegisterObserver(IDimensionObserver* obs);

        // セッター。
        public:
            // 次元の設定と通知。
            inline void SetDimension(DimensionMode mode)
            {
                currentMode_ = mode;
                NotifyObservers();
            }


        // ゲッター。
        public:
            // 今の次元モードを取得。
            DimensionMode GetCurrentMode() const
            {
                return currentMode_;
            }


        private:
            // オブザーバーへの通知。
            void NotifyObservers();

            
        private:
            static CollisionManager* instance;

        private:
            CollisionManager() = default;
            std::vector<CollisionEntry> pObserver_;
            std::vector<IDimensionObserver*> pDimensionObservers_;
            DimensionMode currentMode_ = DimensionMode::dim2D;
            
        };
    }
}

