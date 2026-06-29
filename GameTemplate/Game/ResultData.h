#pragma once

// リザルト画面のデータを管理するデータクラス。
namespace nsApp
{
    namespace nsStage
    {
        /**
         * @class StageResultData 
         * @brief ステージリザルトデータを保持する構造体。
         * @details ゲーム内部のスコア、クリアタイム、倒した敵の数、ノーダメージクリアフラグを保持する。
         */
        struct StageResultData
        {
            int baseScore_;          //! ゲーム内部のスコアデータ。
            float clearTime_;        //! ゲーム内部のタイムデータ。
            uint8_t defeatedEnemies; //! 倒した敵の数。
            bool isNoDamge_;         //! ノーダメージクリアかどうか。

            /**
             * @brief コンストラクタ。メンバ変数を初期化する。
             * @param baseScore 初期スコア。
             * @param clearTime 初期クリアタイム。
             * @param defeatedEnemies 初期倒した敵の数。
             * @param isNoDamge 初期ノーダメージフラグ。
             */
            StageResultData() : baseScore_(0), clearTime_(0.0f), defeatedEnemies(0), isNoDamge_(false) {}
        };
    }
}
