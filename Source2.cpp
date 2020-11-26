#include<algorithm>
#include<iostream>
#include<iomanip>
#include<numeric>
#include<valarray>
#include<vector>
#include<map>
#include<list>
#include<bitset>
#include<stdio.h>
#include<sstream>
#include<string>
#include<functional>
#include<utility>
#include<cmath>
#include<memory>
#include<any>
#include<variant>
#include<random>
#include<chrono>
#include<thread>
#include<mutex>
#include<future>
#include<type_traits>
#include<filesystem>
#include<fstream>
#include<regex>
#include<locale>
#include<codecvt>
#include<execution>
#include<complex>
#include<string_view>
#include<queue>
#include<concepts>

namespace details { using namespace std; }

template<class Ty>
struct my_concept : std::enable_if<Ty::value, nullptr_t> {};
template<class Ty>
using concept_t = typename my_concept<Ty>::type;

using swallow = std::initializer_list<int>;

template<unsigned Priority>
struct overload_priority_t : overload_priority_t<Priority - 1> {};
template<>
struct overload_priority_t<0> {};
namespace overload_priority {
	using highest = const overload_priority_t<6>*;
	using higher = const overload_priority_t<5>*;
	using high = const overload_priority_t<4>*;
	using normal = const overload_priority_t<3>*;
	using low = const overload_priority_t<2>*;
	using lower = const overload_priority_t<1>*;
	using lowest = const overload_priority_t<0>*;
}
overload_priority::highest use_overload_priority = nullptr;

namespace details {
	template<class AlwaysVoid, template<class...>class Op, class ..._Tys>
	struct is_detected_impl :public std::false_type {};

	template<template<class...>class Op, class ..._Tys>
	struct is_detected_impl<void_t<Op<_Tys...>>, Op, _Tys...> :public std::true_type {};
}

template<template<class...>class Op, class ..._Tys>
using is_detected = details::is_detected_impl<void, Op, _Tys...>;

template<size_t index, class..._Tys>
using mget = std::tuple_element<index, std::tuple<_Tys...>>;
template<size_t index, class..._Tys>
using mget_t = std::tuple_element_t<index, std::tuple<_Tys...>>;

template<class>
using true_type_Op = std::true_type;
template<template<class>class Tester, class Result>
struct mcase {};
template<class, class...>
struct mswitch;
template<class T, template<class>class...Ops, class... _Tys>
struct mswitch<T, mcase<Ops, _Tys>...>
{
	static constexpr size_t size = sizeof...(_Tys);
	template<size_t N>
	using N_type = mget<N, _Tys...>;
	template<size_t N>
	using N_type_t = mget_t<N, _Tys...>;
	static constexpr bool conditions[size] = { Ops<T>::value... };

	template<size_t N>
	struct mswitch_impl :std::conditional_t<conditions[N], N_type<N>, mswitch_impl<N + 1>> {};
	template<>
	struct mswitch_impl<size - 1> :std::enable_if<conditions[size - 1], N_type_t<size - 1>> {};
public:
	using type = typename mswitch_impl<0>::type;
};

template<bool Test, class Result>
using mif = std::enable_if<Test, Result>;
template<bool Test, class if_true, class if_false>
using mif_else = std::conditional<Test, if_true, if_false>;

template<class, template<class...>class>
struct mapply_from_tuple { using type = void; };

template<template<class...>class Op, class...Args>
struct mapply_from_tuple<std::tuple<Args...>, Op> { using type = Op<Args...>; };

template<class from, class to, class...Args>
struct mreplace
{
private:
	static constexpr size_t size = sizeof...(Args);
	template<size_t N>
	using N_type = mget<N, Args...>;
	static constexpr bool conditions[size] = { std::is_same_v<from,Args>... };
	template<size_t N>
	using part_t = std::conditional_t<conditions[N], to, typename N_type<N>::type>;
	template<class>
	struct type_impl { using type = void; };
	template<size_t... Seq>
	struct type_impl<std::index_sequence<Seq...>> { using type = std::tuple<part_t<Seq>...>; };
public:
	using type = class type_impl<std::make_index_sequence<size>>::type;
};

template<unsigned N>
struct mplaceholder {};
template<>
struct mplaceholder<0>;


