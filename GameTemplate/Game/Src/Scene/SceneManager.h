#pragma once
#include "Src/Scene/Scene.h"
class SceneManager : public IScene
{
private:
    IScene* pCurrentScene_ = nullptr;
    SceneID currentID_ = SceneID::sTitle;
    SceneID requestID_ = SceneID::sInvalid;

public:
    // 初期化処理。
    bool Start() override;
    // 更新処理。
    void Update() override;
    // シーンの変更処理。
    inline void ChangeScene(SceneID newID)
    {
        requestID_ = newID;
    }

    IScene* CreateScene(SceneID id);

private:
    // コピーコンストラクタ・代入演算子をデフォルト化。
    SceneManager(/* const SceneManager&*/) /*= default*/{};
//    SceneManager& operator=(const SceneManager&) = default;
    virtual ~SceneManager() {};

private:
    static SceneManager* pSceneManger_;

public:
    // シングルトンインスタンスの生成。
    inline static void CreateInstance()
    {
        if (!pSceneManger_)
            pSceneManger_ = new SceneManager();
    }

    // シングルトンインスタンスを取得
    inline static SceneManager* GetInstance()
    {
        return pSceneManger_;
    }

    // シーンの解放処理。
    inline static void DeleteInstance()
    {
        delete pSceneManger_;
        pSceneManger_ = nullptr;
    }
};
