#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glm/glm.hpp>
#include <SDL.h>

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Components.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "GameCommands.h"

void load()
{
	using namespace dae;

	// Movement directions
	glm::vec3 up = { 0.f,-1.f,0.f };
	glm::vec3 down = { 0.f,1.f,0.f };
	glm::vec3 right = { 1.f,0.f,0.f };
	glm::vec3 left = { -1.f,0.f,0.f };

	SDL_Color textColor{ 255,255,255 };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto livesFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);

	//// LIVES
	//auto livesdisplay = std::make_shared<GameObject>();
	//auto transform = livesdisplay->AddComponent<Transform>();
	//transform->SetLocalPosition({ 0.f, 15.f, 0.f });
	//
	//// Make sure to add the render component before the text component!
	//auto renderer = livesdisplay->AddComponent<RenderComponent>();
	//auto text_comp = livesdisplay->AddComponent<TextComponent>("Lives: ", livesFont, textColor);
	//text_comp->Init();
	//livesdisplay->AddComponent<LivesDisplayComponent>();
	//scene.Add(livesdisplay);
	//
	//auto scoreDisplay = std::make_shared<GameObject>();
	//transform = scoreDisplay->AddComponent<Transform>();
	//transform->SetLocalPosition({ 150.f, 15.f, 0.f });
	//
	//renderer = scoreDisplay->AddComponent<RenderComponent>();
	//text_comp = scoreDisplay->AddComponent<TextComponent>("Score: ", livesFont, textColor);
	//text_comp->Init();
	//scoreDisplay->AddComponent<ScoreDisplayComponent>();
	//scene.Add(scoreDisplay);

	float playX{ 347.f }, playY{ 60.f };
	InputManager::GetInstance().AddPlayer();
	auto pacman_go = std::make_shared<GameObject>();
	transform = pacman_go->AddComponent<Transform>();
	transform->SetLocalPosition({ playX, playY, 0.f });
	auto pacRender = pacman_go->AddComponent<RenderComponent>();
	pacRender->SetTexture("pacman.png");
	auto playerComponent = pacman_go->AddComponent<PlayerComponent>(false, 0);
	auto playerCollider = pacman_go->AddComponent<ColliderComponent>("PLAYER");
	playerCollider->SetDimensions(playX, playY, 15.2f, 15.2f);

	playerComponent->AddObserver(livesdisplay->GetComponent<LivesDisplayComponent>());
	playerComponent->AddObserver(scoreDisplay->GetComponent<ScoreDisplayComponent>());
	playerComponent->Start();

	auto moveUp = std::make_shared<MoveCommand>(pacman_go.get(), Direction::UP);
	auto moveDown = std::make_shared<MoveCommand>(pacman_go.get(), Direction::DOWN);
	auto moveRight = std::make_shared<MoveCommand>(pacman_go.get(), Direction::RIGHT);
	auto moveLeft = std::make_shared<MoveCommand>(pacman_go.get(), Direction::LEFT);

	auto dieCommand = std::make_shared<DieCommand>(pacman_go.get());

	// UP
	InputManager::GetInstance().AddCommand(Xbox360Controller::Button::DPADUp, SDL_SCANCODE_W, moveUp, 0, InputManager::KeyState::Down);
	// DOWN
	InputManager::GetInstance().AddCommand(Xbox360Controller::Button::DPADDown, SDL_SCANCODE_S, moveDown, 0, InputManager::KeyState::Down);
	// RIGHT
	InputManager::GetInstance().AddCommand(Xbox360Controller::Button::DPADRight, SDL_SCANCODE_D, moveRight, 0, InputManager::KeyState::Down);
	// LEFT
	InputManager::GetInstance().AddCommand(Xbox360Controller::Button::DPADLeft, SDL_SCANCODE_A, moveLeft, 0, InputManager::KeyState::Down);

	// DIE
	InputManager::GetInstance().AddCommand(Xbox360Controller::Button::ButtonA, SDL_SCANCODE_E, dieCommand, 0, InputManager::KeyState::Down);

	scene.Add(pacman_go);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
