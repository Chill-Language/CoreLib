#pragma once
#ifndef _TinyChillParser_Utils_BindPtr_Header_
#define _TinyChillParser_Utils_BindPtr_Header_
#include "base.hpp"
#include <cstdint>
#include <cassert>

ChillParser_Begin

template <typename DataType, typename PropertyType, unsigned Size = 2>
class BindPtr {
public:
	BindPtr() = default;

	BindPtr(const DataType* data, PropertyType property) {
		_data = reinterpret_cast<std::uintptr_t>(data);
		assert(((_data & ~_mask()) == _data) && (property <= _mask()));
		_data |= static_cast<std::uintptr_t>(property);
	}

	bool isNull() const {
		return _data == 0;
	}

	DataType* get() {
		return reinterpret_cast<DataType*>(_data & ~_mask());
	}
	const DataType* get() const {
		return const_cast<BindPtr*>(this)->get();
	}

	DataType* operator->() {
		return get();
	}
	const DataType* operator->() const {
		return get();
	}

	PropertyType property() const {
		return static_cast<PropertyType>(_data & _mask());
	}

private:
	std::uintptr_t _data = 0;

	static constexpr std::uintptr_t _mask() {
		return static_cast<std::uintptr_t>(1ull << Size) - 1;
	}
};

ChillParser_End

#endif
