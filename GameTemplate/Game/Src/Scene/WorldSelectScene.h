#pragma once
#include <vector>

#include "Src/Scene/Scene.h"
#include "StageIcon.h"

namespace app {
    namespace nsUI {
        class WorldSelectUI;
    }
}


class Fade;

class WorldSelectScene : public IScene
{
public:
    WorldSelectScene() = default;
    virtual ~WorldSelectScene();


public:
    bool Start() override;
    void Update() override;


private:
    void CreateSkyCube();


private:
    Fade* pFade_ = nullptr;
    app::nsUI::WorldSelectUI* pSelectUI_ = nullptr;
    SkyCube* pSkyCube_ = nullptr;

private:
    // アイコンたちを管理するリスト
    std::vector<app::nsUI::StageIcon*> icons_;

    // 今どれを選んでいるか
    int currentIndex_ = 0;

    // 回転演出用の角度
    float currentAngle_ = 0.0f;
    float targetAngle_ = 0.0f;

    bool isDecided_ = false;
    bool isButtonB_ = false;

    bool isRight_ = false;
    bool isLeft_ = false;
};
