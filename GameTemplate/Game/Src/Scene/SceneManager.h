/*
 * @file    SceneManager.h
 * @brief   シーン管理クラス。
 */
#pragma once
#include "Scene.h"

/*
 * @enum    SceneID。
 * @brief   シーンのIDを管理する列挙型。
 * @details シーンの派生クラスにこの列挙型を継承させる。
 * @dote    2025/10/14 … 列挙型作成日。
 */
enum class SceneID : uint8_t
{
	sTitle,
	sStage1,
	sStage2,
	sGameOver,
	sGameClear,
	sResult,
	sInvalid,
};

class SceneManager : public SceneBase
{
private:
	SceneBase* scene_ = nullptr;
	SceneID requestID_ = SceneID::sTitle;
	static SceneManager* instance_;
	virtual ~SceneManager() {};

public:
	/* コンストラクタ。*/
	SceneManager() {};
	/* 初期化処理。*/
	bool Start() override { return true; };
	/* 更新処理。*/
	void Update() override;
	/* 描画処理。*/
	void Render(RenderContext& rc) override;
	/* シーンの遷移処理。*/
	void ChangeScene();
	/* シーンの解放処理。*/ 
	static SceneManager* GetInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new SceneManager();
		}
		return instance_;
	}
	/* シーンの解放処理。*/
	void SetRequest(SceneID id)
	{
		requestID_ = id;
	}
	/* シーンの取得処理。*/
	SceneID GetRequest()
	{
		return requestID_;
	};
	




};

