#include "stdafx.h"

#include "PatchNoteListUI.h"
#include "Src/Core/SoundManager.h"

namespace
{
    //! Note 左端に隙間を空けてバージョンリストを配置する（画面座標）
    constexpr float kNoteCenterX = 440.0f; //!< PatchNoteWindowUI の NOTE_POS.x と合わせる。
    constexpr float kNoteHalfW = 512.0f;   //!< note.DDS 表示幅の半分（1024 / 2）。
    constexpr float kBarHalfW = 180.0f;    //!< select/unselect バー幅の半分（360 / 2）。
    constexpr float kGapFromNote = 32.0f;  //!< Note 左端とバー右端の隙間。
    constexpr float kBarScale = 1.8f;      //!< バー拡大率。

    //! バー中心 X（スケール後の半幅を考慮）
    const float kPosX = kNoteCenterX - kNoteHalfW - kGapFromNote - kBarHalfW * kBarScale;

    const float kStartY = 470.0f; //!< 先頭 ver（最新）の Y。大きいほど上。
    const float kStepY = 150.0f;  //!< ver 行と行の中心間隔（行間）。

    //! スクロール表示領域（画面 Y）。範囲外の行は非表示にする
    constexpr float kViewTopY = 500.0f;   //!< 表示上限（これより上は見せない）。
    constexpr float kViewBottomY = 80.0f; //!< 表示下限（これより下は見せない）。
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void PatchNoteListUI::Init(UITransform* parent)
        {
            parent_ = parent;

            //! 全スロットを初期化（実際の表示数は SetEntries で決まる）
            for (int i = 0; i < MAX_ITEMS; ++i)
            {
                items_[i].Init(parent_, {kPosX, kStartY, 0.0f}, kBarScale);
                items_[i].SetVisible(false);
            }
        }

        void PatchNoteListUI::SetEntries(const std::vector<PatchNoteEntry>& entries)
        {
            itemCount_ = (int) entries.size();
            if (itemCount_ > MAX_ITEMS)
                itemCount_ = MAX_ITEMS;

            //! 初期選択は常に先頭（最新 ver）
            selectedIndex_ = 0;
            scrollOffsetY_ = 0.0f;

            for (int i = 0; i < MAX_ITEMS; ++i)
            {
                if (i < itemCount_)
                    items_[i].SetEntry(entries[(size_t) i]);
            }

            RefreshSelection();
            LayoutItems();
        }

        void PatchNoteListUI::LayoutItems()
        {
            //! バー高さ 56 * scale の半分。表示範囲判定用
            const float barHalfHeight = 28.0f * kBarScale;

            for (int i = 0; i < itemCount_; ++i)
            {
                //! スクロール量を加味した各行の Y
                const float y = kStartY - i * kStepY + scrollOffsetY_;
                items_[i].SetBarPosition({kPosX, y, 0.0f});

                //! 表示領域と重なる行だけ描画する
                const bool inView = (y + barHalfHeight >= kViewBottomY) && (y - barHalfHeight <= kViewTopY);
                items_[i].SetVisible(isVisible_ && inView);
            }

            //! 未使用スロットは非表示
            for (int i = itemCount_; i < MAX_ITEMS; ++i)
                items_[i].SetVisible(false);
        }

        void PatchNoteListUI::ClampScrollToSelection()
        {
            if (itemCount_ <= 0)
                return;

            const float selY = kStartY - selectedIndex_ * kStepY + scrollOffsetY_;

            //! 選択行が下にはみ出したらリストを上へ送る
            if (selY < kViewBottomY)
                scrollOffsetY_ += (kViewBottomY - selY);

            //! 選択行が上にはみ出したらリストを下へ戻す
            if (selY > kViewTopY)
                scrollOffsetY_ -= (selY - kViewTopY);

            //! 先頭 ver より上へスクロールしない
            const float scrollMin = 0.0f;

            //! 末尾 ver が下限に届くまでが最大スクロール
            float scrollMax = kViewBottomY - (kStartY - (itemCount_ - 1) * kStepY);
            if (scrollMax < 0.0f)
                scrollMax = 0.0f;

            if (scrollOffsetY_ < scrollMin)
                scrollOffsetY_ = scrollMin;
            if (scrollOffsetY_ > scrollMax)
                scrollOffsetY_ = scrollMax;
        }

        void PatchNoteListUI::Update()
        {
            if (!isVisible_)
                return;

            for (int i = 0; i < itemCount_; ++i)
                items_[i].Update();
        }

        void PatchNoteListUI::Draw(RenderContext& rc)
        {
            if (!isVisible_)
                return;

            for (int i = 0; i < itemCount_; ++i)
                items_[i].Draw(rc);
        }

        void PatchNoteListUI::SetVisible(bool visible)
        {
            isVisible_ = visible;
            LayoutItems();
        }

        bool PatchNoteListUI::UpdateInput(bool isUp, bool isDown)
        {
            if (!isVisible_ || itemCount_ <= 0)
                return false;

            if (!isUp && !isDown)
                return false;

            if (isUp)
            {
                selectedIndex_--;
                if (selectedIndex_ < 0)
                    selectedIndex_ = itemCount_ - 1;
            }
            else if (isDown) //!< if → else if に変更
            {
                selectedIndex_++;
                if (selectedIndex_ >= itemCount_)
                    selectedIndex_ = 0;
            }
            nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 2.0f);

            RefreshSelection();
            ClampScrollToSelection();
            LayoutItems();
            return true;
        }

        void PatchNoteListUI::RefreshSelection()
        {
            for (int i = 0; i < itemCount_; ++i)
                items_[i].SetSelected(i == selectedIndex_);
        }
    } // namespace nsUI
} // namespace nsApp
