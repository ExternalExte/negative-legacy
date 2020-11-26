#include<string>
#include<iostream>
#include<variant>
#include<vector>
#include<functional>
#include<unordered_map>
#include<sstream>
#include<algorithm>
#include<locale>
#include<string_view>

namespace lisplike
{

	class Literal
	{
	public:
		enum class Type
		{
			Number, String, Boolean, Nil
		};
		Type type = Type::Nil;
		std::variant<double, std::string, bool> value;
		Literal() :type(Type::Nil), value() {}
		Literal(std::string _name) :type(Type::String), value(_name) {}
		template<class T>
		Literal(Type _type, T _value) : type(_type), value(_value) {}

	private:
		template<Type> struct from_type { using T = void; };
		template<> struct from_type<Type::Number> { using T = double; };
		template<> struct from_type<Type::String> { using T = std::string; };
		template<> struct from_type<Type::Boolean> { using T = bool; };
		template<> struct from_type<Type::Nil> { using T = nullptr_t; };

		template<Type, class T>	T& get_impl();
		template<> double& get_impl<Type::Number, double>() { return std::get<double>(value); }
		template<> std::string& get_impl<Type::String, std::string>() { return std::get<std::string>(value); }
		template<> bool& get_impl<Type::Boolean, bool>() { return std::get<bool>(value); }

	public:
		template<Type T = Type::Nil>
		typename from_type<T>::T& get() { return get_impl<T, typename from_type<T>::T>(); }
	};

	class Variable_Table
	{
		inline static std::unordered_map<std::string, Literal> variable_table;
	public:
		static void regist(std::string _name, Literal value) { variable_table[_name] = value; }
		static Literal& get(std::string _name) { return variable_table[_name]; }
		static bool check(std::string _name) { return (bool)variable_table.count(_name); }
	};

	class Atom;
	using List = std::vector<Atom>;

	using Function = std::function<Atom(List)>;
	class Function_Table
	{
		inline static std::unordered_map<std::string, Function> func_table;
	public:
		static void regist(std::string _name, Function func) { func_table[_name] = func; }
		static Function& get(std::string& _name) { return func_table[_name]; }
		static bool check(std::string _name) { return (bool)func_table.count(_name); }
	};

	class Atom
	{
	public:
		Literal _literal;
		std::string _name;
		enum class Tags
		{
			Literal, Variable, Function
		};
		Tags tag;
		Atom() :_literal(), _name(), tag(Tags::Literal) {};
		Atom(Literal _literal) :tag(Tags::Literal), _literal(_literal) {}
		Atom(std::string _name, Tags _tag) :tag(_tag), _name(_name) {}
		Atom(Literal _literal, std::string _name) :_literal(_literal), _name(_name), tag(Tags::Literal) {}
	private:
		template<Tags>struct from_tag { using T = void; };
		template<>struct from_tag<Tags::Literal> { using T = Literal; };
		template<>struct from_tag<Tags::Variable> { using T = Literal; };
		template<>struct from_tag<Tags::Function> { using T = Function; };

		template<Tags, class T>T& get_impl();
		template<>Literal& get_impl<Tags::Literal, Literal>() { return _literal; }
		template<>Literal& get_impl<Tags::Variable, Literal>() { return Variable_Table::get(_name); }
		template<>Function& get_impl<Tags::Function, Function>() { return Function_Table::get(_name); }

	public:
		template<Tags _tag>
		typename from_tag<_tag>::T& get() { return get_impl<_tag, typename from_tag<_tag>::T>(); }
	};

#pragma region Functions


	std::string to_string(Literal::Type type)
	{
		switch (type)
		{
		case lisplike::Literal::Type::Number:
			return "Number";
		case lisplike::Literal::Type::String:
			return "String";
		case lisplike::Literal::Type::Boolean:
			return "Boolean";
		case lisplike::Literal::Type::Nil:
			return "Nil";
		}
		return "";
	}

	std::string to_string(Literal _literal)
	{
		std::string head = " type:" + to_string(_literal.type) + ", value:";
		switch (_literal.type)
		{
		case lisplike::Literal::Type::Number:
			return head + std::to_string(_literal.get<lisplike::Literal::Type::Number>());
		case lisplike::Literal::Type::String:
			return head + '\"' + _literal.get<lisplike::Literal::Type::String>() + '\"';
		case lisplike::Literal::Type::Boolean:
			return head + (_literal.get<lisplike::Literal::Type::Boolean>() ? "true" : "false");
		case lisplike::Literal::Type::Nil:
			return "Nil";
		}
		return "";
	}

