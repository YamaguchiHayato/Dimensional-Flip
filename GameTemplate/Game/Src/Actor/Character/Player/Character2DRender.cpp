#include "stdafx.h"

//
// Character2DRender.cpp
// 板ポリのようなモデルで画像を切り替えてアニメーションできるモデルレンダー
// TODO:UIアニメーションさせたいけど、一旦ModelRenderを切り替えてやる
//

#include "Character2DRender.h"


Character2DRender::Character2DRender() : m_currentIndex(0)
{
    m_modelRenderList.clear();
}

Character2DRender::~Character2DRender()
{
    // スマートポインタなので自動解放される
    m_modelRenderList.clear();
}

void Character2DRender::Init(const std::vector<std::string>& assetPathList)
{
    // モデルレンダーを複数初期化する
    for (const auto& assetPath : assetPathList)
    {
        auto modelRender = std::make_unique<ModelRender>();
        modelRender->SetZPrepassFilePath("Assets/shader/preProcess/CharacterZPrepass.fx");
        modelRender->Init(assetPath.c_str());
        m_modelRenderList.push_back(std::move(modelRender));
    }
}

void Character2DRender::Update()
{
    // すべてのモデルレンダーを更新する
    for (const auto& modelRender : m_modelRenderList)
    {
        modelRender->SetPosition(m_position);
        modelRender->SetScale(m_scale);
        modelRender->SetRotation(m_rotation);
        modelRender->Update();
    }
}

void Character2DRender::Render(RenderContext& rc)
{
    // 現在のインデックスのモデルレンダーだけ描画する
    if (m_currentIndex >= 0 && m_currentIndex < m_modelRenderList.size())
        m_modelRenderList[m_currentIndex]->Draw(rc);
}
