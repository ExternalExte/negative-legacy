#include"nameof.hpp"
#include<iostream>
#include<valarray>
#include<numeric>
#include<algorithm>
#include<random>
#include<tuple>
#include<future>

namespace{	using std::cin, std::cout, std::endl, std::make_tuple, std::move,std::future; }

auto variance(std::valarray<long double>& data)
{
	auto temp = pow(data - (data.sum() / data.size()), 2.).sum();
	return make_tuple(temp / data.size(), temp / (data.size() - 1));
}
auto average(std::valarray<long double>& data) { return data.sum() / data.size(); }

int main()
{
	
	std::valarray<long double> data(100'000), sample(10'000);
	{
		std::mt19937_64 engine(std::random_device{}());
		std::uniform_real_distribution<long double> dist(1., 10'000.);
		for (auto& i : data)
			i = dist(engine);
		std::sample(begin(data), end(data), begin(sample), size(sample), engine);
	}

	{
		cout << "SAMPLE" << endl;
		auto average = ::average(sample);
		cout << NAMEOF(average) << ":" << average << endl;
	}
	cout << endl;
	{
		cout << "POPULATION" << endl;
		auto [sample_variance, unbiased_variance] = variance(data);
		auto average = ::average(sample);
		cout << NAMEOF(average) << ":" << average << endl;

		auto standard_error = sqrt(unbiased_variance * (size(data) - size(sample))) / sqrt(size(sample) * (size(data) - 1));
		cout << NAMEOF(standard_error) << ":" << standard_error << endl;
	}
	
	return 0;
}