#pragma once

#include <algorithm>
#include <array>
#include <vector>

#include "NonCopyable.h"

/**
 * @file   CollisionManager.h
 * @brief  2D/3D 次元に応じてコリジョン有効状態を切り替えるシングルトン。
 */

namespace nsK2Engine
{
    class CollisionObject;
}

namespace nsApp
{
    namespace nsCollision
    {
        /**
         * @enum DimensionMode
         * @brief コリジョン適用の次元モード。
         */
        enum class DimensionMode : uint8_t
        {
            dim2D, //!< 2D モード
            dim3D  //!< 3D モード
        };

        /**
         * @enum CollisionProperty
         * @brief オブジェクトのコリジョン有効条件。
         */
        enum class CollisionProperty : uint8_t
        {
            AlwaysSolid, //!< 常に有効
            SolidOnly2D, //!< 2D モード時のみ有効
            SolidOnly3D  //!< 3D モード時のみ有効
        };

        /**
         * @struct CollisionEntry
         * @brief 登録済みコリジョンオブジェクトとそのプロパティ。
         */
        struct CollisionEntry
        {
            CollisionProperty property_;       //!< 有効条件。
            nsK2Engine::CollisionObject* pObj; //!< 対象オブジェクト。
        };

        /**
         * @struct IDimensionObserver
         * @brief 次元切り替え通知を受け取るオブザーバーインターフェース。
         */
        struct IDimensionObserver
        {
            virtual ~IDimensionObserver() = default;

            /**
             * @brief 次元モードが変更されたときに呼ばれる。
             * @param mode 新しい次元モード。
             */
            virtual void IDimensionChanged(DimensionMode mode) = 0;
        };

        /**
         * @class CollisionManager
         * @brief コリジョン登録と 2D/3D 切り替えを統括するシングルトン。
         */
        class CollisionManager : public NonCopyable
        {
        public:
            /**
             * @brief シングルトンインスタンスを取得する。
             * @return CollisionManager の参照。未生成なら new する。
             */
            static CollisionManager& GetInstance()
            {
                if (!instance)
                    instance = new CollisionManager();
                return *instance;
            }

            /**
             * @brief コリジョンオブジェクトを登録する。既登録ならプロパティのみ更新。
             * @param obj  対象 CollisionObject。
             * @param prop 有効条件。既定は AlwaysSolid。
             */
            void RegisterObject(nsK2Engine::CollisionObject* obj,
                                CollisionProperty prop = CollisionProperty::AlwaysSolid);

            /**
             * @brief コリジョンオブジェクトの登録を解除する。
             * @param unObj 解除するオブジェクト。
             */
            inline void UnRegisterObject(nsK2Engine::CollisionObject* unObj)
            {
                pObserver_.erase(std::remove_if(pObserver_.begin(), pObserver_.end(),
                                                [unObj](const CollisionEntry& entry) { return entry.pObj == unObj; }),
                                 pObserver_.end());
            }

            /**
             * @brief エントリのプロパティに応じてコリジョン ON/OFF を適用する。
             * @param entry 対象エントリ。
             */
            void ApplyCollisionState(CollisionEntry& entry);

            /**
             * @brief 次元変更オブザーバーを登録する。
             * @param obs オブザーバー。nullptr は無視。
             */
            void RegisterObserver(IDimensionObserver* obs);

            /**
             * @brief 次元変更オブザーバーの登録を解除する。
             * @param obs 解除するオブジェクト。
             */
            void UnRegisterObserver(IDimensionObserver* obs);

            /**
             * @brief 次元モードを変更し、全オブジェクトとオブザーバーに通知する。
             * @param mode 新しい次元モード。
             */
            inline void SetDimension(DimensionMode mode)
            {
                currentMode_ = mode;
                NotifyObservers();
            }

            /**
             * @brief 現在の次元モードを取得する。
             */
            DimensionMode GetCurrentMode() const { return currentMode_; }

        private:
            /* コンストラクタ。*/
            CollisionManager() = default;

            /**
             * @brief 登録オブジェクトとオブザーバーへ次元変更を通知する。
             */
            void NotifyObservers();

        private:
            static CollisionManager* instance; //!< シングルトンインスタンス。

            std::vector<CollisionEntry> pObserver_;                //!< 登録コリジョン一覧。
            std::vector<IDimensionObserver*> pDimensionObservers_; //!< 次元オブザーバー一覧。
            DimensionMode currentMode_ = DimensionMode::dim2D;     //!< 現在の次元モード。
        };
    } // namespace nsCollision
} // namespace nsApp

using DimensionMode = nsApp::nsCollision::DimensionMode;
using CollisionProperty = nsApp::nsCollision::CollisionProperty;
using CollisionEntry = nsApp::nsCollision::CollisionEntry;
using IDimensionObserver = nsApp::nsCollision::IDimensionObserver;
using CollisionManager = nsApp::nsCollision::CollisionManager;

namespace app
{
    namespace collision
    {
        using DimensionMode = nsApp::nsCollision::DimensionMode;
        using CollisionProperty = nsApp::nsCollision::CollisionProperty;
        using CollisionEntry = nsApp::nsCollision::CollisionEntry;
        using IDimensionObserver = nsApp::nsCollision::IDimensionObserver;
        using CollisionManager = nsApp::nsCollision::CollisionManager;
    } // namespace collision
} // namespace app
