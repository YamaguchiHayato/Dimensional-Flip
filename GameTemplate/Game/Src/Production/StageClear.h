#pragma once

#include "stdint.h"
#include "Src/Core/Game.h"
#include "Src/Actor/Stage/StageID.h"

/**
 * @file   StageClear.h
 * @brief  ステージクリアロゴ演出 GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        /**
         * @class StageClear
         * @brief  ステージクリア時に表示するロゴスプライト。
         */
        class StageClear : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            StageClear() = default;
            virtual ~StageClear() = default;

        public:
            /**
             * @brief 初期化。スプライトを読み込む。
             * @return 成功時 true。
             */
            bool Start();

            /**
             * @brief 毎フレームの更新。
             */
            void Update();

            /**
             * @brief 描画。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);

        public:
            /**
             * @brief 表示座標をセットする。
             * @param pos ワールド座標。
             */
            inline void SetPosition(const Vector3& pos)
            {
                position_ = pos;
            }

        private:
            /**
             * @brief 次ステージ ID を決定する。
             * @return 遷移先 StageID。
             */
            StageID DecideStage();

        private:
            SpriteRender stageClearSprite_; //!< ステージクリアロゴ
            Vector3 position_ = Vector3::Zero; //!< 表示位置
            float timer_ = 0.0f;             //!< 経過タイマー
        };
    } // namespace nsProduction
} // namespace nsApp

using StageClear = nsApp::nsProduction::StageClear;
