#pragma once

enum class enGamePadSelect : uint8_t
{
    enSelect_AButton, /// Aボタン。
    enSelect_BButton, /// Bボタン。
    enSelect_XButton, /// Xボタン。
    enSelect_YButton, /// Yボタン。
    enSelect_LStick,  /// Lボタン。
    enSelect_RStick,  /// Rボタン。
    enSelect_DPad,    /// 十字キー。
    enSelect_Num,     /// ゲームパッド数。
};

enum class enTitleScene : uint8_t
{
    enTitle,      /// タイトル画面。
    enModeSelect, /// モード選択画面。
    enHowToPlay,  /// 遊び方説明画面。
    enNum         /// シーン数。
};

enum class enTitleModeSelect : uint8_t
{
    enTitleTransition_Title,      /// タイトル。
    enTitleTransition_ModeSelect, /// モード選択。
    enTitleTransition_HowToPlay,  /// 遊び方。
    enTitleTransition_Num         /// 遷移数。
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