template<template<class...>class Tester, class...Args>
struct mbind
{
private:
	using wrapped_args = std::tuple<Args...>;
	template<size_t N>
	using arg = std::tuple_element_t<N, wrapped_args>;
	template<class..._Tys>
	struct mbind_impl
	{
		using wrapped_tys = std::tuple<_Tys...>;
		template<size_t N>
		using ty = std::tuple_element_t<N - 1, wrapped_tys>;
		template<class T>
		struct part_impl { using type = T; };
		template<size_t N>
		struct part_impl<mplaceholder<N>> { using type = ty<N>; };
		template<size_t index>
		using part = typename part_impl<arg<index>>::type;
		template<class>
		struct apply_impl { using type = void; };

		template<size_t...Seq>
		struct apply_impl<std::index_sequence<Seq...>> { using type = Tester<part<Seq>...>; };

		using apply = typename apply_impl<std::make_index_sequence<std::tuple_size_v<wrapped_args>>>::type;
	};
public:
	template<class...Types>
	using apply = typename mbind_impl<Types...>::apply;
};

namespace mplaceholders
{
	using m_1 = mplaceholder<1>;
	using m_2 = mplaceholder<2>;
	using m_3 = mplaceholder<3>;
	using m_4 = mplaceholder<4>;
	using m_5 = mplaceholder<5>;
	using m_6 = mplaceholder<6>;
	using m_7 = mplaceholder<7>;
	using m_8 = mplaceholder<8>;
	using m_9 = mplaceholder<9>;
	using m_10 = mplaceholder<10>;
	using m_11 = mplaceholder<11>;
	using m_12 = mplaceholder<12>;
	using m_13 = mplaceholder<13>;
	using m_14 = mplaceholder<14>;
	using m_15 = mplaceholder<15>;
	using m_16 = mplaceholder<16>;
	using m_17 = mplaceholder<17>;
	using m_18 = mplaceholder<18>;
	using m_19 = mplaceholder<19>;
	using m_20 = mplaceholder<20>;
}

namespace details {
	template<class T>	using has_begin_op = decltype(std::declval<T>().begin());
	template<class T>	using has_end_op = decltype(std::declval<T>().end());
	template<class T>	using is_defined_begin_op = decltype(begin(std::declval<T>()));
	template<class T>	using is_defined_end_op = decltype(end(std::declval<T>()));
	template<class T>	using has_tuple_interface_op = tuple<decltype(get<0>(std::declval<T>())), tuple_size<T>, tuple_element<0, T>>;
	template<class T>	using is_istreamable_op = decltype(declval<istream&>() >> std::declval<T&>());
	template<class T>	using is_ostreamable_op = decltype(declval<ostream&>() << std::declval<T&>());
};

template<class T>	using has_begin = is_detected<details::has_begin_op, T>;
template<class T>	using has_end = is_detected<details::has_end_op, T>;
template<class T>	using is_defined_begin = is_detected<details::is_defined_begin_op, T>;
template<class T>	using is_defined_end = is_detected<details::is_defined_end_op, T>;
template<class T>	using is_iterable = std::conjunction<std::disjunction<has_begin<T>, is_defined_begin<T>>, std::disjunction<has_end<T>, is_defined_end<T>>>;
template<class T>	using has_tuple_interface = is_detected<details::has_tuple_interface_op, T>;
template<class T>	using is_istreamable = is_detected<details::is_istreamable_op, T>;
template<class T>	using is_ostreamable = is_detected<details::is_ostreamable_op, T>;

template<class T>	using only_iterable = concept_t<is_iterable<T>>;
template<class T>	using only_tuple_interface = concept_t<has_tuple_interface<T>>;
template<class T>	using only_istreamable = concept_t<is_istreamable<T>>;
template<class T>	using only_ostreamable = concept_t<is_ostreamable<T>>;
template<class T>	using only_integral = concept_t<std::is_integral<T>>;
template<class T>	using only_signed_integral = concept_t<std::conjunction<std::is_integral<T>, std::is_signed<T>>>;
template<class T>	using only_unsigned_integral = concept_t<std::conjunction<std::is_integral<T>, std::is_unsigned<T>>>;
template<class T>	using only_array = concept_t<std::is_array<T>>;
template<class T, class U>	using only_same = concept_t<std::is_same<T, U>>;

namespace read_alpha
{
	std::istream& readerStream = std::cin;
	namespace details
	{
		template<class T, only_same<char, T> = nullptr>
		inline char read_impl(overload_priority::highest) { return readerStream.get(); }