	std::string to_string(Atom::Tags tag)
	{
		switch (tag)
		{
		case lisplike::Atom::Tags::Literal:
			return "Literal";
		case lisplike::Atom::Tags::Variable:
			return "Variable";
		case lisplike::Atom::Tags::Function:
			return "Function";
		}
		return "";
	}

	std::string to_string(Atom atom)
	{
		using namespace std;
		auto buf = "tag:" + to_string(atom.tag);
		if (atom.tag == lisplike::Atom::Tags::Literal)
			if (auto str_lit = to_string(atom._literal); str_lit == "Nil")
				return "Nil";
			else
				buf += ", " + str_lit;
		else buf += ", name:" + atom._name;
		return buf;
	}

	std::istream& operator>>(std::istream& in, Atom& atom)
	{
		std::string s;
		in >> s;

		if (Function_Table::check(s))
		{
			atom.tag = Atom::Tags::Function;
			atom._name = s;
			return in;
		}
		if (Variable_Table::check(s))
		{
			atom.tag = Atom::Tags::Variable;
			atom._name = s;
			return in;
		}

		atom.tag = Atom::Tags::Literal;
		auto&& _literal = atom._literal;
		std::string lowered;
		lowered.resize(s.size());

		std::transform(s.begin(), s.end(), lowered.begin(), [](char c)->char {return std::tolower(c); });

		if (std::all_of(s.begin(), s.end(), [](char c) {return c == '.' || std::isdigit(c); }))
		{
			_literal.type = Literal::Type::Number;
			_literal.value = std::stod(s);
		}
		else if (lowered == "true" || lowered == "false")
		{
			_literal.type = Literal::Type::Boolean;
			_literal.value = (lowered == "true") ? true : false;
		}
		else if (lowered == "nil")
		{
			_literal.type = Literal::Type::Nil;
		}
		else
		{
			_literal.type = Literal::Type::String;
			_literal.value = s;
		}

		return in;
	}
	std::string_view trim_SideParen(std::string_view string)
	{
		auto first_pos = string.find_first_of('('), last_pos = string.find_last_of(')');
		return string.substr(first_pos + 1, last_pos - first_pos - 1);
	}

	void init()
	{
		auto get_num = [](Atom& atom)->double& { return atom.get<Atom::Tags::Literal>().get<Literal::Type::Number>(); };

		Function_Table::regist("add",
			[&](List list)
			{
				Atom result{ Literal{ Literal::Type::Number,0. } };
				for (auto&& i : list) get_num(result) += get_num(i);
				return result;
			});
		Function_Table::regist("sub",
			[&](List list)
			{
				Atom result{ Literal{Literal::Type::Number,get_num(list[0]) * 2 } };
				for (auto&& i : list)get_num(result) -= get_num(i);
				return result;
			});
		Function_Table::regist("mul",
			[&](List list)
			{
				Atom result;
				result.tag = Atom::Tags::Literal;
				result._literal.type = Literal::Type::Number;
				result._literal.value = 1.;
				for (auto&& i : list)get_num(result) *= get_num(i);
				return result;
			});
		Function_Table::regist("div",
			[&](List list)
			{
				Atom result{ Literal{Literal::Type::Number,get_num(list[0]) * get_num(list[0]) } };
				for (auto&& i : list)get_num(result) /= get_num(i);
				return result;
			});
		Function_Table::regist("eval",
			[&](List list)
			{
				Atom result{ Literal{Literal::Type::String,std::string("")} };
				auto&& ref = result.get<Atom::Tags::Literal>().get<Literal::Type::String>();
				ref = trim_SideParen(ref);
				ref += '(';
				for (auto&& i : list) ref += to_string(i) + '\n';
				ref.pop_back();
				ref += ')';
				return result;
			});
		Function_Table::regist("define",
			[&](List list)
			{
				auto name = list[0].get<Atom::Tags::Literal>().get<Literal::Type::String>();
				Atom result{ name,Atom::Tags::Variable };
				Variable_Table::regist(name, list[1].get<Atom::Tags::Literal>());
				return result;
			});
		Function_Table::regist("assign",
			[&](List list)
			{
				Atom result{ list[0]._name,Atom::Tags::Variable };
				Variable_Table::get(list[0]._name) = list[1].get<Atom::Tags::Literal>();
				return result;
			});
		Function_Table::regist("cmd",
			[&](List list)
			{
				for (auto&& i : list)system(i.get<Atom::Tags::Literal>().get<Literal::Type::String>().c_str());
				return Atom{ Literal{"()"} };
			});
	}
#pragma endregion

