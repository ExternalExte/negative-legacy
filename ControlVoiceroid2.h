#pragma once
#include"property.h"
#include<memory>
#include<string_view>
#include<sstream>
#include<chrono>

namespace ControlVoiceroid2ForCpp
{
	struct UIAutomationData;
	class Voiceroid2Controller
	{
	public:
		Voiceroid2Controller();
		~Voiceroid2Controller();
	private:
		std::unique_ptr<UIAutomationData> Data;
		bool GetHandle();//『既に起動している』VOICEROID2のウィンドウハンドル取得
		bool StartVoiceroid2();//pathからVoiceroid2Editor起動
		bool CanonicalizationHandle();
		bool GetWindowAutomationElement();
		bool GetUIElements();
		void set_textBox(const std::wstring& text);
		enum class ButtonCode
		{
			play, stop, begin, end, save, playtime
		};
		bool PushButton(ButtonCode code);
	public:
		void Initialize(std::wstring_view editorPath);
		set_only_property<std::wstring, &Voiceroid2Controller::set_textBox>
			textBox{ *this };
		void say(std::wstring quote);
		std::chrono::milliseconds getPlaytime();
		void stop();
		void sayAndWait(std::wstring quote);

	};

	struct Voiceroid2StreamBuf :public std::wstringbuf
	{
		std::reference_wrapper<Voiceroid2Controller> controller;
		Voiceroid2StreamBuf(std::reference_wrapper<Voiceroid2Controller> controller) :std::wstringbuf(), controller(controller) {}
		int sync()override
		{
			controller.get().sayAndWait(str());
			return 0;
		}
	};
};