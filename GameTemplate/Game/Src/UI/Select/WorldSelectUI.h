#pragma once

namespace app
{
    namespace nsUI
    {
        class WorldSelectUI : public IGameObject
        {
        public:
            WorldSelectUI() = default;
            virtual ~WorldSelectUI() = default;


        public:
            void Init();
            void Update() override;
            void Render(RenderContext& rc) override;


        public:
            // ステージ名を設定する関数
            void SetStageName(const wchar_t* name);


        private:
            // 選択中のステージ名を左上に描画。
            void DrawStageName();

            // 「StageSelect」と表記。
            void DrawStageSelectText();

            // スティック操作UIを描画。
            void DrawStickUI();

            // 「Select」と表記。
            void DrawSelectText();

            // BボタンUIを描画。
            void DrawBackButtonUI();

            // 「Back」と表記。
            void DrawBackText();

        private:
            // 左上のステージ名表示用
            FontRender stageNameText_;

            // 上部に「WorldSelect」と描画する用。
            FontRender worldSelectTextUI_;

            // 左下にスティック操作用。
            SpriteRender stickUIRender_;

            // 「Select」と表記する用。
            FontRender selectTextUI_;

            // 右下にバックボタン用。
            SpriteRender backButtonRender_;

            // バックテキスト用。
            FontRender backUIText_;
        };

    }
}