	class Expression
	{
	public:
		std::vector<Expression> children;
		Atom result;
		void eval()
		{
			using std::cout, std::endl, std::string;

			if (children.empty())
				return;
			for (auto&& i : children) i.eval();


			auto&& head = children[0].result;
			if (head.tag == Atom::Tags::Function)
			{
				List args;
				for (unsigned i = 1; i < children.size(); ++i)
				{
					auto&& temp = children[i].result;
					switch (temp.tag)
					{
					case Atom::Tags::Function:
						args.emplace_back(temp);
						break;
					case Atom::Tags::Literal:
						if (auto literal_v = temp.get<Atom::Tags::Literal>(); literal_v.type == Literal::Type::Nil)
						{
							if (!children[i].children.empty())
								for (auto&& child : children[i].children)
									args.emplace_back(child.result);
						}
						else
							args.emplace_back(literal_v);
						break;
					case Atom::Tags::Variable:
						args.emplace_back(temp.get<Atom::Tags::Variable>(), temp._name);
						break;
					}
				}
				auto [lit, name, tag] = head.get<Atom::Tags::Function>()(args);
				result.tag = tag;
				result._literal = lit;
				result._name = name;
				children.clear();
				return;
			}
			else return;
		}
	};


	Atom make_atom(std::string& str)
	{
		Atom result;

		if (Function_Table::check(str))
		{
			result.tag = Atom::Tags::Function;
			result._name = str;
			return result;
		}
		if (Variable_Table::check(str))
		{
			result.tag = Atom::Tags::Variable;
			result._name = str;
			return result;
		}

		result.tag = Atom::Tags::Literal;
		auto&& _literal = result._literal;
		std::string lowered;
		lowered.resize(str.size());
		std::transform(str.begin(), str.end(), lowered.begin(), [](char c)->char { return std::tolower(c); });

		if (std::all_of(str.begin(), str.end(), [](char c) {return c == '.' || std::isdigit(c); }))
		{
			_literal.type = Literal::Type::Number;
			_literal.value = std::stod(str);

		}
		else if (auto is_true = lowered == "true"; is_true || lowered == "false")
		{
			_literal.type = Literal::Type::Boolean;
			_literal.value = is_true;
		}
		else if (lowered == "nil")
		{
			_literal.type = Literal::Type::Nil;
		}
		else
		{
			_literal.type = Literal::Type::String;
			_literal.value = str;
		}

		return result;

	}
	namespace details {
		std::string_view make_tree_impl(std::string_view string)
		{
			auto first_pos = string.find_first_of('('), last_pos = string.find_last_of(')');
			return string.substr(first_pos + 1, last_pos - first_pos);
		}
	}

	void make_tree(std::string_view str, Expression& Expression, bool is_first = true)
	{
		using std::cout, std::endl;
		if (is_first) str = details::make_tree_impl(str);
		std::string temp;
		bool is_str = false;
		for (unsigned i = 0; i < str.size(); i++)
		{
			auto&& element = str[i];
			if (element == '(')
			{
				auto open_paren_pos = i;
				auto close_paren_pos = str.find_first_of(')', i);

				auto substr = str.substr(open_paren_pos + 1, close_paren_pos - open_paren_pos);
				auto&& sub_expr = Expression.children.emplace_back();

				make_tree(substr, sub_expr, false);
				i = close_paren_pos;
			}
			else if (element == '\"')is_str = !is_str;
			else if (is_str||element != ' ' && element != ')')
				temp += element;
			else if (!temp.empty())
			{
				auto&& child_r = Expression.children.emplace_back().result;
				child_r = make_atom(temp);
				temp.clear();
			}
		}

	}
}


inline std::string readline() { std::string str; char c; while ((c = getchar()) != '\n')	str += c; return str; }

int main_source()
{
	using std::cin, std::cout, std::endl, lisplike::make_tree, lisplike::Expression, lisplike::to_string;
	lisplike::init();
	Expression test;
	std::string str;
	lisplike::Variable_Table::regist("prev", lisplike::Literal{});
	while (true)
	{
		cout << ">>>";
		str = readline();
		if (str == "exit")return 0;
		test.children.clear();
		test.result = lisplike::Atom{};
		make_tree(str, test);
		test.eval();
		str = lisplike::trim_SideParen(to_string(test.result));
		cout << str << endl;
		lisplike::Variable_Table::get("prev") = lisplike::Literal{ str };
	}
	return 0;
}
