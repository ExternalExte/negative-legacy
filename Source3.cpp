#include<iostream>
#include<string>
#include<any>
#include<boost/spirit/home/qi.hpp>
#include<boost/phoenix.hpp>
namespace
{
	using std::cin, std::cout, std::endl;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::qi::ascii;
}

struct calc_gram : qi::grammar<std::string::iterator, double(void), ascii::space_type>
{
public:
	qi::rule<std::string::iterator, double(void), ascii::space_type> term, factor, expr, function;

	calc_gram() : calc_gram::base_type(term)
	{
		using qi::_val, qi::_1;
		expr = (qi::double_ | '(' >> term >> ')')[_val = _1];
		factor = expr[_val = _1] >> *('*' >> expr[_val *= _1] | '/' >> expr[_val /= _1]);
		term = factor[_val = _1] >> *('+' >> factor[_val += _1] | '-' >> factor[_val -= _1]);
	}
};


int main()
{
	auto gram = calc_gram();
	cout << std::boolalpha << ">>>";
	for (std::string buf; std::getline(cin, buf) && buf != "end"; cout << ">>>")
	{
		auto first = buf.begin(), last = buf.end();
		std::any result;
		bool success = qi::phrase_parse(first, last, gram, ascii::space, result);

		if (success)
			cout << "[[success]]:" << std::any_cast<double>(result) << endl;
		else
			cout << "[[failed]]" << endl;
	}
	return 0;
}