#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment (lib, "xinput.lib")

#include "Controllers.h"

using namespace dae;

class XBox360Controller::XBox360ControllerImpl
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame{};
	WORD buttonsReleasedThisFrame{};

	unsigned int _controllerIdx;
public:
	explicit XBox360ControllerImpl(unsigned int controllerIdx)
		:_controllerIdx(controllerIdx)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}

	~XBox360ControllerImpl() = default;

	XBox360ControllerImpl(const XBox360ControllerImpl& other) = delete;
	XBox360ControllerImpl(XBox360ControllerImpl&& other) = delete;
	XBox360ControllerImpl& operator=(const XBox360ControllerImpl& other) = delete;
	XBox360ControllerImpl& operator=(XBox360ControllerImpl&& other) = delete;

	void Update()
	{
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		XInputGetState(_controllerIdx, &currentState);

		const auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & ~currentState.Gamepad.wButtons;
	}

	bool IsDownThisFrame(unsigned int button) const { return buttonsPressedThisFrame & button; }
	bool IsUpThisFrame(unsigned int button) const { return buttonsReleasedThisFrame & button; }
	bool IsBeingPressed(unsigned int button) const { return currentState.Gamepad.wButtons & button; }
};

XBox360Controller::XBox360Controller(unsigned int controllerIdx)
	:idx(controllerIdx)
{
	pImpl = new XBox360ControllerImpl(controllerIdx);
}

XBox360Controller::~XBox360Controller()
{
	delete pImpl;
}

void XBox360Controller::Update()
{
	pImpl->Update();
}

bool XBox360Controller::IsDown(ControllerButton button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool XBox360Controller::IsUp(ControllerButton button) const
{
	return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool XBox360Controller::IsBeingPressed(ControllerButton button) const
{
	return pImpl->IsBeingPressed(static_cast<unsigned int>(button));
}

