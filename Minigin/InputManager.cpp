#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include <XInput.h>
#pragma comment (lib, "xinput.lib")

#include "InputManager.h"

#include <iostream>

#include "backends/imgui_impl_sdl2.h"

namespace dae
{
	InputManager::~InputManager()
	{
		for (auto element : m_KeyCommands)
		{
			delete element;
			element = nullptr;
		}
	}

	bool InputManager::ProcessInput(float deltaT)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);

			if (e.type == SDL_QUIT) {
				return false;
			}
			if (e.type == SDL_KEYDOWN) {
				for (const auto& controller : m_KeyCommands)
				{
					if (controller->state == KeyState::Down)
					{
						if (e.key.keysym.scancode == controller->key)
							controller->command.get()->Execute(deltaT);
					}
				}
			}
			if (e.type == SDL_KEYUP)
			{
				for (const auto& controller : m_KeyCommands)
				{
					if (controller->state == KeyState::Up)
					{
						if (e.key.keysym.scancode == controller->key)
							controller->command.get()->Execute(deltaT);
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && m_isMousePressed == false)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
					m_isMousePressed = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
					m_isMousePressed = false;
			}
		}

		Update();

		for (const auto& controller : m_KeyCommands)
		{
			switch (controller->state)
			{
			case KeyState::Down:
				if (IsDownThisFrame(controller->controllerButton, controller->playerIdx))
					controller->command->Execute(deltaT);
				break;
			case KeyState::Pressed:
				if (IsPressed(controller->controllerButton, controller->playerIdx))
					controller->command->Execute(deltaT);
				break;
			case KeyState::Up:
				if (IsUpThisFrame(controller->controllerButton, controller->playerIdx))
					controller->command->Execute(deltaT);
				break;
			default:
				break;
			}
		}

		return true;
	}

	void InputManager::Update()
	{
		for (const auto& controller : m_pControllers)
		{
			controller->Update();
		}
	}

	// returns size of m_pControllers aka the last added player what
	// their number is
	int InputManager::AddPlayer()
	{
		int idx = static_cast<int>(m_pControllers.size());
		m_pControllers.push_back(std::make_unique<XBox360Controller>(idx));
		return idx;
	}

	// returns a player, if the idx is out-of-range it takes the last added player
	XBox360Controller& InputManager::GetPlayer(int idx)
	{
		if (idx <= m_pControllers.size())
		{
			return *m_pControllers.at(idx);
		}
		else
		{
			return *m_pControllers.back();
		}

	}


	bool InputManager::IsPressed(XBox360Controller::Button button, int  playerIdx) const
	{
		return m_pControllers[playerIdx]->IsBeingPressed(button);
	}

	bool InputManager::IsDownThisFrame(XBox360Controller::Button button, int playerIdx) const
	{
		return m_pControllers[playerIdx]->IsDown(button);
	}

	bool InputManager::IsUpThisFrame(XBox360Controller::Button button, int playerIdx) const
	{
		return m_pControllers[playerIdx]->IsUp(button);
	}

	void InputManager::AddCommand(XBox360Controller::Button controllerButton, SDL_Scancode keyboardButton, std::shared_ptr<Command> command, int playerIdx, KeyState state)
	{
		if (playerIdx > m_pControllers.size())
		{
			std::cout << "Player not found, cannot add command\n" << std::endl;
			return;
		}
		KeyAction* action = new KeyAction();
		action->command = command;
		action->controllerButton = controllerButton;
		action->state = state;
		action->playerIdx = playerIdx;
		action->key = keyboardButton;
		m_KeyCommands.emplace_back(action);
	}
	
}