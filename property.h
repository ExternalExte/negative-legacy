#pragma once
#include<functional>

namespace details {
	template <class C, typename TReturn, typename... TArgs>
	auto member_function_class_type_impl(TReturn(C::* member_function)(TArgs...) const)->C;

	template <class C, typename TReturn, typename... TArgs>
	auto member_function_class_type_impl(TReturn(C::* member_function)(TArgs...))->C;

	template <auto MemberFunctionPointer>
	using member_function_class_type_t = decltype(member_function_class_type_impl(MemberFunctionPointer));
};

template <typename TProperty, auto Getter>
class get_only_property {
	using C = details::member_function_class_type_t<Getter>;
public:
	get_only_property(const C& ins) : _instance(ins) {}
	operator TProperty() const { return get(); }
	TProperty operator()() const { return get(); }

#pragma region operators

	template <typename T>
	auto operator[](const T& other) const -> decltype(std::declval<TProperty>[other]) { return get()[other]; }

	template <typename T>
	auto operator+(const T& other) const-> decltype(std::declval<TProperty>() + other) { return get() + other; }

	template <typename T>
	auto operator-(const T& other) const-> decltype(std::declval<TProperty>() - other) { return get() - other; }

	template <typename T>
	auto operator*(const T& other) const-> decltype(std::declval<TProperty>()* other) { return get() * other; }

	template <typename T>
	auto operator/(const T& other) const-> decltype(std::declval<TProperty>() / other) { return get() / other; }

	template <typename T>
	auto operator%(const T& other) const-> decltype(std::declval<TProperty>() % other) { return get() % other; }

	template <typename T>
	auto operator|(const T& other) const-> decltype(std::declval<TProperty>() | other) { return get() | other; }

	template <typename T>
	auto operator&(const T& other) const-> decltype(std::declval<TProperty>()& other) { return get() & other; }

	template <typename T>
	auto operator^(const T& other) const-> decltype(std::declval<TProperty>() ^ other) { return get() ^ other; }

	template <typename T>
	bool operator||(const T& other) const { return get() || other; }

	template <typename T>
	bool operator&&(const T& other) const { return get() && other; }

	template <typename T>
	TProperty operator>>(const T& other) const { return get() >> other; }

	template <typename T>
	TProperty operator<<(const T& other) const { return get() << other; }

	template <typename T>
	bool operator==(const T& other) const { return get() == other; }

	template <typename T>
	bool operator!=(const T& other) const { return get() != other; }

	template <typename T>
	bool operator>(const T& other) const { return get() > other; }

	template <typename T>
	bool operator>=(const T& other) const { return get() >= other; }

	template <typename T>
	bool operator<(const T& other) const { return get() < other; }

	template <typename T>
	bool operator<=(const T& other) const { return get() <= other; }

	TProperty operator+() const { return +get(); }
	TProperty operator-() const { return -get(); }
	TProperty operator!() const { return !get(); }
	TProperty operator~() const { return ~get(); }

#pragma endregion

protected:
	TProperty get() const { return (_instance.*Getter)(); }

private:
	const C& _instance;
};

template <typename TProperty, auto Setter>
class set_only_property {
	using C = details::member_function_class_type_t<Setter>;

public:
	set_only_property(C& ins) : _instance(ins) {}

	set_only_property& operator=(const TProperty& v) { set(v);	return *this; }

	void operator()(const TProperty& v) { set(v); }

protected:
	void set(const TProperty& v) { (_instance.*Setter)(v); }

private:
	C& _instance;
};

template <typename TProperty, auto Getter, auto Setter>
class get_set_property : public get_only_property<TProperty, Getter>,
	public set_only_property<TProperty, Setter> {
	using C = details::member_function_class_type_t<Getter>;
	static_assert(std::is_same_v<C, details::member_function_class_type_t<Setter>>,
		"The class of Getter and Setter must be same.");
	using TGetProperty = get_only_property<TProperty, Getter>;
	using TSetProperty = set_only_property<TProperty, Setter>;

public:
	get_set_property(C& ins) : TGetProperty(ins), TSetProperty(ins) {}

#pragma region operators

	get_set_property& operator=(const TProperty& v) { TSetProperty::operator=(v); return *this; }

	get_set_property& operator++() { auto buf = TGetProperty::get(); TSetProperty::set(++buf); return *this; }

	TProperty operator++(int) { auto buf = TGetProperty::get();	auto ret = buf++; TSetProperty::set(buf); return ret; }

	get_set_property& operator--() { auto buf = TGetProperty::get(); TSetProperty::set(--buf); return *this; }

	TProperty operator--(int) { auto buf = TGetProperty::get();	auto ret = buf--; TSetProperty::set(buf); return ret; }

	template <typename T>
	get_set_property& operator+=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf += other); return *this; }

	template <typename T>
	get_set_property& operator-=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf -= other); return *this; }

	template <typename T>
	get_set_property& operator*=(const T& other) { auto buf = TGetProperty::get(); TSetProperty::set(buf *= other); return *this; }

	template <typename T>
	get_set_property& operator/=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf /= other); return *this; }

	template <typename T>
	get_set_property& operator%=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf %= other); return *this; }

	template <typename T>
	get_set_property& operator|=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf |= other); return *this; }

	template <typename T>
	get_set_property& operator&=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf &= other); return *this; }

	template <typename T>
	get_set_property& operator^=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf ^= other); return *this; }

	template <typename T>
	get_set_property& operator>>=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf >>= other); return *this; }

	template <typename T>
	get_set_property& operator<<=(const T& other) { auto buf = TGetProperty::get();	TSetProperty::set(buf <<= other); return *this; }

#pragma endregion

};
