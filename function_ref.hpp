#pragma once
#include <type_traits>
#include <functional>

template <class FunctionType>
class function_ref;

template <class ReturnType, class... Args>
class function_ref<ReturnType(Args...)> final
{
	using Call = ReturnType(*)(void*, Args &&...);

	static ReturnType uninitCall(void*, Args&&...)
	{
		throw std::bad_function_call();
	}

	/// operator()�ɂ���Ď��ۂɌĂяo�����b�p�[�֐�
	template <class Fun>
	static ReturnType call(void* object, Args&&... args) noexcept(
		noexcept(std::declval<Fun>()(std::forward<Args>(args)...)))
	{
		using Pointer = std::add_pointer_t<Fun>;
		return (*static_cast<Pointer>(object))(std::forward<Args>(args)...);
	}

	/// �֐��I�u�W�F�N�g�ւ̃|�C���^
	void* object_ = nullptr;

	/// object_���Ăяo�����߂̃��b�p�[�֐��ւ̃|�C���^
	Call call_ = &function_ref::uninitCall;

public:
	constexpr function_ref() noexcept = default;

	constexpr function_ref(std::nullptr_t) noexcept {}

	constexpr function_ref(const function_ref&) noexcept = default;

	template <class Fun>
	constexpr function_ref(Fun&& fun) noexcept
	{
		static_assert(std::is_invocable_r_v<ReturnType, Fun, Args...>, "argument is not invocable with specified signature.");
		// fun�̃A�h���X���擾��object_�Ɋi�[
		// operator&���I�[�o�[���[�h����Ă���ꍇ���l���ĉ�肭�ǂ����@��p���Ă���
		object_ = std::addressof(fun);

		// ���b�p�[�֐������̉����Ċi�[
		call_ = &function_ref::call<Fun>;
	}

	/// �R�s�[�R���X�g���N�^
	constexpr function_ref& operator=(const function_ref&) noexcept = default;

	constexpr function_ref& operator=(std::nullptr_t) noexcept
	{
		object_ = nullptr;
		call_ = &function_ref::uninitCall;
	}

	template <class Fun>
	constexpr function_ref& operator=(Fun&& fun) noexcept {
		static_assert(std::is_invocable_r_v<ReturnType, Fun, Args...>,
			"argument is not invocable with specified signature.");

		object_ = std::addressof(fun);
		call_ = &function_ref::call<Fun>;
	}

	constexpr void swap(function_ref& other) noexcept {
		std::swap(object_, other.object_);
		std::swap(call_, other.call_);
	}

	constexpr explicit operator bool() const noexcept { return object_; }

	/// �����֐��I�u�W�F�N�g���w���Ă��邩�ǂ����̔�r
	constexpr bool operator==(const function_ref& other) const noexcept {
		return (object_ == other.object_);
	}

	/// ���b�p�[�֐����Ăяo��
	ReturnType operator()(Args&&... args) const 
	{
		return call_(object_, std::forward<Args>(args)...);
	}
};