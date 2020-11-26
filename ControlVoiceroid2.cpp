#include"ControlVoiceroid2.h"
#include<exception>
#include<future>
#include<Windows.h>
#include<UIAutomation.h>
#include<iostream>
namespace ControlVoiceroid2ForCpp
{
#define throwLocation() throw std::exception(("LINE:"+std::to_string(__LINE__)+" FILE:"+__FILE__).c_str())

	struct UIAutomationData
	{
		HWND voiceroid2_hWnd = nullptr;
		IUIAutomation* automationMaster = nullptr;
		IUIAutomationElement* windowElement = nullptr;
		std::wstring_view editorPath;
		IUIAutomationValuePattern* textBox = nullptr;
		IUIAutomationElementArray* buttons = nullptr;
		IUIAutomationElement* subwindowElement = nullptr;
	};

	Voiceroid2Controller::Voiceroid2Controller()
	{
		Data = std::make_unique<UIAutomationData>();
		static bool call_once = true;
		if (call_once)
		{
			CoInitialize(NULL);
			CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&Data->automationMaster));
			call_once = false;
		}
	}

	Voiceroid2Controller::~Voiceroid2Controller()
	{
		CoUninitialize();
	}

	bool Voiceroid2Controller::GetHandle()
	{
		auto&& hWnd = Data->voiceroid2_hWnd;
		if (hWnd != NULL)
			return true;

		if (hWnd = FindWindowA(NULL, "VOICEROID2"); hWnd == NULL)
			if (hWnd = FindWindowA(NULL, "VOICEROID2*"); hWnd == NULL)
				return false;
		return true;
	}

	bool Voiceroid2Controller::StartVoiceroid2()
	{
		STARTUPINFOW sInfo{};
		PROCESS_INFORMATION pInfo{};
		if (!CreateProcessW(Data->editorPath.data(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &sInfo, &pInfo))
			return false;
		else
			return CloseHandle(pInfo.hThread) && CloseHandle(pInfo.hProcess);
	}

	bool Voiceroid2Controller::CanonicalizationHandle()
	{
		if (this->GetHandle())
			return true;

		auto dispatch = std::async(
			[this]
			{
				Voiceroid2Controller::StartVoiceroid2();
				auto&& hWnd = Data->voiceroid2_hWnd;
				do {
					hWnd = FindWindowA(NULL, "VOICEROID2");
				} while (hWnd == NULL);
			});

		using namespace std::chrono_literals;
		return dispatch.wait_for(20s) != std::future_status::timeout;
	}

	bool Voiceroid2Controller::GetWindowAutomationElement()
	{
		return Data->automationMaster->ElementFromHandle(Data->voiceroid2_hWnd, &Data->windowElement) == S_OK
			&& Data->windowElement != nullptr;
	}
	bool Voiceroid2Controller::GetUIElements()
	{
		if (!GetWindowAutomationElement())
			return false;
		auto&& automationMaster = Data->automationMaster;
		auto&& automationElement = Data->windowElement;

		//ウィンドウのUIを取得していく
		VARIANT searchText;//検索用の文字列(が入ったVARIANT)
		searchText.vt = VT_BSTR;
		searchText.bstrVal = SysAllocString(L"c");
		IUIAutomationCondition* condition;//作成された条件へのポインタ
		//condition,searchTextは使い回す

		// 再生ボタン、テキストボックスが配置されているコンテナの名前は"c"
		IUIAutomationElement* customC;
		if (automationMaster->CreatePropertyCondition(UIA_AutomationIdPropertyId, searchText, &condition) != S_OK
			|| automationElement->FindFirst(TreeScope::TreeScope_Children, condition, &customC) != S_OK
			|| customC == nullptr)
			return false;

		// テキストボックスを取得
		IUIAutomationElement* textBoxElement;
		SysFreeString(searchText.bstrVal);//開放しとく
		searchText.bstrVal = SysAllocString(L"TextBox");
		if (automationMaster->CreatePropertyCondition(UIA_AutomationIdPropertyId, searchText, &condition) != S_OK
			|| customC->FindFirst(TreeScope::TreeScope_Children, condition, &textBoxElement) != S_OK
			|| textBoxElement == nullptr)
			return false;

		auto&& textBox = Data->textBox;
		if (textBoxElement->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&textBox)) != S_OK)
			return false;

		SysFreeString(searchText.bstrVal);
		searchText.bstrVal = SysAllocString(L"ボタン");
		if (automationMaster->CreatePropertyCondition(UIA_LocalizedControlTypePropertyId, searchText, &condition) != S_OK
			|| customC->FindAll(TreeScope::TreeScope_Children, condition, &Data->buttons) != S_OK
			|| Data->buttons == nullptr)
			return false;
		return true;
	}

	void Voiceroid2Controller::set_textBox(const std::wstring& text)
	{
		Data->textBox->SetValue(SysAllocString(text.data()));
	}
	void Voiceroid2Controller::Initialize(std::wstring_view path)
	{
		Data->editorPath = path;
		if (!CanonicalizationHandle() || !GetUIElements())
			throwLocation();
	}
	bool Voiceroid2Controller::PushButton(ButtonCode code)
	{
		int index = static_cast<int>(code);
		IUIAutomationElement* button;
		if (Data->buttons->GetElement(index, &button) != S_OK)
			return false;
		IUIAutomationInvokePattern* push;
		if (button->GetCurrentPatternAs(UIA_InvokePatternId, IID_PPV_ARGS(&push)) != S_OK)
			return false;
		if (push->Invoke() != S_OK)
			return false;
		return true;
	}
	void Voiceroid2Controller::say(std::wstring quote)
	{
		set_textBox(quote);
		PushButton(ButtonCode::play);
	}

	std::chrono::milliseconds Voiceroid2Controller::getPlaytime()
	{
		using namespace std::chrono_literals;
		PushButton(ButtonCode::playtime);
		HWND infohWnd;
		auto dispatch = std::async(
			[&]() {
				do {
					infohWnd = FindWindowA(NULL, "情報");
				} while (infohWnd == NULL);
			}
		);
		IUIAutomationElement* infoWindow;
		if (dispatch.wait_for(3s) == std::future_status::timeout
			|| Data->automationMaster->ElementFromHandle(infohWnd, &infoWindow) != S_OK
			|| infoWindow == NULL)
			throwLocation();
		VARIANT searchText;
		searchText.vt = VT_BSTR;
		searchText.bstrVal = SysAllocString(L"65535");
		IUIAutomationCondition* condition;
		IUIAutomationElement* infoElement;
		if (Data->automationMaster->CreatePropertyCondition(UIA_AutomationIdPropertyId, searchText, &condition) != S_OK
			|| infoWindow->FindFirst(TreeScope::TreeScope_Children, condition, &infoElement) != S_OK)
			throwLocation();
		BSTR info;
		infoElement->get_CurrentName(&info);
		auto result = std::chrono::milliseconds(int(std::stod(std::wstring(info).substr(6, 5)) * 1000));
		IUIAutomationElement* okButton;
		SysFreeString(searchText.bstrVal);
		searchText.bstrVal = SysAllocString(L"2");
		if (Data->automationMaster->CreatePropertyCondition(UIA_AutomationIdPropertyId, searchText, &condition) != S_OK
			|| infoWindow->FindFirst(TreeScope::TreeScope_Children, condition, &okButton) != S_OK)
			throwLocation();
		IUIAutomationInvokePattern* push;

		okButton->GetCurrentPatternAs(UIA_InvokePatternId, IID_PPV_ARGS(&push));
		push->Invoke();
		return result;
	}
	void Voiceroid2Controller::stop()
	{
		PushButton(ButtonCode::stop);
	}
	void Voiceroid2Controller::sayAndWait(std::wstring quote)
	{
		set_textBox(quote);
		auto waittime = getPlaytime();
		PushButton(ButtonCode::play);
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(waittime + 100ms);
	}
#undef throwLocation
}