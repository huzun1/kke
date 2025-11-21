#pragma once

#include "../common/Color.hh"
#include <cstdint>

namespace kke::ui {

// 16進数から色を作成（RGB）
inline Color4f rgb(uint32_t hex) {
    return Color4f(
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f,
        1.0f  // 完全不透明
    );
}

// 16進数から色を作成（RGBA）
inline Color4f rgba(uint32_t hex, float alpha) {
    return Color4f(
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >> 8) & 0xFF) / 255.0f,
        (hex & 0xFF) / 255.0f,
        alpha
    );
}

// ピクセル単位（現在は単純に値を返す）
inline float px(float value) {
    return value;
}

// パーセンテージ（将来的に対応する可能性があるため定義）
inline float percent(float value) {
    return value;  // 今のところピクセルと同じ
}

// 色のプリセット
namespace colors {
    inline Color4f red() {
        return rgb(0xFF0000);
    }

    inline Color4f green() {
        return rgb(0x00FF00);
    }

    inline Color4f blue() {
        return rgb(0x0000FF);
    }

    inline Color4f yellow() {
        return rgb(0xFFFF00);
    }

    inline Color4f cyan() {
        return rgb(0x00FFFF);
    }

    inline Color4f magenta() {
        return rgb(0xFF00FF);
    }

    inline Color4f black() {
        return rgb(0x000000);
    }

    inline Color4f white() {
        return rgb(0xFFFFFF);
    }

    inline Color4f gray() {
        return rgb(0x808080);
    }

    inline Color4f transparent() {
        return rgba(0x000000, 0.0f);
    }
}

// グローバル名前空間にエイリアスをエクスポート（簡潔に書くため）
using colors::red;
using colors::green;
using colors::blue;
using colors::yellow;
using colors::cyan;
using colors::magenta;
using colors::black;
using colors::white;
using colors::gray;
using colors::transparent;

}  // namespace kke::ui
