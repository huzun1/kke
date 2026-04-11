#pragma once

#include <wrl/client.h>

#include <expected>
#include <memory>
#include <string>

#include "DWriteFontService.hh"

using Microsoft::WRL::ComPtr;

class DWriteFontServiceFactory {
public:
	static std::expected<std::shared_ptr<kke::DWriteFontService>, std::string> create(
		ComPtr<IDWriteFactory5> writeFactory);
};
