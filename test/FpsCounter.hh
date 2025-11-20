#pragma once

#include <algorithm>  // For std::sort
#include <chrono>
#include <cstdint>
#include <deque>
#include <numeric>
#include <vector>

class FpsCounter {
public:
	using clock = std::chrono::steady_clock;
	using duration_float = std::chrono::duration<float>;

	// history_duration: 統計を計算する期間
	explicit FpsCounter(duration_float history_duration = std::chrono::seconds(5))
		: history_duration_(history_duration) {
		last_ = clock::now();  // 初期化
	}

	void frame() {
		const auto now = clock::now();
		const auto dt = duration_float(now - last_).count();
		last_ = now;

		if (dt > 0.0f) {
			float current_fps = 1.0f / dt;
			fps_history_.push_back({now, current_fps});
			fps_ = current_fps;	 // 最新のFPSを更新
		}

		// 期間外の古いエントリーを削除
		while (!fps_history_.empty() && fps_history_.front().first < now - history_duration_) {
			fps_history_.pop_front();
		}
	}

	float fps() const noexcept {
		return fps_;
	}

	// 平均FPSを計算
	float average_fps() const {
		if (fps_history_.empty()) {
			return 0.0f;
		}
		float sum = 0.0f;
		for (const auto& entry : fps_history_) {
			sum += entry.second;
		}
		return sum / fps_history_.size();
	}

	// 指定されたパーセンタイルのFPSを計算
	float percentile_fps(float p) const {
		if (fps_history_.empty()) {
			return 0.0f;
		}

		std::vector<float> values;
		values.reserve(fps_history_.size());
		for (const auto& entry : fps_history_) {
			values.push_back(entry.second);
		}
		std::sort(values.begin(), values.end());

		// パーセンタイルのインデックスを計算
		// pは0から100の範囲
		float index_float = (static_cast<float>(values.size()) - 1.0f) * (p / 100.0f);

		// 線形補間
		if (std::floor(index_float) == index_float) {
			// インデックスが整数値の場合
			return values[static_cast<size_t>(index_float)];
		} else {
			// インデックスが非整数値の場合、線形補間
			size_t lower_index = static_cast<size_t>(std::floor(index_float));
			size_t upper_index = static_cast<size_t>(std::ceil(index_float));
			float weight = index_float - static_cast<float>(lower_index);
			return values[lower_index] * (1.0f - weight) + values[upper_index] * weight;
		}
	}

	// 1パーセンタイルFPS（下位1%）を計算
	float p1_fps() const {
		return percentile_fps(1.0f);
	}

	// 99パーセンタイルFPS（上位1%）を計算
	float p99_fps() const {
		return percentile_fps(99.0f);
	}

private:
	clock::time_point last_;
	float fps_ = 0.0f;	// 最新のFPS

	// 過去のFPS値とその計測時刻を保存
	std::deque<std::pair<clock::time_point, float>> fps_history_;
	duration_float history_duration_;  // 統計を計算する期間
};
