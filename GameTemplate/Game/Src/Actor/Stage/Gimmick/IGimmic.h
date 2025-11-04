#pragma once
#include "Src/Actor/Actor.h"
class IGimmic : public Actor
{
public:
    IGimmic() {};
    virtual ~IGimmic() {};
    virtual bool Start() override { return true; }
    virtual void Update() override {};
    virtual void Render(RenderContext& rc) override {}

    virtual const std::string InitGimmick(const std::string& gimmickname)
    {
        std::string gimmickpath = "Assets/stage/" + gimmickname + ".tkm";
        return gimmickpath;
    };
};
