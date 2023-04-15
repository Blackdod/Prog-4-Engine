#pragma once
#include <memory>
#include <map>
#include "Controllers.h"
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
		~InputManager() = default;
		bool ProcessInput(float deltaTime);

		void AssignButtonToCommand(	unsigned int controllerIdx,
									ControllerButton button,
									std::unique_ptr<Command>&& command,
									InputType inputType);

		void AssignButtonToCommand(SDL_Scancode button,
									std::unique_ptr<Command>&& command);

	private:
		std::vector<std::unique_ptr<XBox360Controller>> m_pGamePads{};

		std::map<std::pair<unsigned, ControllerButton>, std::pair<std::unique_ptr<Command>, InputType>> m_Commands;
		std::map< SDL_Scancode, std::unique_ptr<Command>> m_KeyboardCommands;
	};

}
