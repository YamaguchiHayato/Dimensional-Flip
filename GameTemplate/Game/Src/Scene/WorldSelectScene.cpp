#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Production/Fade.h"
#include "Src/UI/Select/WorldSelectUI.h"
#include "WorldSelectScene.h"

namespace
{
    const float RADIUS = 25.0f; //! アイコン配置円の半径

    struct StageData
    {
        const char* path;    //! プレビュー tkm パス
        StageID id;          //! ステージ ID
        const wchar_t* name; //! UI 表示名
    };

    std::vector<StageData> dataList = {
        {"Assets/stage/Stage1/Stage1.tkm", StageID::sStage1, L"Stage1"},
        {"Assets/stage/tutorialStage.tkm", StageID::sTutorialStage, L"Tutorial Stage"},
        {"Assets/stage/WorldSelect/BossStage.tkm", StageID::sStageEX, L"Boss Stage"},
    };
} // namespace

namespace nsApp
{
    namespace nsScene
    {
        WorldSelectScene::~WorldSelectScene()
        {
            /**
             * @brief ステージプレビューアイコンをすべて削除する
             */
            for (auto* icon : icons_)
                DeleteGO(icon);
            icons_.clear();

            /**
             * @brief 選択 UI を削除する
             */
            if (pSelectUI_)
            {
                DeleteGO(pSelectUI_);
                pSelectUI_ = nullptr;
            }

            /**
             * @brief 背景 SkyCube を削除する
             */
            if (pSkyCube_)
            {
                DeleteGO(pSkyCube_);
                pSkyCube_ = nullptr;
            }
        }

        bool WorldSelectScene::Start()
        {
            pFade_ = SceneManager::GetInstance()->GetFade();
            if (pFade_)
                pFade_->StartFadeIn();

            /**
             * @brief カメラをステージアイコン全体が見える位置に置く
             */
            if (g_camera3D)
            {
                g_camera3D->SetPosition({0.0f, 20.0f, -40.0f});
                g_camera3D->SetTarget({0.0f, 0.0f, 0.0f});
            }

            nsCore::SoundManager::GetInstance()->PlayBGM(GameSoundList_BGM_StageSelect);

            /**
             * @brief インゲーム描画状態を解除してから環境光だけ設定する
             */
            if (g_renderingEngine)
            {
                g_renderingEngine->EnableCompositeBackground(false);
                g_renderingEngine->SetStageBackGroundRenderer(nullptr);
                g_renderingEngine->SetAmbient(Vector3(0.4f, 0.40f, 0.5f));
            }

            CreateSkyCube();

            /**
             * @brief ModelRender を持つ StageIcon を生成する
             */
            for (const auto& data : dataList)
            {
                auto* icon = NewGO<nsUI::StageIcon>(0, "StageIcon");
                icon->Init(data.path, data.id);
                icons_.push_back(icon);
            }

            pSelectUI_ = NewGO<nsUI::WorldSelectUI>(1, "WorldSelectUI");
            pSelectUI_->Init();

            return true;
        }

        void WorldSelectScene::Update()
        {
            /**
             * @brief フェードアウト前のみ入力を受け付ける
             */
            if (!isDecided_ && pFade_ && pFade_->IsFadeOutEnd() == false)
            {
                bool isRight = g_pad[0]->IsTrigger(enButtonRight); //! 右入力
                if (isRight)
                {
                    currentIndex_++;
                    if (currentIndex_ >= (int) icons_.size())
                        currentIndex_ = 0;
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 3.0f);
                }

                bool isLeft = g_pad[0]->IsTrigger(enButtonLeft); //! 左入力
                if (isLeft)
                {
                    currentIndex_--;
                    if (currentIndex_ < 0)
                        currentIndex_ = (int) icons_.size() - 1;
                }

                /**
                 * @brief A ボタンでステージ決定
                 */
                if (g_pad[0]->IsTrigger(enButtonA))
                {
                    isDecided_ = true;
                    pFade_->StartFadeOut();
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button, 3.0f);
                }

                /**
                 * @brief B ボタンでタイトルへ戻る
                 */
                if (g_pad[0]->IsTrigger(enButtonB))
                {
                    isButtonB_ = true;
                    pFade_->StartFadeOut();
                    nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SelectScreen_Cancel);
                }
            }

            /**
             * @brief 選択中ステージ名を UI に反映する
             */
            if (pSelectUI_)
                pSelectUI_->SetStageName(dataList[currentIndex_].name);

            float angleStep = 360.0f / icons_.size();       //! 1 アイコンあたりの角度
            float targetAngle = -currentIndex_ * angleStep; //! 目標回転角
            targetAngle_ = targetAngle;
            currentAngle_ += (targetAngle_ - currentAngle_) * 0.1f;

            /**
             * @brief 各アイコンを円周上に配置する
             */
            for (int i = 0; i < (int) icons_.size(); i++)
            {
                float deg = currentAngle_ + (i * angleStep); //! このアイコンの角度（度）
                float rad = Math::DegToRad(deg);             //! ラジアン変換

                Vector3 pos; //! 配置座標
                pos.x = sinf(rad) * RADIUS;
                pos.y = 0.0f;
                pos.z = cosf(rad) * RADIUS;

                icons_[i]->SetPosition(pos);
            }

            /**
             * @brief フェードアウト完了後にシーン遷移する
             */
            if (pFade_ && pFade_->IsFadeOutEnd())
            {
                nsCore::SoundManager::GetInstance()->StopBGM(GameSoundList_BGM_StageSelect);

                if (isDecided_)
                {
                    SceneManager::GetInstance()->ShowLoading();
                    StageID selectID = dataList[currentIndex_].id; //! 選択されたステージ ID
                    nsStage::StageManager::SetNextInitStageID(selectID);
                    SceneManager::GetInstance()->ChangeScene(nsScene::SceneID::sInGame);
                }
                else if (isButtonB_)
                {
                    SceneManager::GetInstance()->ChangeScene(nsScene::SceneID::sTitle);
                }
            }
        }

        void WorldSelectScene::CreateSkyCube()
        {
            /**
             * @brief 夜空 SkyCube を背景表示専用で生成する
             */
            pSkyCube_ = NewGO<SkyCube>(0, "skycube");
            pSkyCube_->SetScale(Vector3::One * 100.0f);
            pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Night);
        }
    } // namespace nsScene
} // namespace nsApp