		template<class T, only_same<T, std::string> = nullptr>
		inline std::string read_impl(overload_priority::highest)
		{
			string str;
			char c = ' ';
			for (; c == '\n' || c == ' ' || c == '\t'; c = readerStream.get());
			for (str += c; (c = readerStream.get()) != '\n'; str += c);
			return str;
		}

		template<class T, only_signed_integral<T> = nullptr>
		inline T read_impl(overload_priority::higher)
		{
			T x = 0, f = 1; char ch = readerStream.get();
			while (ch < '0' || ch>'9') { if (ch == '-') f = -1; ch = readerStream.get(); }
			while (ch >= '0' && ch <= '9') { x = 10 * x + ch - '0'; ch = readerStream.get(); }
			return x * f;
		}


		template<class T, only_unsigned_integral<T> = nullptr>
		inline T read_impl(overload_priority::higher)
		{
			T n = 0; char c = readerStream.get();
			while (c >= '0' && c <= '9') { n = 10 * n + c - '0'; c = readerStream.get(); }
			return n;
		}

		template<class T, only_istreamable<T> = nullptr>
		inline T read_impl(overload_priority::normal)
		{
			T s;
			readerStream >> s;
			return s;
		}

		template<class T, concept_t<std::conjunction<std::is_constructible<T, std::string>, is_istreamable<T>>> = nullptr>
		inline T read_impl(overload_priority::low)
		{
			string str;
			readerStream >> str;
			T temp(str);
			return temp;
		}

		template<class T, concept_t<std::conjunction<std::is_default_constructible<T>, std::is_assignable<T, std::string>, is_istreamable<T>>> = nullptr>
		inline T read_impl(overload_priority::lower)
		{
			std::string str;
			readerStream >> str;
			T temp;
			temp = str;
			return temp;
		}
		template<class T>	using is_readable_op = decltype(read_impl<T>(use_overload_priority));
	}

	template<class T> using is_readable = is_detected<details::is_readable_op, T>;
	template<class T> using only_readable = concept_t<is_readable<T>>;
	struct reader
	{
	public:
		template<class T, only_readable<T> = nullptr>
		inline operator T() { return details::read_impl<T>(use_overload_priority); }

		template<class T, only_readable<T> = nullptr>
		inline T get() { return details::read_impl<T>(use_overload_priority); }

	private:
		friend reader read();
		reader() {}
	};

	inline reader read() { return reader(); }
}
namespace beta_read
{
	namespace details
	{
		template<class T>
		T read_impl();
		template<>
		char read_impl()
		{
			char c;
			scanf_s("%c", &c);
			return c;
		}
		template<class T>
		T read_impl()
		{
			static_assert(std::is_integral_v<T> && (sizeof T) == (sizeof short()));

		}
		void test()
		{
			read_impl<int>();
		}

		template<class T>
		using is_readable_op = decltype(read_impl<T>());
	}

	template<class T>
	using is_readable = is_detected<details::is_readable_op, T>;

	class reader
	{
	public:
		template<class T>
		operator T()
		{
			static_assert(is_readable<T>::value);
			return details::read_impl<T>();
		}
		reader(const reader&) = delete;
		reader(reader&&) = delete;
	private:
		friend reader read();
		reader() = default;
	};
	
	reader read()	
	{
		return {};
	}
}

template<class begin_t, class end_t = begin_t>
class range
{
	begin_t _first;
	end_t _last;
public:
	range(begin_t first, end_t last) :_first(first), _last(last) {}
	begin_t begin() { return _first; }
	end_t end() { return _last; }
	std::reverse_iterator<begin_t> rbegin() { return std::make_reverse_iterator(_last); }
	std::reverse_iterator<end_t> rend() { return std::make_reverse_iterator(_first); }
	size_t size() { return std::distance(_first, _last); }
};

template<class T, class U>T begin(range<T, U> _range) { return _range.begin(); }
template<class T, class U>U end(range<T, U> _range) { return _range.end(); }
template<class T, class U>T rbegin(range<T, U> _range) { return _range.rbegin(); }
template<class T, class U>U rend(range<T, U> _range) { return _range.rend(); }
std::string to_string(bool _Val) { return (_Val) ? "true" : "false"; }

template<class char_type = char, class string_type = std::basic_string<char_type>, class ifstream_type = std::basic_ifstream<char_type>>
string_type readfile(std::filesystem::path path)
{
	ifstream_type file;
	file.open(path);
	if (!file)
	{
		std::cerr << "error file [" << path.string() << "] cannot be opened in " << __FILE__ << " : line " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
	}
	string_type buf;
	for (char_type c; file.get(c); buf += c);
	file.close();
	return buf;
}

