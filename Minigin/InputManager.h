#pragma once
#include <memory>
#include <map>
#include <SDL_scancode.h>

#include "Controllers.h"
#include "Singleton.h"
#include "Command.h"

namespace dae
{
	

	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class MouseButton
		{
			Left, Middle, Right
		};

		enum class KeyState
		{
			Down,
			Pressed,
			Up
		};

		struct KeyAction
		{
			KeyState state{};
			std::shared_ptr<Command> command{};
			int playerIdx{};

			XBox360Controller::Button controllerButton{};
			SDL_Scancode key{};
		};

		InputManager() = default;
		~InputManager();
		bool ProcessInput(float deltaT);
		void Update();

		int AddPlayer();


		bool IsPressed(XBox360Controller::Button button, int playerIdx) const;
		bool IsDownThisFrame(XBox360Controller::Button button, int playeIdx) const;
		bool IsUpThisFrame(XBox360Controller::Button button, int playerIdx) const;
		glm::ivec2 GetMousePos()const { return m_MousePos; };
		bool IsMousePressed() const { return m_isMousePressed; };

		void AddCommand(XBox360Controller::Button button, SDL_Scancode keyboardButton, std::shared_ptr<Command> command, int playerIdx, KeyState state = KeyState::Down);

		XBox360Controller& GetPlayer(int idx);

	private:

		const Uint8* m_pCurrentState{};
		Uint8* m_pPreviousState{};

		std::vector<std::unique_ptr<XBox360Controller>> m_pControllers{};
		std::vector<KeyAction*> m_KeyCommands{};

		glm::ivec2 m_MousePos{};
		bool m_isMousePressed{ false };
	};

}
