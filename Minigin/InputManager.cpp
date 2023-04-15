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
	InputManager::InputManager()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
		
			dwResult = XInputGetState(i, &state);
		
			if (dwResult == ERROR_SUCCESS)
			{
				m_pGamePads.push_back(std::make_unique<XBox360Controller>(static_cast<unsigned int>(i)));
			}
			else
			{
				std::cout << "There is no controller with idx: " << i << std::endl;
			}
		}
	}

	bool InputManager::ProcessInput(float deltaTime)
	{
		SDL_Event e;
		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		for (auto commandsIterator = m_KeyboardCommands.begin(); commandsIterator != m_KeyboardCommands.end(); ++commandsIterator)
		{
			if(keyState[commandsIterator->first])
			{
				commandsIterator->second->Execute(deltaTime);
			}
		}

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return false;
			}

			if(e.type == SDL_KEYDOWN)
			{
				
			}

			if (e.type == SDL_KEYUP)
			{

			}

			ImGui_ImplSDL2_ProcessEvent(&e);
		}

		for (const auto& controller : m_pGamePads)
		{
			controller->Update();

			for (auto CommandsIterator = m_Commands.begin(); CommandsIterator != m_Commands.end(); ++CommandsIterator)
			{
				if(CommandsIterator->first.first != controller->idx)
				{
					continue;
				}

				switch (CommandsIterator->second.second)
				{
				case InputType::pressed:
					if (controller->IsBeingPressed(CommandsIterator->first.second))
					{
						CommandsIterator->second.first->Execute(deltaTime);
					}
					break;
				case InputType::up:
					if (controller->IsUp(CommandsIterator->first.second))
					{
						CommandsIterator->second.first->Execute(deltaTime);
					}
					break;
				case InputType::down:
					if (controller->IsDown(CommandsIterator->first.second))
					{
						CommandsIterator->second.first->Execute(deltaTime);
					}
					break;
				default:
					break;
				}
			}

			if (controller->IsDown(ControllerButton::LeftShoulder))
			{
				return false;
			}
		}

		return true;
	}

	void InputManager::AssignButtonToCommand(unsigned int controllerIdx, ControllerButton button, std::unique_ptr<Command>&& command, InputType inputType)
	{
		m_Commands[std::make_pair(controllerIdx, button)] = std::make_pair(std::move(command), inputType);
	}

	void InputManager::AssignButtonToCommand(SDL_Scancode button, std::unique_ptr<Command>&& command)
	{
		m_KeyboardCommands[button] = std::move(command);
	}
	
}