#include "FontHasher.hh"

#include <type_traits>
#include <variant>

#include "FontHashTag.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;

uint64_t FontHasher::hash(FontAppearance const& appearance) {
	Hasher hasher;
	hasher.combine(FontHashTag::TextFormat);
	hasher.combine(appearance.fontFamily);
	hasher.combine(appearance.fontSize);
	hasher.combine(appearance.weight);
	return hasher.get();
}

uint64_t FontHasher::hash(Text const& text) {
	Hasher hasher;
	hasher.combine(FontHashTag::TextLayout);
	hasher.combine(hash(text.fontAppearance));
	uint64_t textHash = 14695981039346656037ull;
	std::visit(
		[&textHash](auto const& value) {
			using Value = std::decay_t<decltype(value)>;
			using Character = typename Value::value_type;
			using UnsignedCharacter = std::make_unsigned_t<Character>;
			for (Character character : value) {
				textHash ^= static_cast<uint64_t>(static_cast<UnsignedCharacter>(character));
				textHash *= 1099511628211ull;
			}
		},
		text.text
	);
	hasher.combine(textHash);
	return hasher.get();
}

std::wstring FontHasher::getTextString(Text const& text) {
	return std::visit(
		[](auto const& value) -> std::wstring {
			using Value = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<Value, std::wstring>) {
				return value;
			} else {
				return toWideString(value);
			}
		},
		text.text
	);
}

std::wstring FontHasher::toWideString(std::string const& text) {
	return std::wstring(text.begin(), text.end());
}
