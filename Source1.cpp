#include<vector>
#include<algorithm>
#include<chrono>
#include<iostream>
#include<limits>
#include<random>
#include<unordered_map>
#include<map>
#include<string_view>
#include<functional>
#include<future>
#include<execution>

using iterator_t = std::vector<int>::iterator;
struct various_sorts
{
	static void stalin(iterator_t first, iterator_t last, iterator_t out)
	{
		*out = *first;
		for (auto i = first + 1, j = out + 1; i != last; ++i, ++j)
			if (*i > * (i - 1))
				*j = *i;
	}

	static void washing(iterator_t first, iterator_t last, iterator_t out)
	{
		auto max = *out = *first;
		for (auto i = first + 1, j = out + 1; i != last; ++i, ++j)
			*j = max = std::max(*i, max);
	}

	static void std(iterator_t first, iterator_t last) { std::sort(first, last); }
	static void std_stable(iterator_t first, iterator_t last) { std::stable_sort(first, last); }

	static void bubble(iterator_t first, iterator_t last)
	{
		bool is_sorted = false;
		unsigned adjust = 1;
		while (!is_sorted)
		{
			is_sorted = true;
			for (auto i = first; i != last - adjust; ++i)
				if (*i > * (i + 1))
				{
					std::swap(*i, *(i + 1));
					is_sorted = false;
				}
			++adjust;
		}
	}
	static void shaker(iterator_t first, iterator_t last)
	{
		bool is_sorted = false;
		unsigned adjust = 1;
		while (!is_sorted)
		{
			is_sorted = true;
			for (auto i = first; i != last - adjust; ++i)
				if (*i > * (i + 1))
				{
					std::swap(*i, *(i + 1));
					is_sorted = false;
				}
			for (auto i = last - 1; i != first + adjust; --i)
				if (*i < *(i - 1))
				{
					std::swap(*i, *(i - 1));
					is_sorted = false;
				}
			++adjust;
		}
	}
	static void comb(iterator_t first, iterator_t last)
	{
		unsigned duration = last - first;
		bool is_sorted = false;
		while (!is_sorted || duration != 1)
		{
			is_sorted = true;
			if (duration > 1)
				duration /= 1.3;
			for (auto i = first; i + duration != last; ++i)
				if (*i > * (i + duration))
				{
					std::swap(*i, *(i + duration));
					is_sorted = false;
				}
		}
	}
	static void gnome(iterator_t first, iterator_t last)
	{
		auto base_pos = first;
		for (auto i = first; i + 1 != last;)
		{
			if (*i > * (i + 1))
			{
				std::swap(*i, *(i + 1));
				if (i != first)
					--i;
			}
			else
			{
				++base_pos;
				i = base_pos;
			}
		}
	}

	static void selection(iterator_t first, iterator_t last)
	{
		for (auto&& i = first; i != last; ++i)
		{
			auto min_itr = i;//ŠO‚Éo‚µ‚½ê‡‚Æ”äŠr‚µ‚Ä—LˆÓ·‚ªŒ©‚ç‚ê‚È‚©‚Á‚½‚Ì‚Å(ry
			for (auto j = i; j != last; ++j)
				if (*min_itr > * j)
					min_itr = j;
			std::swap(*i, *min_itr);
		}
	}

	static void insertion(iterator_t first, iterator_t last)
	{
		for (auto&& i = first + 1; i != last; ++i)
			for (auto&& j = i; j > first&&* (j - 1) > * j; --j)
				std::swap(*(j - 1), *j);
	}

	static void comb_insertion(iterator_t first, iterator_t last)
	{
		for (unsigned duration = (last - first) / 1.3; duration > 1; duration /= 1.3)
			for (auto i = first, j = first + duration; j != last; ++i, ++j)
				if (*i > * j)
					std::swap(*i, *j);

		for (auto i = first + 1; i != last; ++i)
			for (auto j = i; j > first&&* (j - 1) > * j; --j)
				std::swap(*(j - 1), *j);
	}

	static void merge(iterator_t first, iterator_t last)
	{
		std::vector<iterator_t::value_type> buffer(size_t(last - first));
		auto rlast = std::make_reverse_iterator(last), rfirst = std::make_reverse_iterator(first);
		merge_impl(buffer, first, last, rlast, rfirst);
	}
	static void merge_impl(std::vector<iterator_t::value_type>& buffer,
		iterator_t& first, iterator_t& last,
		std::reverse_iterator<iterator_t>& rlast, std::reverse_iterator<iterator_t>& rfirst)
	{
		size_t size = last - first;
		if (size == 1)
			return;
		if (size == 2)
		{
			if (auto& v1 = *first, &v2 = *(first + 1); v1 > v2)
				std::swap(v1, v2);
			return;
		}
		auto mid = first + size_t(size / 2);
		auto rmid = rfirst - size_t(size / 2);
		merge_impl(buffer, first, mid, rmid, rfirst);
		merge_impl(buffer, mid, last, rlast, rmid);
		std::copy(rlast, rmid, std::copy(first, mid, buffer.begin()));

		auto left = buffer.begin(), right = buffer.begin() + size - 1;
		for (auto i = first; i != last; ++i)
			if (*left > * right)
			{
				*i = *right;
				--right;
			}
			else
			{
				*i = *left;
				++left;
			}
	}