namespace details {
	template <class C, typename TReturn, typename... TArgs>
	auto member_function_class_type_impl(TReturn(C::* member_function)(TArgs...) const)->C;

	template <class C, typename TReturn, typename... TArgs>
	auto member_function_class_type_impl(TReturn(C::* member_function)(TArgs...))->C;

	template <auto MemberFunctionPointer>
	using member_function_class_type_t = decltype(member_function_class_type_impl(MemberFunctionPointer));
};

template <typename TProperty, auto Getter>
class get_only_property
{
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

void start(std::filesystem::path path, std::string_view args)
{
	system(("cd " + path.remove_filename().string() + " & " + path.filename().string() + " " + args.data()).c_str());
}

class call_process :public std::stringbuf
{
	std::filesystem::path path;
public:
	call_process(std::filesystem::path path) :std::stringbuf(), path(path) {}
	int_type sync()override
	{
		start(path, str());
		return 0;
	}
};

namespace algebraic_structure
{
	//èWçá
	struct set {};

	//ìÒçÄââéZ
	template<int traits>
	struct operate { static constexpr int operate_traits = traits; };

	//ï¬ê´
	constexpr int closure = 0b1;
	//ëSàÊê´
	constexpr int totality = 0b10;
	//åãçáñ@ë•
	constexpr int associativity = 0b100;
	//íPà å≥ÇÃë∂ç›
	constexpr int identity = 0b1000;
	//ãtå≥ÇÃë∂ç›
	constexpr int invertibility = 0b10000;
	//åä∑ñ@ë•
	constexpr int commutativity = 0b100000;

	//îºåQ
	struct semi_group :set, operate<closure | totality | associativity> {};
	//â¬ä∑îºåQ
	struct commutative_semi_group :set, operate<closure | totality | associativity | commutativity> {};
	//ÉÇÉmÉCÉh
	struct monoid :set, operate<closure | totality | associativity | identity> {};
	//â¬ä∑ÉÇÉmÉCÉh
	struct commutative_monoid :set, operate<closure | totality | associativity | identity | commutativity> {};
	//åQ
	struct group :set, operate<closure | totality | associativity | identity | invertibility> {};
	//ÉAÅ[ÉxÉãåQ
	struct abelian_group :set, operate<closure | totality | associativity | identity | invertibility | commutativity> {};

	//â¡éZ
	template<class traits>
	struct addition { using addition_traits = traits; };
	//èÊéZ
	template<class traits>
	struct multiplication { using multiplication_traits = traits; };

	//ï™îzñ@ë•
	template<template<class>class add, template<class>class mul>
	struct distributive { static constexpr bool is_distributive = true; };

	//ä¬
	struct ring :set, addition<abelian_group>, multiplication<semi_group>, distributive<addition, multiplication> {};
	//â¬ä∑ä¬
	struct commutative_ring :set, addition<abelian_group>, multiplication<commutative_semi_group>, distributive<addition, multiplication> {};
	//ëÃ
	struct field :set, addition<abelian_group>, multiplication<group>, distributive<addition, multiplication> {};
	//â¬ä∑ëÃ
	struct commutative_field :set, addition<abelian_group>, multiplication<abelian_group>, distributive<addition, multiplication> {};

	//é©ëRêî
	struct NaturalNumber :addition<commutative_monoid>, multiplication<commutative_monoid>, distributive<addition, multiplication> {};
	//êÆêî
	struct Integer :addition<abelian_group>, multiplication<commutative_monoid>, distributive<addition, multiplication> {};
	//óLóùêî
	struct RationalNumber :commutative_field {};
	//é¿êî
	struct RealNumber :commutative_field {};
	//ï°ëfêî
	struct ComplexNumber :commutative_field {};
	//élå≥êî
	struct Quaternion :field {};
}
template<class T>
using uniq = std::unique_ptr<T>;

template<class T>
auto pass(uniq<T>& ptr) { return std::move(ptr); }
template<class T>
auto pass(uniq<T>&& ptr) { return std::move(ptr); }
template<class T>
auto pass(T* ptr) { return uniq<T>(ptr); }

template<class T, class... Types>
auto pass(Types&&... args) { return std::make_unique<T>(args...); }

int main_source2()
{
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	using std::cin, std::cout, std::endl;
	using namespace std;

	return 0;
}
