#include "stdafx.h"
#include "EndRollText.h"
#include <memory>

namespace
{
    const float SCROLL_SPEED = 50.0f;     // 上がる速さ
    const float TEXT_X_POS = -200.0f;      // 表示するX座標 
    const float START_Y_OFFSET = -540.0f; // 画面下端の座標

    // 表示するテキストを定義する。
    struct Def
    {
        const wchar_t* text; // 表示するテキスト。
        float scale;         // 大きさ。
        float margin;        // 余白。
    };

    const wchar_t* MY_NAME = L"Hayato Yamaguchi"; // 自分の名前。
}

namespace app
{
    namespace production
    {
        bool EndRollText::Start()
        {
            // テキストの管理。
            ManageText();

            return true;
        }


        bool EndRollText::ManageText()
        {
            std::vector<Def> data =
            {
                // タイトル名を表記。
                {L"Dimension Flip", 2.5f, 150.0f},

                // スタッフセクション。
                {L"---Staff---", 2.5f, 40.0f},

                // 企画。
                {L"Planning", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // メインプログラマ。
                {L"Main Programmer", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // サブプログラマ。
                {L"Sub Programmer", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // モデラ―。
                {L"Modeling", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // レベルデザイナー。
                {L"Level Designer", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // UI/UXデザイン。
                {L"UI/UX Designer", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // 品質保証。
                {L"Quality Assurance", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 100.0f},

                // キャスト。
                {L"---Cast---", 1.2f, 40.0f},

                // 主人公。
                {L"Girl", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 80.0f},

                // 敵その1。
                {L"Enemy 1", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 80.0f},

                // 敵その2。
                {L"Enemy 2", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 80.0f},

                // Boss。
                {L"Boss", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 80.0f},

                // スペシャルサンクス。
                {L"Special Thanks", 1.0f, 40.0f},
                {MY_NAME, 1.5f, 80.0f},

                // 代表挨拶。
                {L"Presented by", 1.0f, 50.0f},
                {MY_NAME, 2.0f, 200.0f},
                {L"Thank You For Playing !", 2.5f, 0.0f},
            };

            // ここの設定値を基準に文字を下に並べていく。
            auto currentYPos = 0.0f;

            for (const auto& d : data)
            {
                auto line  =std::make_unique<StaffRollLine>();

                // フォントを設定。
                // テキストをセット。
                line->staffRollText_.SetText(d.text);

                // 大きさをセット。
                line->staffRollText_.SetScale(d.scale);

                // ベースカラーの設定。
                line->staffRollText_.SetColor(Vector4::White);

                // 影の濃さを設定。
                line->staffRollText_.SetShadowParam(true, 3.0f, Vector4::Black);

                // 中心地をセット。
                line->staffRollText_.SetPivot({0.5f, 1.0f});

                // 初期位置をセットする。
                line->initializePos_ = Vector3(TEXT_X_POS, currentYPos, 0.0f);
                line->staffRollText_.SetPosition(line->initializePos_);

                // リストに登録。
                lines_.push_back(std::move(line));

                // 次の行の座標計算を行う。
                auto lineHeight = 40.0f * d.scale;
                currentYPos -= (lineHeight + d.margin);
            }

            // スクロールし始める初期位置を設定する。
            scrollSpeed_ = START_Y_OFFSET;

            return true;
        }


        void EndRollText::Update()
        {
            if (isEnd_)
                return;

            // スクロール管理。
            scrollSpeed_ += SCROLL_SPEED * g_gameTime->GetFrameDeltaTime();

            // 終了判定。
            if (!lines_.empty())
            {
                // 最後の行の現在位置をチェックする。
                const auto& lastLine = lines_.back();
                auto currentLastY = lastLine->initializePos_.y + scrollSpeed_;

                // 最後の行が画面上端を超えたら終了する。
                if (currentLastY > 600.0f)
                    isEnd_ = true;

                // 座標更新。
                for (auto& line : lines_)
                {
                    Vector3 positon = line->initializePos_;
                    positon.y += scrollSpeed_;
                    line->staffRollText_.SetPosition(positon);
                }
            }
        }


        void EndRollText::Render(RenderContext& rc)
        {
            for (auto& line : lines_)
            {
                // 画面外にカリングする。
                auto y = line->staffRollText_.GetPosition().y;
                if (y > -650.0f && y < 650.0f)
                {
                    line->staffRollText_.Draw(rc);
                }
            }
        }
    }
}
