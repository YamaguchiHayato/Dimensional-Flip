#pragma once

/**
 * @file   IComponent.h
 * @brief  Framework Component の共通インターフェース。
 */
namespace nsApp
{
    namespace nsFramework
    {
        class Entity;

        /**
         * @class IComponent
         * @brief Entity に付与される処理単位の基底インターフェース。
         * @note  IGameObject ではない。Entity の Update / Draw から呼ばれる。
         */
        class IComponent
        {
        public:
            virtual ~IComponent() = default;

            /**
             * @brief Entity に追加された直後に呼ばれる。
             * @param pOwner 所有 Entity。
             */
            virtual void OnAttach(Entity* pOwner) { pOwner_ = pOwner; }

            /**
             * @brief Entity から外される直前に呼ばれる。
             */
            virtual void OnDetach() { pOwner_ = nullptr; }

            /**
             * @brief 毎フレームの更新。
             * @param deltaTime 経過秒数。
             */
            virtual void OnUpdate(float deltaTime) { (void) deltaTime; }

            /**
             * @brief 毎フレームの描画。
             * @param rc            レンダリングコンテキスト。
             * @param ownerWorld    所有 Entity のワールド行列。
             */
            virtual void OnDraw(RenderContext& rc, const Matrix& ownerWorld)
            {
                (void) rc;
                (void) ownerWorld;
            }

            /**
             * @brief 所有 Entity を取得する。
             * @return 所有 Entity。未 Attach なら nullptr。
             */
            Entity* GetOwner() const { return pOwner_; }


        protected:
            Entity* pOwner_ = nullptr; //! 所有 Entity。未 Attach なら nullptr。
        };
    } // namespace nsFramework
} // namespace nsApp
