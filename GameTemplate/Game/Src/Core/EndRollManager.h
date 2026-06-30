#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"
#include "Src/Production/Fade.h"
#include <vector>

/**
 * @file   EndRollManager.h
 * @brief  エンドロール全体（画像・テキスト・フェード）を統括する GO。
 */

namespace nsApp
{
    namespace nsProduction
    {
        class EndRollImage;
        class EndRollText;

        /**
         * @class EndRollManager
         * @brief  EndRollImage / EndRollText / Fade を束ねるマネージャ。
         */
        class EndRollManager : public EndRollBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            EndRollManager() = default;
            virtual ~EndRollManager();


        public:
            /**
             * @brief 初期化処理。EndRollImage / EndRollText / Fade を生成する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 更新処理。EndRollImage / EndRollText / Fade を更新する。
             */
            void Update() override;

            /**
             * @brief 描画処理。EndRollImage / EndRollText / Fade を描画する。
             * @param rc 
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief スキップフォントを初期化する。
             */
            void InitSkipFont();

            /**
             * @brief スキップ処理。EndRollImage / EndRollText / Fade を即終了させる。
             */
            void TrySkip();


        public:
            /**
             * @brief 終了判定。EndRollImage / EndRollText / Fade がすべて終了しているかを返す。
             * @return 終了していれば true。
             */
            bool IsEnd() const;


        private:
            std::vector<EndRollBase*> components_; //! < EndRollImage / EndRollText / Fade のリスト
            EndRollImage* pImage_ = nullptr;       //! < EndRollImage のポインタ
            EndRollText* pText_ = nullptr;         //! < EndRollText のポインタ
            Fade* pFade_ = nullptr;                //! < Fade のポインタ
            bool isFadeOutStarted_ = false;        //! < フェードアウトが開始されたか
            FontRender skipFont_;                  //! < スキップフォント
        };
    } // namespace nsProduction
} // namespace nsApp

using EndRollManager = nsApp::nsProduction::EndRollManager;
