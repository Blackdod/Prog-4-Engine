#pragma once
#include <memory>
#include <map>
#include "XBox360Controller.h"
#include "Singleton.h"
#include "Command.h"

namespace dae
{
	enum class InputType
	{
		down,
		up,
		pressed
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		bool ProcessInput(float deltaTime);

		void AssignButtonToCommand(	unsigned int controllerIdx,
									XBox360Controller::ControllerButton button,
									Command* command,
									InputType inputType);
	private:
		XBox360Controller* m_pXboxController = nullptr;
		InputType m_InputType = InputType::up;
		std::vector<std::unique_ptr<XBox360Controller>> m_pControllers{};

		std::map<std::pair<unsigned, XBox360Controller::ControllerButton>, std::pair<std::unique_ptr<Command>, InputType>> m_Commands;
	};

}
