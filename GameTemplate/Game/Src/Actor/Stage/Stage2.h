#pragma once
#include "Src/Actor/Stage/IStage.h"

namespace
{
    const Vector3 StartPos(0.0f, 101.0f, 0.0f);
}

class RotationFool;
class Stage2 : public IStage
{
public:
	Stage2() {};
	virtual ~Stage2();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
    // モデルのファイルパスをセット。
    inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};
// ゲッター。
public:
    inline Vector3 GetStageStartPos() const override
    {
        return StartPos;
    }

// ギミック生成。
private:
    inline void RotationFoolNewGO(); // 回転トリック。

private:
    // ギミック。
    RotationFool* pRotationFool_ = nullptr;

private:
    Vector3 scale_ = Vector3::One;
};

