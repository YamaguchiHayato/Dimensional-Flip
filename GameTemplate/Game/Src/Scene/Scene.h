#pragma once
//
// 1.基底クラスとなるもの
// 　→シーン自体で何がいるか
// 　　どういった動きをさせるべきか
// 　　→シーンには何がいるか？
// 　　　シーン＝ゲームオブジェクトの集合体
// 　　　　　　→インゲームならPlayer,Enemyなど
// 　　　→Start＝初期化＝いるよね。なぜならゲームオブジェクトを作らないといけないから
// 　　　　　　　　　　　　NewGO<Player>。そのSceneに必要なものをつくる
// 　　　→Update＝なにかしら更新＝ゲームオブジェクトだけで考えるとUpdateはいらない？
// 　　　　　　　　　　　　　　　　なぜなら勝手に更新が呼ばれるから
// 　　　　タイトルからインゲームのシーン切替をします
// 　　　　ゲームオブジェクトの更新のためじゃなくて、シーン切替とか、
//         ゲームオブジェクト以外の更新場所を提供するために必要
//       →Render＝描画＝ゲームオブジェクトで勝手に呼ばれるからいらないよね


//
// シーンのインターフェース
// こういう挙動をしたいという考えのもとに、派生先で実装を強制する
//
class IScene 
{
public:
    IScene() {};
    virtual ~IScene() {};

    virtual bool Start() = 0 { return true; };
    virtual void Update() = 0 {};
};


// 今のシーンを識別する列挙型。
enum class SceneID : uint8_t
{
    sTitle = 0,
    sInGame,
    sResult,
    sGameOver,
    sInvalid = 0xff
};

