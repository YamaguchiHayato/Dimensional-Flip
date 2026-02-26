#pragma once

// リザルト画面のデータを管理するデータクラス。
namespace app
{
    struct StageResultData
    {
        int baseScore_;           // ゲーム内部のスコアデータ。
        float clearTime_;             // ゲーム内部のタイムデータ。
        uint8_t defeatedEnemies;      // 倒した敵の数。
        bool isNoDamge_;              // ノーダメージクリアかどうか。


        // 初期化用コンストラクタ。
        StageResultData()
            : baseScore_(0),
              clearTime_(0.0f),
              defeatedEnemies(0),
              isNoDamge_(false)
        {}
    };
}
