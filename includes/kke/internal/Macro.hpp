#pragma once

#define DEFINE_GETTER_SETTER(type, func_name, field_name) \
	const type& get##func_name() {                        \
		return field_name;                                \
	}                                                     \
	void set##func_name(type const& val) {                \
		field_name = val;                                 \
	}
