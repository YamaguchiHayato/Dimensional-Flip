#pragma once

enum class enGamePadSelect : uint8_t
{
    AButton, /// Aボタン。
    BButton, /// Bボタン。
    XButton, /// Xボタン。
    YButton, /// Yボタン。
    LStick,  /// Lボタン。
    RStick,  /// Rボタン。
    DPad,    /// 十字キー。
    Num,     /// ゲームパッド数。
};

enum class enTitleScene : uint8_t
{
    sTitle,      /// タイトル画面。
    sModeSelect, /// モード選択画面。
    sHowToPlay,  /// 遊び方説明画面。
    sNum         /// シーン数。
};

enum class enTitleModeSelect : uint8_t
{
    sTitle,      /// タイトル。
    sModeSelect, /// モード選択。
    sHowToPlay,  /// 遊び方。
    sNum         /// 遷移数。
};

class Fade;
class Title : public IGameObject
{
public:
    Title() {};
    ~Title() {};

    bool Start();
    void Update();
    void Render(RenderContext& rc);

public:
    inline const void SetGameLooadFlag(bool flag) { gameLoadFlag = flag; }

public:
    inline bool GetGameLoadFlag() { return gameLoadFlag; }

private:
    Fade* fade_ = nullptr;

private:
    SpriteRender titleRender_;

private:
    bool gameLoadFlag = false;
};
