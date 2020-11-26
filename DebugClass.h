#pragma once
#include<iostream>
#include<string>
using namespace std::string_literals;

class DebugClass
{
public:
#pragma region
	DebugClass()noexcept
	{
		std::cout << "default constructor:" << this << std::endl;
	}
	DebugClass(const DebugClass& x)noexcept
	{
		std::cout << "copy constructor:" << this << "<--" << &x << std::endl;
	}

	DebugClass(DebugClass&& x)noexcept
	{
		std::cout << "move constructor:" << this << "<--" << &x << std::endl; 
	}

	DebugClass(std::initializer_list<int>)noexcept
	{
		std::cout << "initializer_list constructor:" << this << std::endl;
	}

	~DebugClass()noexcept
	{
		std::cout << "destructor:" << this << std::endl; 
	}

	DebugClass& operator=(const DebugClass& x)noexcept
	{
		std::cout << "copy assignment:" << this << "<--" << &x << std::endl;
		return *this;
	}
	DebugClass& operator=(DebugClass&& x)noexcept
	{
		std::cout << "move assignment:" << this << "<-" << &x << std::endl;
		return *this;
	}
	void operator()()noexcept
	{
		std::cout << "function call:" << this << std::endl;
	}
	static void* operator new(std::size_t n)//���̊֐��̎��s��Ɏ����ŃR���X�g���N�^���Ăяo��
	{
		auto p = ::operator new(n);
		std::cout << "new:" << p << std::endl;
		return p;
	}
	static void operator delete(void* p)noexcept//�f�X�g���N�^�͂��̊֐��̎��s�O�Ɏ����ŌĂ΂��
	{
		std::cout << "delete:" << p << std::endl;
		::operator delete(p);
	}
#pragma endregion
};
