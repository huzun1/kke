#pragma once

#include "Component.hh"
#include "Style.hh"
#include "Utilities.hh"
#include <yoga/Yoga.h>
#include <memory>
#include <string>
#include <format>

namespace kke::ui {

// Divビルダークラス - Tailwind風のメソッドチェーンAPIを提供
class DivBuilder {
private:
    std::shared_ptr<Component> component_;

public:
    DivBuilder() {
        component_ = std::make_shared<Component>();
    }

    // ========== Flexboxレイアウト ==========

    // flex-direction: row
    DivBuilder& flex() {
        YGNodeStyleSetFlexDirection(component_->getYogaNode(), YGFlexDirectionRow);
        return *this;
    }

    // flex-direction: column
    DivBuilder& flex_col() {
        YGNodeStyleSetFlexDirection(component_->getYogaNode(), YGFlexDirectionColumn);
        return *this;
    }

    // flex-direction: row-reverse
    DivBuilder& flex_row_reverse() {
        YGNodeStyleSetFlexDirection(component_->getYogaNode(), YGFlexDirectionRowReverse);
        return *this;
    }

    // flex-direction: column-reverse
    DivBuilder& flex_col_reverse() {
        YGNodeStyleSetFlexDirection(component_->getYogaNode(), YGFlexDirectionColumnReverse);
        return *this;
    }

    // ========== Justify Content ==========

