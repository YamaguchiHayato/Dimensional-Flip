#pragma once

class Actor : public IGameObject
{
protected:
	Actor() = default;
	virtual ~Actor() = default;


protected:
    // 初期化処理。
	virtual bool Start() override
    {
        return true;
    }
    // 更新処理。
	virtual void Update() override
    {

    }
    // 描画処理。
	virtual void Render(RenderContext& rc) override
    {

    }
};
