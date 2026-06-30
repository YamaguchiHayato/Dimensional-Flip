#pragma once

#include "Src/Scene/Scene.h"

/**
 * @file   SceneManager.h
 * @brief  シーン遷移・ロード画面・フェードを統括するマネージャ。
 */

/**
 * @enum SceneTransitionState
 * @brief Game 内ステージ遷移用（SceneManager とは別系統）。
 */
enum class SceneTransitionState : uint8_t
{
    None,
    FadeOut,
    Load,
    Load_Render,
    Load_Wait,
    Load_WaitFinish,
    FadeIn,
    FadeIn_Wait,
};

class Fade;
class LoadingScene;

/**
 * @class SceneManager
 * @brief 全シーンの生成・破棄・遷移タイミングを管理するシングルトン。
 */
class SceneManager : public IScene
{
private:
    IScene* pCurrentScene_ = nullptr;       //! < 現在のシーン。>
    SceneID currentID_ = SceneID::sTitle;   //! < 現在のシーン ID。>
    SceneID requestID_ = SceneID::sInvalid; //! < 遷移リクエストされたシーン ID。>
    SceneID targetID = SceneID::sInvalid;   //! < 遷移先のシーン ID。>
    static SceneManager* pSceneManger_;     //! < シングルトンインスタンス。>
    Fade* pFade_ = nullptr;                 //!< フェード制御。>
    LoadingScene* pLoadingScene_ = nullptr; //!< ロード画面。>


private:
    bool isLoadingSceneActive_ = false;     //< ロード画面がアクティブか。>
    bool isSceneControler_ = false;         //!< シーン遷移中の制御中か。>
    bool isAutoLoadingEnabled_ = true;      //!< フェードアウト中の自動ロード表示を有効にするか。>
    float minLoadingTime_ = 0.0f;           //!< ロード画面の最低表示時間（秒）


public:
    /**
     * @brief シーンマネージャを初期化する。
     * @return true で初期化成功。
     */
    bool Start() override;

    /**
     * @brief シーンマネージャを更新する。
     */
    void Update() override;

    /**
     * @brief シーンマネージャを終了する。
     * @param id 終了するシーン ID。
     * @return true で終了成功。
     */
    IScene* CreateScene(SceneID id);

    /**
     * @brief ロード画面を表示する。
     */
    void ShowLoading();

    /**
     * @brief ロード画面を非表示にする。
     */
    void HideLoading();

    /**
     * @brief フェードアウト中の自動ロード表示を有効/無効にする。
     * @param enable true で有効。
     */
    void SetEnableAutoLoading(bool enable) { isAutoLoadingEnabled_ = enable; }

    /**
     * @brief ロード画面の最低表示時間を設定する。
     * @param newID 遷移先シーン ID。
     */
    inline void ChangeSceneInternal(SceneID newID) { requestID_ = newID; }

    /**
     * @brief シーン遷移をリクエストする。
     * @param newID 遷移先シーン ID。
     */
    inline void ChangeScene(SceneID newID) { requestID_ = newID; }

    /**
     * @brief シーン退場時に描画エンジンのインゲーム専用状態を解除する。
     * @details 数式背景（Composite）とステージ背景コールバックを解除。
     */
    static void ResetRenderingStateForScene()
    {
        if (!g_renderingEngine)
            return;
        g_renderingEngine->EnableCompositeBackground(false);
        g_renderingEngine->SetStageBackGroundRenderer(nullptr);
    }

private:
    /* コンストラクタとデストラクタ。*/
    SceneManager() = default;
    virtual ~SceneManager();


public:
    /**
     * @brief シングルトンインスタンスを生成する。
     */
    inline static void CreateInstance()
    {
        if (!pSceneManger_)
            pSceneManger_ = new SceneManager();
    }

    /**
     * @brief フェード制御を取得する。
     * @return フェード制御のポインタ。
     */
    inline Fade* GetFade()
    {
        return pFade_;
    }

    /**
     * @brief 遷移先のシーン ID を取得する。
     * @return 遷移先のシーン ID。
     */
    SceneID GetTargetSceneID() const
    {
        return targetID;
    }

    /**
     * @brief シングルトンインスタンスを取得する。
     * @return シングルトンインスタンスのポインタ。
     */
    inline static SceneManager* GetInstance() { return pSceneManger_; }

    /**
     * @brief シングルトンインスタンスを削除する。
     */
    inline static void DeleteInstance()
    {
        delete pSceneManger_;
        pSceneManger_ = nullptr;
    }
};
