#pragma once

#define DEFINE_GETTER_SETTER(type, field_name) \
	const type& get##field_name() {            \
		return field_name;                     \
	}                                          \
	void set##field_name(type const& val) {    \
		field_name = val;                      \
	}