	static void merge_comb_insertion(iterator_t first, iterator_t last)
	{
		std::vector<iterator_t::value_type> buffer(size_t(last - first));
		auto rlast = std::make_reverse_iterator(last), rfirst = std::make_reverse_iterator(first);
		merge_comb_insertion_impl(buffer, first, last, rlast, rfirst);
	}
	static void merge_comb_insertion_impl(std::vector<iterator_t::value_type>& buffer,
		iterator_t& first, iterator_t& last,
		std::reverse_iterator<iterator_t>& rlast, std::reverse_iterator<iterator_t>& rfirst)
	{
		size_t size = last - first;
		if (size == 1)
			return;
		if (size == 2)
		{
			if (auto& v1 = *first, &v2 = *(first + 1); v1 > v2)
				std::swap(v1, v2);
			return;
		}
		if (size < 2048)
		{
			comb_insertion(first, last);
			return;
		}
		auto mid = first + size_t(size / 2);
		auto rmid = rfirst - size_t(size / 2);
		merge_comb_insertion_impl(buffer, first, mid, rmid, rfirst);
		merge_comb_insertion_impl(buffer, mid, last, rlast, rmid);
		std::copy(rlast, rmid, std::copy(first, mid, buffer.begin()));

		auto left = buffer.begin(), right = buffer.begin() + size - 1;
		for (auto i = first; i != last; ++i)
			if (*left > * right)
			{
				*i = *right;
				--right;
			}
			else
			{
				*i = *left;
				++left;
			}
	}

};

struct compare_by_second
{
public:
	constexpr bool operator()(const std::pair<std::string_view, double>& a, const std::pair<std::string_view, double>& b)
	{
		if (a.second != b.second)
			return a.second < b.second;
		else
			return a.first < b.first;
	}
};

class compare_sorts
{
private:
	inline static std::chrono::system_clock::time_point start_point_buffer;
	inline static long long result_time_buffer;
public:
	inline static std::unordered_multimap<std::string_view, long long> result_times;
	static void measure_start() { start_point_buffer = std::chrono::system_clock::now(); }
	static void measure_end() { auto now = std::chrono::system_clock::now(); result_time_buffer = (now - start_point_buffer).count(); }
	static void record(std::string_view str) { result_times.pass(str, result_time_buffer); }
	static void make_random_array(iterator_t first, iterator_t last)
	{
		static bool is_first_call = true;
		std::random_device seed_gene;
		std::mt19937_64 engine(seed_gene.operator());
		if (is_first_call)
		{
			std::uniform_int_distribution<int> dist{ INT_MIN,INT_MAX };
			std::generate(first, last, std::bind(dist, engine));
			is_first_call = false;
		}
		else
			std::shuffle(first, last, engine);
	}
	static void check_sorted(iterator_t first, iterator_t last) { std::cout << "is" << (std::is_sorted(first, last) ? " " : "n't ") << "sorted!!!!" << std::endl; }
	static void print(iterator_t first, iterator_t last) { for (auto i = first; i != last; ++i)std::cout << *i << std::endl; }
};

int main()
{
	using std::cin, std::cout, std::endl, std::vector;
	constexpr size_t N = 1000;
	vector<int> ary(N);
	cout << std::fixed << std::noshowpoint;
#define MEASURE(func_name)	\
	compare_sorts::make_random_array(ary.begin(), ary.end());\
	compare_sorts::measure_start();\
	various_sorts::func_name(ary.begin(), ary.end());\
	compare_sorts::measure_end();\
	compare_sorts::record(#func_name)
#define check() compare_sorts::check_sorted(ary.begin(),ary.end());
#define print() compare_sorts::print(ary.begin(),ary.end());

	unsigned count = 0;
	for (unsigned i = 0; i < 10; i++)
	{
		//MEASURE(std);
		//MEASURE(std_stable);
		//MEASURE(merge);
		MEASURE(comb_insertion);
		//MEASURE(merge_comb_insertion);

		//MEASURE(comb);
		//MEASURE(selection);
		//MEASURE(insertion);
		//MEASURE(gnome);
		//MEASURE(shaker);
		//MEASURE(bubble);
	}

	std::vector<std::pair<std::string_view, long long>> sorted_results{ compare_sorts::result_times.size() };
	std::copy(compare_sorts::result_times.begin(), compare_sorts::result_times.end(), sorted_results.begin());
	std::sort(sorted_results.begin(), sorted_results.end(), compare_by_second{});

	for (auto&& [str, i] : sorted_results)
		cout << i << ":" << str << endl;

	return 0;
}

#undef MEASURE
#undef check
#undef print

unsigned bits_MSB(unsigned v)
{
	v |= v >> 1u;
	v |= v >> 2u;
	v |= v >> 4u;
	v |= v >> 8u;
	v |= v >> 16u;
	return v ^ (v >> 1u);
}

unsigned bits_reverse(unsigned v)
{
	v = (v & 0x55555555u) << 1u | (v >> 1u & 0x55555555u);
	v = (v & 0x33333333u) << 2u | (v >> 2u & 0x33333333u);
	v = (v & 0x0f0f0f0fu) << 4u | (v >> 4u & 0x0f0f0f0fu);
	v = (v & 0x00ff00ffu) << 8u | (v >> 8u & 0x00ff00ffu);
	v = (v & 0x0000ffffu) << 16u | (v >> 16u & 0x0000ffffu);
	return v;
}

unsigned bits_count(unsigned v)
{
	v = (v & 0x55555555u) + (v >> 1u & 0x55555555u);
	v = (v & 0x33333333u) + (v >> 2u & 0x33333333u);
	v = (v & 0x0f0f0f0fu) + (v >> 4u & 0x0f0f0f0fu);
	v = (v & 0x00ff00ffu) + (v >> 8u & 0x00ff00ffu);
	v = (v & 0x0000ffffu) + (v >> 16u & 0x0000ffffu);
	return v;
}
