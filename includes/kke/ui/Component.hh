#pragma once

#include "../common/Color.hh"
#include "../common/Point.hh"
#include "../common/geometry/Rect.hh"
#include "../common/geometry/RoundedRect.hh"
#include "../Engine.hh"
#include "../brush/SolidColorBrush.hh"
#include "../font/FontWeight.hh"
#include "Style.hh"
#include <yoga/Yoga.h>
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace kke::ui {

// コンポーネントクラス - UIツリーのノードを表現
class Component : public std::enable_shared_from_this<Component> {
protected:
    YGNodeRef yogaNode_;
    std::vector<std::shared_ptr<Component>> children_;

    // スタイルプロパティ
    std::optional<Color4f> backgroundColor_;
    std::optional<Color4f> textColor_;
    std::optional<Color4f> borderColor_;
    std::optional<float> borderWidth_;
    std::optional<float> borderRadius_;
    std::optional<Shadow> shadow_;
    std::optional<float> textSize_;
    std::optional<std::wstring> textContent_;
    std::optional<std::wstring> fontFamily_;
    std::optional<FontWeight> fontWeight_;

public:
    Component() {
        yogaNode_ = YGNodeNew();
        // デフォルト設定
        YGNodeStyleSetFlexDirection(yogaNode_, YGFlexDirectionRow);
    }

    virtual ~Component() {
        if (yogaNode_) {
            YGNodeFree(yogaNode_);
            yogaNode_ = nullptr;
        }
    }

    // コピー禁止
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    // Yogaノードへのアクセス
    YGNodeRef getYogaNode() const { return yogaNode_; }

    // 子要素の追加
    void addChild(std::shared_ptr<Component> child) {
        children_.push_back(child);
        YGNodeInsertChild(yogaNode_, child->yogaNode_, YGNodeGetChildCount(yogaNode_));
    }

    // テキストサイズを測定してレイアウトに反映
    void measureTextIfNeeded(Engine* engine) {
        // テキストコンテンツがあり、幅または高さが未設定の場合は測定
        if (textContent_.has_value() && !textContent_.value().empty()) {
            float fontSize = textSize_.value_or(16.0f);
            FontWeight fontWeight = fontWeight_.value_or(FontWeight::NORMAL);
            std::wstring fontFamily = fontFamily_.value_or(L"Arial");

            // テキストサイズを測定
            auto textSize = engine->getTextSize(
                textContent_.value(),
                fontWeight,
                fontFamily,
                static_cast<int>(fontSize)
            );

            // 幅が設定されていない場合は、テキストサイズ + パディング
            if (YGFloatIsUndefined(YGNodeStyleGetWidth(yogaNode_).value)) {
                float paddingLeft = YGNodeStyleGetPadding(yogaNode_, YGEdgeLeft).value;
                float paddingRight = YGNodeStyleGetPadding(yogaNode_, YGEdgeRight).value;
                if (YGFloatIsUndefined(paddingLeft)) paddingLeft = 0.0f;
                if (YGFloatIsUndefined(paddingRight)) paddingRight = 0.0f;

                // テキストサイズ + 実際のパディング（レンダリング時の余白は含めない）
                float totalWidth = textSize.x + paddingLeft + paddingRight;
                YGNodeStyleSetWidth(yogaNode_, totalWidth);
            }

            // 高さが設定されていない場合は、テキストサイズ + パディング
            if (YGFloatIsUndefined(YGNodeStyleGetHeight(yogaNode_).value)) {
                float paddingTop = YGNodeStyleGetPadding(yogaNode_, YGEdgeTop).value;
                float paddingBottom = YGNodeStyleGetPadding(yogaNode_, YGEdgeBottom).value;
                if (YGFloatIsUndefined(paddingTop)) paddingTop = 0.0f;
                if (YGFloatIsUndefined(paddingBottom)) paddingBottom = 0.0f;

                // テキストサイズ + 実際のパディング（レンダリング時の余白は含めない）
                float totalHeight = textSize.y + paddingTop + paddingBottom;
                YGNodeStyleSetHeight(yogaNode_, totalHeight);
            }
        }

        // 子要素も再帰的に測定
        for (auto& child : children_) {
            child->measureTextIfNeeded(engine);
        }
    }

    // レイアウト計算
    void calculateLayout(float availableWidth = YGUndefined, float availableHeight = YGUndefined) {
        YGNodeCalculateLayout(yogaNode_, availableWidth, availableHeight, YGDirectionLTR);
    }

    // レイアウト結果の取得
    Rect getRect() const {
        float left = YGNodeLayoutGetLeft(yogaNode_);
        float top = YGNodeLayoutGetTop(yogaNode_);
        float width = YGNodeLayoutGetWidth(yogaNode_);
        float height = YGNodeLayoutGetHeight(yogaNode_);
        return Rect{left, top, left + width, top + height};
    }

    float getLayoutX() const { return YGNodeLayoutGetLeft(yogaNode_); }
    float getLayoutY() const { return YGNodeLayoutGetTop(yogaNode_); }
    float getLayoutWidth() const { return YGNodeLayoutGetWidth(yogaNode_); }
    float getLayoutHeight() const { return YGNodeLayoutGetHeight(yogaNode_); }

    // スタイルプロパティのセッター
    void setBackgroundColor(const Color4f& color) { backgroundColor_ = color; }
    void setTextColor(const Color4f& color) { textColor_ = color; }
    void setBorderColor(const Color4f& color) { borderColor_ = color; }
    void setBorderWidth(float width) { borderWidth_ = width; }
    void setBorderRadius(float radius) { borderRadius_ = radius; }
    void setShadow(const Shadow& shadow) { shadow_ = shadow; }
    void setTextSize(float size) { textSize_ = size; }
    void setTextContent(const std::wstring& text) { textContent_ = text; }
    void setFontFamily(const std::wstring& family) { fontFamily_ = family; }
    void setFontWeight(FontWeight weight) { fontWeight_ = weight; }

    // スタイルプロパティのゲッター
    const std::optional<Color4f>& getBackgroundColor() const { return backgroundColor_; }
    const std::optional<Color4f>& getTextColor() const { return textColor_; }
    const std::optional<Color4f>& getBorderColor() const { return borderColor_; }
    const std::optional<float>& getBorderWidth() const { return borderWidth_; }
    const std::optional<float>& getBorderRadius() const { return borderRadius_; }
    const std::optional<Shadow>& getShadow() const { return shadow_; }
    const std::optional<float>& getTextSize() const { return textSize_; }
    const std::optional<std::wstring>& getTextContent() const { return textContent_; }
    const std::optional<std::wstring>& getFontFamily() const { return fontFamily_; }
    const std::optional<FontWeight>& getFontWeight() const { return fontWeight_; }

    // レンダリング（再帰的）
    void render(Engine* engine, float offsetX = 0.0f, float offsetY = 0.0f) {
        float x = offsetX + getLayoutX();
        float y = offsetY + getLayoutY();
        float width = getLayoutWidth();
        float height = getLayoutHeight();

        // サイズが0の場合はレンダリングをスキップ
        if (width <= 0.0f || height <= 0.0f) {
            return;
        }

        Rect rect{x, y, x + width, y + height};
        float radius = borderRadius_.value_or(0.0f);

        // 1. シャドウの描画（最背面）
        // 注意: 現在のEngine APIはシャドウオフセットをサポートしていません
        if (shadow_.has_value()) {
            const auto& s = shadow_.value();
            if (radius > 0.0f) {
                engine->drawRoundedShadow(
                    RoundedRect(rect, radius),
                    SolidColorBrush(s.color),
                    s.blur
                );
            } else {
                engine->drawRectShadow(
                    rect,
                    SolidColorBrush(s.color),
                    s.blur
                );
            }
        }

        // 2. 背景色の描画
        if (backgroundColor_.has_value()) {
            if (radius > 0.0f) {
                engine->fillRounded(
                    RoundedRect(rect, radius),
                    SolidColorBrush(backgroundColor_.value())
                );
            } else {
                engine->fillRect(rect, SolidColorBrush(backgroundColor_.value()));
            }
        }

        // 3. ボーダーの描画
        if (borderWidth_.has_value() && borderWidth_.value() > 0.0f && borderColor_.has_value()) {
            if (radius > 0.0f) {
                engine->drawRounded(
                    RoundedRect(rect, radius),
                    SolidColorBrush(borderColor_.value()),
                    borderWidth_.value()
                );
            } else {
                engine->drawRect(
                    rect,
                    SolidColorBrush(borderColor_.value()),
                    borderWidth_.value()
                );
            }
        }

        // 4. テキストの描画
        if (textContent_.has_value() && !textContent_.value().empty()) {
            Color4f textCol = textColor_.value_or(Color4f(0.0f, 0.0f, 0.0f, 1.0f));  // デフォルトは黒
            float fontSize = textSize_.value_or(16.0f);  // デフォルトは16px
            FontWeight fontWt = fontWeight_.value_or(FontWeight::NORMAL);
            std::wstring fontFam = fontFamily_.value_or(L"Arial");

            // パディングを考慮したテキスト位置
            float paddingLeft = YGNodeStyleGetPadding(yogaNode_, YGEdgeLeft).value;
            float paddingTop = YGNodeStyleGetPadding(yogaNode_, YGEdgeTop).value;
            if (YGFloatIsUndefined(paddingLeft)) paddingLeft = 0.0f;
            if (YGFloatIsUndefined(paddingTop)) paddingTop = 0.0f;

            Point2f textPos{x + paddingLeft, y + paddingTop};

            engine->drawText(
                textPos,
                textContent_.value(),
                fontWt,
                fontFam,
                static_cast<int>(fontSize),
                SolidColorBrush(textCol)
            );
        }

        // 5. 子要素の再帰的レンダリング
        for (auto& child : children_) {
            child->render(engine, x, y);
        }
    }

    // 子要素へのアクセス
    const std::vector<std::shared_ptr<Component>>& getChildren() const {
        return children_;
    }
};

}  // namespace kke::ui
