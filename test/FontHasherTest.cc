#include <string>

#include "kke/appearance/Text.hh"
#include "kke/engine/d2d/resource/font/hash/FontHasher.hh"

int main() {
	kke::FontAppearance appearance{
		.fontFamily = "Inter",
		.fontSize = 14.0f,
	};
	kke::Text narrowText{
		.text = std::string("Array List"),
		.position = {0.0f, 0.0f},
		.fontAppearance = appearance,
	};
	kke::Text wideText{
		.text = std::wstring(L"Array List"),
		.position = {0.0f, 0.0f},
		.fontAppearance = appearance,
	};
	kke::Text differentText{
		.text = std::wstring(L"Click GUI"),
		.position = {0.0f, 0.0f},
		.fontAppearance = appearance,
	};

	if (kke::FontHasher::hash(narrowText) != kke::FontHasher::hash(wideText)) {
		return 1;
	}
	if (kke::FontHasher::hash(wideText) == kke::FontHasher::hash(differentText)) {
		return 1;
	}
	return 0;
}
