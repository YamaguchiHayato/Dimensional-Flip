#pragma once

#include <vector>

#include "Src/Scene/Scene.h"
#include "StageIcon.h"

/**
 * @file   WorldSelectScene.h
 * @brief  ステージ選択シーン。
 */

namespace app
{
    namespace nsUI
    {
        class WorldSelectUI;
    }
} 

class Fade;

/**
 * @class WorldSelectScene
 * @brief  SkyCube 背景と StageIcon 回転でステージを選ぶシーン。
 * @note   SetAmbientByIBLTexture は使わず SetAmbient のみ（IBL 漏れ防止）。
 */
class WorldSelectScene : public IScene
{
public:
    /* コンストラクタとデストラクタ。*/
    WorldSelectScene() = default;
    virtual ~WorldSelectScene();


public:
    /**
     * @brief シーン開始時の初期化処理。
     * @return true: 初期化成功、false: 初期化失敗
     */
    bool Start() override;

    /**
     * @brief シーン更新処理。
     */
    void Update() override;


private:
    /**
     * @brief 夜空 SkyCube を生成する（背景描画のみ。全局 IBL は設定しない）。
     */
    void CreateSkyCube();


private:
    Fade* pFade_ = nullptr;                         //!< フェード制御
    app::nsUI::WorldSelectUI* pSelectUI_ = nullptr; //!< ステージ選択 UI
    SkyCube* pSkyCube_ = nullptr;                   //!< 夜空 SkyCube

    std::vector<app::nsUI::StageIcon*> icons_;      //!< ステージプレビュー用 3D アイコン

    int currentIndex_ = 0;                          //!< 選択中インデックス
    float currentAngle_ = 0.0f;                     //!< 回転補間の現在角
    float targetAngle_ = 0.0f;                      //!< 回転補間の目標角

    bool isDecided_ = false;                        //!< A ボタン決定済み
    bool isButtonB_ = false;                        //!< B ボタンキャンセル済み
    bool isRight_ = false;                          //!< 右入力済み
    bool isLeft_ = false;                           //< 左入力済み
};
