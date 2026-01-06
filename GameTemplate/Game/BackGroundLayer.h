#pragma once


// シェーダー(b1)に送る定数バッファ（sprite.fx の SpriteRenderCb と一致）
struct Float2
{
    float x, y;
};


struct CBScrollData
{
    // LinearWipe
    Vector2 linearWipeDirection = {0.0f, 0.0f}; // float2
    float linearWipeSize = 0.0f;                // float
    float _pad0 = 0.0f;                         // 16byte揃え

    // wipe/effect mode
    int linearWipeDrawingMode = -1; // int（-1でswitch default→clipしない）
    float drawingRate = 0.0f;       // float
    int screenDrawingMode = -1;     // int（-1でswitch default→加工しない）
    float _pad1 = 0.0f;             // 16byte揃え

    // UV scroll
    Vector2 uvOffset = {0.0f, 0.0f}; // float2
    Vector2 _pad2 = {0.0f, 0.0f};    // 16byte揃え
};

static_assert(sizeof(CBScrollData) == 48, "CBScrollData is NOT matching sprite.fx(b1) !");


namespace app
{
    namespace stage
    {
        class BackGroundLayer
        {
        public:
            BackGroundLayer();
            virtual ~BackGroundLayer();


        public:
            // 初期化。
            // filePath: 背景画像のファイルパス。
            // zDepth: Z座標。
            // scrollRaito: スクロール速度の割合。
            void Init(const char* filePath, float zDepth, float scrollRaito);


            // 更新。
            void Update(const Vector3& cameraPos);


            // 描画。
            void Draw(RenderContext& rc, nsK2EngineLow::Camera* pCamera);


        private:
            Sprite* pSprite_ = nullptr; // 背景スプライト。
            CBScrollData cbData_;

            float zDepth_ = 0.0f;       // Z座標。
            float scrollRaito_ = 1.0f;  // スクロール速度の割合。
        };

    }
}

