#pragma once
/**
 * 板ポリのようなモデルで画像を切り替えてアニメーションできるモデルレンダー
 * TODO: UIアニメーションさせたいけど、一旦ModelRenderを切り替えてやる
 *
 * コピー禁止
 */
#include <vector>
#include <memory>
#include <string>
#include "Src/Collision/NonCopyable.h"

class Character2DRender:public Noncopyable
{
private:
    /** 描画するモデルレンダーのリスト(UVアニメーションになったら1個になる) */
    std::vector<std::unique_ptr<ModelRender>> m_modelRenderList;
    /** 現在描画に使っているModelRender(UVアニメーションになったらいらない) */
    int m_currentIndex;

    /** 座標 */
    Vector3 m_position = Vector3::Zero;
    /** 拡縮 */
    Vector3 m_scale = Vector3::One;
    /** 回転 */
    Quaternion m_rotation = Quaternion::Identity;

public:
    Character2DRender();
    ~Character2DRender();

    void Init(const std::vector<std::string>& assetPathList);
    void Update();
    void Render(RenderContext& rc);

public:
    /** 座標取得 */
    const Vector3& GetPosition() const {return m_position; }
    /** 座標設定 */
    void SetPosition(const Vector3& position) { m_position = position; }

    /** 拡縮取得 */
    const Vector3& GetScale() const {return m_scale; }
    /** 拡縮設定 */
    void SetScale(const Vector3& scale) { m_scale = scale; }

    /** 回転取得 */
    const Quaternion& GetRotation() const {return m_rotation; }
    /** 回転設定 */
    void SetRotation(const Quaternion& rotation) { m_rotation = rotation; }

    /** 描画対象を設定 */
    void SetCurrentIndex(int index) { m_currentIndex = index; }
};