    DivBuilder& justify_start() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifyFlexStart);
        return *this;
    }

    DivBuilder& justify_center() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifyCenter);
        return *this;
    }

    DivBuilder& justify_end() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifyFlexEnd);
        return *this;
    }

    DivBuilder& justify_between() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifySpaceBetween);
        return *this;
    }

    DivBuilder& justify_around() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifySpaceAround);
        return *this;
    }

    DivBuilder& justify_evenly() {
        YGNodeStyleSetJustifyContent(component_->getYogaNode(), YGJustifySpaceEvenly);
        return *this;
    }

    // ========== Align Items ==========

    DivBuilder& items_start() {
        YGNodeStyleSetAlignItems(component_->getYogaNode(), YGAlignFlexStart);
        return *this;
    }

    DivBuilder& items_center() {
        YGNodeStyleSetAlignItems(component_->getYogaNode(), YGAlignCenter);
        return *this;
    }

    DivBuilder& items_end() {
        YGNodeStyleSetAlignItems(component_->getYogaNode(), YGAlignFlexEnd);
        return *this;
    }

    DivBuilder& items_stretch() {
        YGNodeStyleSetAlignItems(component_->getYogaNode(), YGAlignStretch);
        return *this;
    }

    DivBuilder& items_baseline() {
        YGNodeStyleSetAlignItems(component_->getYogaNode(), YGAlignBaseline);
        return *this;
    }

    // ========== Align Self ==========

    DivBuilder& self_start() {
        YGNodeStyleSetAlignSelf(component_->getYogaNode(), YGAlignFlexStart);
        return *this;
    }

    DivBuilder& self_center() {
        YGNodeStyleSetAlignSelf(component_->getYogaNode(), YGAlignCenter);
        return *this;
    }

    DivBuilder& self_end() {
        YGNodeStyleSetAlignSelf(component_->getYogaNode(), YGAlignFlexEnd);
        return *this;
    }

    DivBuilder& self_stretch() {
        YGNodeStyleSetAlignSelf(component_->getYogaNode(), YGAlignStretch);
        return *this;
    }

    // ========== サイズ指定 ==========

    // width/heightを同時設定
    DivBuilder& size(float value) {
        YGNodeStyleSetWidth(component_->getYogaNode(), value);
        YGNodeStyleSetHeight(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& width(float value) {
        YGNodeStyleSetWidth(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& height(float value) {
        YGNodeStyleSetHeight(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& min_width(float value) {
        YGNodeStyleSetMinWidth(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& min_height(float value) {
        YGNodeStyleSetMinHeight(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& max_width(float value) {
        YGNodeStyleSetMaxWidth(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& max_height(float value) {
        YGNodeStyleSetMaxHeight(component_->getYogaNode(), value);
        return *this;
    }

    // Tailwind風のサイズプリセット (size-8 = 32px)
    DivBuilder& size_8() { return size(32.0f); }
    DivBuilder& size_12() { return size(48.0f); }
    DivBuilder& size_16() { return size(64.0f); }
    DivBuilder& size_20() { return size(80.0f); }
    DivBuilder& size_24() { return size(96.0f); }

    // ========== スペーシング ==========

    DivBuilder& gap(float value) {
        YGNodeStyleSetGap(component_->getYogaNode(), YGGutterAll, value);
        return *this;
    }

    DivBuilder& gap_x(float value) {
        YGNodeStyleSetGap(component_->getYogaNode(), YGGutterColumn, value);
        return *this;
    }

    DivBuilder& gap_y(float value) {
        YGNodeStyleSetGap(component_->getYogaNode(), YGGutterRow, value);
        return *this;
    }

    // Tailwind風のgapプリセット
    DivBuilder& gap_1() { return gap(spacing::s1); }
    DivBuilder& gap_2() { return gap(spacing::s2); }
    DivBuilder& gap_3() { return gap(spacing::s3); }
    DivBuilder& gap_4() { return gap(spacing::s4); }
    DivBuilder& gap_5() { return gap(spacing::s5); }
    DivBuilder& gap_6() { return gap(spacing::s6); }
    DivBuilder& gap_8() { return gap(spacing::s8); }

    // パディング（全方向）
    DivBuilder& padding(float value) {
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeAll, value);
        return *this;
    }

    // パディング（個別）
    DivBuilder& padding(float top, float right, float bottom, float left) {
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeTop, top);
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeRight, right);
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeBottom, bottom);
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeLeft, left);
        return *this;
    }

    DivBuilder& padding_x(float value) {
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeLeft, value);
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeRight, value);
        return *this;
    }

    DivBuilder& padding_y(float value) {
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeTop, value);
        YGNodeStyleSetPadding(component_->getYogaNode(), YGEdgeBottom, value);
        return *this;
    }

    // マージン（全方向）
    DivBuilder& margin(float value) {
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeAll, value);
        return *this;
    }

    // マージン（個別）
    DivBuilder& margin(float top, float right, float bottom, float left) {
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeTop, top);
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeRight, right);
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeBottom, bottom);
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeLeft, left);
        return *this;
    }

    DivBuilder& margin_x(float value) {
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeLeft, value);
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeRight, value);
        return *this;
    }

    DivBuilder& margin_y(float value) {
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeTop, value);
        YGNodeStyleSetMargin(component_->getYogaNode(), YGEdgeBottom, value);
        return *this;
    }

    // ========== Flexプロパティ ==========

    DivBuilder& flex_grow(float value) {
        YGNodeStyleSetFlexGrow(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& flex_shrink(float value) {
        YGNodeStyleSetFlexShrink(component_->getYogaNode(), value);
        return *this;
    }

    DivBuilder& flex_basis(float value) {
        YGNodeStyleSetFlexBasis(component_->getYogaNode(), value);
        return *this;
    }

    // ========== スタイル ==========

    DivBuilder& bg(const Color4f& color) {
        component_->setBackgroundColor(color);
        return *this;
    }

    DivBuilder& text_color(const Color4f& color) {
        component_->setTextColor(color);
        return *this;
    }

    DivBuilder& text_size(float size) {
        component_->setTextSize(size);
        return *this;
    }

    // Tailwind風のテキストサイズプリセット
    DivBuilder& text_xs() { return text_size(text_sizes::xs); }
    DivBuilder& text_sm() { return text_size(text_sizes::sm); }
    DivBuilder& text_base() { return text_size(text_sizes::base); }
    DivBuilder& text_lg() { return text_size(text_sizes::lg); }
    DivBuilder& text_xl() { return text_size(text_sizes::xl); }
    DivBuilder& text_2xl() { return text_size(text_sizes::xl2); }
    DivBuilder& text_3xl() { return text_size(text_sizes::xl3); }

    DivBuilder& border(float width) {
        component_->setBorderWidth(width);
        return *this;
    }

    DivBuilder& border_1() { return border(border_widths::thin); }
    DivBuilder& border_2() { return border(border_widths::medium); }
    DivBuilder& border_4() { return border(border_widths::thick); }

    DivBuilder& border_color(const Color4f& color) {
        component_->setBorderColor(color);
        return *this;
    }

    DivBuilder& rounded(float radius) {
        component_->setBorderRadius(radius);
        return *this;
    }

    // Tailwind風の角丸プリセット
    DivBuilder& rounded_none() { return rounded(rounded::none); }
    DivBuilder& rounded_sm() { return rounded(rounded::sm); }
    DivBuilder& rounded_md() { return rounded(rounded::md); }
    DivBuilder& rounded_lg() { return rounded(rounded::lg); }
    DivBuilder& rounded_xl() { return rounded(rounded::xl); }
    DivBuilder& rounded_full() { return rounded(rounded::full); }

    // シャドウ
    DivBuilder& shadow(float blur, const Color4f& color, float offsetX = 0.0f, float offsetY = 0.0f) {
        component_->setShadow(Shadow(blur, color, offsetX, offsetY));
        return *this;
    }

    // Tailwind風のシャドウプリセット
    DivBuilder& shadow_sm() {
        component_->setShadow(shadows::sm());
        return *this;
    }

    DivBuilder& shadow() {
        component_->setShadow(shadows::md());
        return *this;
    }

    DivBuilder& shadow_lg() {
        component_->setShadow(shadows::lg());
        return *this;
    }

    DivBuilder& shadow_xl() {
        component_->setShadow(shadows::xl());
        return *this;
    }

    // フォント
    DivBuilder& font_family(const std::wstring& family) {
        component_->setFontFamily(family);
        return *this;
    }

    DivBuilder& font_weight(FontWeight weight) {
        component_->setFontWeight(weight);
        return *this;
    }

    DivBuilder& font_normal() { return font_weight(FontWeight::NORMAL); }
    DivBuilder& font_bold() { return font_weight(FontWeight::BOLD); }

    // ========== 子要素 ==========

    // 子要素追加（ビルダー）
    DivBuilder& child(DivBuilder childBuilder) {
        component_->addChild(childBuilder.build());
        return *this;
    }

    // 子要素追加（コンポーネント）
    DivBuilder& child(std::shared_ptr<Component> childComponent) {
        component_->addChild(childComponent);
        return *this;
    }

    // 子要素追加（テキスト - std::string）
    DivBuilder& child(const std::string& text) {
        // std::stringからstd::wstringに変換
        std::wstring wtext(text.begin(), text.end());
        component_->setTextContent(wtext);
        return *this;
    }

    // 子要素追加（テキスト - std::wstring）
    DivBuilder& child(const std::wstring& text) {
        component_->setTextContent(text);
        return *this;
    }

    // ========== ビルド ==========

    std::shared_ptr<Component> build() {
        return component_;
    }

    // 暗黙的な変換（オプション）
    operator std::shared_ptr<Component>() {
        return component_;
    }
};

// グローバル関数 - div()でビルダーを作成
inline DivBuilder div() {
    return DivBuilder();
}

}  // namespace kke::ui
