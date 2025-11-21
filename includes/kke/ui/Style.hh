#pragma once

#include "../common/Color.hh"
#include <optional>

namespace kke::ui {

// シャドウの設定
struct Shadow {
    float blur;
    Color4f color;
    float offsetX;
    float offsetY;

    Shadow(float blur, Color4f color, float offsetX = 0.0f, float offsetY = 0.0f)
        : blur(blur), color(color), offsetX(offsetX), offsetY(offsetY) {}
};

// プリセットシャドウ
namespace shadows {
    inline Shadow sm() {
        return Shadow(2.0f, Color4f(0.0f, 0.0f, 0.0f, 0.1f), 0.0f, 1.0f);
    }

    inline Shadow md() {
        return Shadow(4.0f, Color4f(0.0f, 0.0f, 0.0f, 0.15f), 0.0f, 2.0f);
    }

    inline Shadow lg() {
        return Shadow(8.0f, Color4f(0.0f, 0.0f, 0.0f, 0.2f), 0.0f, 4.0f);
    }

    inline Shadow xl() {
        return Shadow(16.0f, Color4f(0.0f, 0.0f, 0.0f, 0.25f), 0.0f, 8.0f);
    }
}

// テキストサイズのプリセット（Tailwind風）
namespace text_sizes {
    constexpr float xs = 12.0f;
    constexpr float sm = 14.0f;
    constexpr float base = 16.0f;
    constexpr float lg = 18.0f;
    constexpr float xl = 20.0f;
    constexpr float xl2 = 24.0f;
    constexpr float xl3 = 30.0f;
    constexpr float xl4 = 36.0f;
    constexpr float xl5 = 48.0f;
    constexpr float xl6 = 60.0f;
}

// 間隔のプリセット（Tailwind風）
namespace spacing {
    constexpr float s0 = 0.0f;
    constexpr float s1 = 4.0f;
    constexpr float s2 = 8.0f;
    constexpr float s3 = 12.0f;
    constexpr float s4 = 16.0f;
    constexpr float s5 = 20.0f;
    constexpr float s6 = 24.0f;
    constexpr float s8 = 32.0f;
    constexpr float s10 = 40.0f;
    constexpr float s12 = 48.0f;
    constexpr float s16 = 64.0f;
    constexpr float s20 = 80.0f;
    constexpr float s24 = 96.0f;
}

// ボーダー幅のプリセット
namespace border_widths {
    constexpr float none = 0.0f;
    constexpr float thin = 1.0f;
    constexpr float medium = 2.0f;
    constexpr float thick = 4.0f;
}

// 角丸のプリセット
namespace rounded {
    constexpr float none = 0.0f;
    constexpr float sm = 2.0f;
    constexpr float md = 4.0f;
    constexpr float lg = 8.0f;
    constexpr float xl = 12.0f;
    constexpr float full = 9999.0f;  // 完全な円形
}

}  // namespace kke::ui
