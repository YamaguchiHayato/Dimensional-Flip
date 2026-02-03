#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/RotationFool.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace
{
    const Vector3 GIMMICKCOLLISION(0.0f, 0.0f, 0.0f);//コリジョンの高さ
	const Vector3 SCALE(3.0f, 3.0f, 5.0f);	//モデルの大きさ

}

namespace GimmickPos
{
    // 回転トリック。
    namespace RotationFoolPosition
    {
        const Vector3 Pos1(300.0f, 0.0f, -0.0f);
    }
}

RotationFool::~RotationFool()
{
    if (pGimmickCollision_)
    {
        DeleteGO(pGimmickCollision_);
        pGimmickCollision_ = nullptr;
    }
}

bool RotationFool::Start()
{
	//ファイルパス
    std::string foolPath = InitGimmick("Stage2/RotationFool");
    render_.Init(foolPath.c_str(), 0, 0, enModelUpAxisZ, false, true);// モデルをセットする。
	
	// モデルの大きさ。
	render_.SetScale(SCALE);
	// モデルの更新作業。
	render_.Update();

	// 探索処理。
	pPlayer = FindGO<Player>("player");

    // 物理。
    gimmickPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());
	// コリジョン。
	pGimmickCollision_ = NewGO<CollisionObject>(0, "collisionobject");
	//コリジョンを動く床に設置
	pGimmickCollision_->CreateBox(gimmickPos_ + GIMMICKCOLLISION,Quaternion::Identity, GIMMICKCOLLISION);
	// 座標を設定。
	render_.SetPosition(gimmickPos_);
	// コリジョンを破棄。
	pGimmickCollision_->SetIsEnableAutoDelete(false);
    return true;
}

void RotationFool::Update()
{
    // ギミックの移動。
    Move(g_gameTime->GetFrameDeltaTime());
    // ギミックモデルの更新。
    render_.Update();
    // ギミックモデルの座標更新。
    render_.SetPosition(gimmickPos_);
    // ギミック物理の座標更新。
    gimmickPhysics_.SetPosition(gimmickPos_);
    // ギミックコリジョンの設定。
    pGimmickCollision_->SetPosition(gimmickPos_ + GIMMICKCOLLISION);
}


void RotationFool::Render(RenderContext& rc)
{
    render_.Draw(rc);
}


void RotationFool::Move(float deltaTime)
{
    // 
    switch (foolState_)
    {
    /////////////////////////////////////////
    // A. 上昇状態。
    /////////////////////////////////////////
    case FoolState::UP:
        // 上昇中。
        gimmickPos_.y += moveSpeed_* g_gameTime->GetFrameDeltaTime();
        // 上限に達したかをチェック。
        if (gimmickPos_.y >= GetTopPos().y)
        {
            // 座標を上端に合わせる。
            gimmickPos_.y = GetTopPos().y;
            // ステートをTOPに変更。
            foolState_ = FoolState::TOP;
            // 待機タイマーをセットする。
            SetStopTime(2.0f);

            render_.SetPosition(gimmickPos_);
        }
        break;

    /////////////////////////////////////////
    // B. 上停止状態。
    ////////////////////////////////////////
    case FoolState::TOP:
        // 停止時間のカウントダウンを始める。
        stopTime_ -= deltaTime;

        // タイマーが0以下になったら。
        if (stopTime_ <= 0.0f)
        {
            // タイマーをリセットする。
            SetStopTime(2.0f); 
            // ステートをDOWNに変更する。
            foolState_ = FoolState::DOWN;
        }
    break;

    /////////////////////////////////////////
    // C. 下降状態。
    ////////////////////////////////////////
    case FoolState::DOWN:
        // 下降中。
        gimmickPos_.y -= moveSpeed_ * g_gameTime->GetFrameDeltaTime();

        // 下端に到達したか。
        if (gimmickPos_.y <= GetInitPos().y)
        {
            // 座標を下端に合わせる。
            gimmickPos_.y = GetInitPos().y;
            // ステートをBOTTOMに変更する。
            foolState_ = FoolState::BOTTOM;
            // 待機タイマーをセットする。
            SetStopTime(2.0f);
        }
        break;
    /////////////////////////////////////////
    // D. 下降状態。
    ////////////////////////////////////////
    case FoolState::BOTTOM:
        // 停止時間のカウントダウンを始める。
        stopTime_ -= deltaTime;

        // タイマーが0以下になったら。
        if (stopTime_ <= 0.0f)
        {
            SetStopTime(2.0f); // タイマーをリセットする。
            foolState_ = FoolState::UP;
        }
        break;
    default:
        break;
    }
}
