#pragma once

namespace app
{
    namespace nsUI
    {
        class ManualUI : public IGameObject
        {
        public:
            ManualUI() = default;
            virtual ~ManualUI() = default;


        public:
            void Init();
            void Update();
            void Render(RenderContext& rc);


        private:
            // マニュアルテキストの初期化。
            void InitManualText();

            // BackボタンのUIの初期化。
            void InitBackButton();

            // 「操作説明」と表記。
            void InitTitleText();

            // 移動の操作説明。
            void InitMoveText();

            // カメラアクションの操作説明。
            void InitCameraActionText();

            // ジャンプの操作説明。
            void InitJumpText();

            // 「Back」と表記
            void InitBackText();

        // セッター。
        public:
            // UIを開く。
            inline void Open()
            {
                isActive_ = true;
            }

            // UIを閉じる。
            inline void Close()
            {
                isActive_ = false;
            }


        // ゲッター。
        public:
            // UIがアクティブか。
            inline bool IsActive() const
            {
                return isActive_;
            }


        private:
            // コントローラーの画像。
            SpriteRender manualSprite_;

            // BボタンUIの画像。
            SpriteRender buttonActionUI_;

            // 「Back」と表記。
            FontRender backTextFont_;

            // 「操作説明」と表記
            FontRender titleFont_;

            // Aボタンの操作説明
            FontRender jumpTextFont_;

            // Bボタンの操作説明
            FontRender cameraActionTextFont_;

            // 十字キーの操作説明
            FontRender moveTextFont_;

            bool isActive_ = false;
        };

    }
}

