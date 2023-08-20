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
#include "Displays.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "LevelCreator.h"

void load()
{
	using namespace dae;

	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// PACMAN
	float playX{ 312.f }, playY{ 296.2f };
	InputManager::GetInstance().AddPlayer();
	auto pacman_go = std::make_shared<GameObject>();
	Transform* transform = &pacman_go->AddComponent<Transform>();
	transform->SetPosition({ playX, playY, 0.f });
	pacman_go.get()->SetLocalPosition(glm::vec3{playX, playY, 0.f});
	pacman_go.get()->SetStartPosition(glm::vec3{playX, playY, 0.f});
	RenderComponent* pacRender = &pacman_go->AddComponent<RenderComponent>();
	pacRender->SetTexture("pacman.png");
	PlayerComponent* playerComponent = &pacman_go->AddComponent<PlayerComponent>(false, 0);
	ColliderComponent* playerCollider = &pacman_go->AddComponent<ColliderComponent>("PLAYER");
	playerCollider->SetDimensions(15.2f, 15.2f);
	playerCollider->SetPosition(playX, playY);

	// LIVES
	SDL_Color textColor{ 255,255,255 };
	auto livesFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto scoreFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

	const auto livesdisplay = std::make_shared<GameObject>();
	transform = &livesdisplay->AddComponent<Transform>();
	transform->SetPosition({ 0.f, 15.f, 0.f });
	livesdisplay.get()->SetLocalPosition(glm::vec3{35.f, 15.f, 0.f});

	// Make sure to add the render component before the text component!
	livesdisplay->AddComponent<RenderComponent>();
	livesdisplay->AddComponent<TextComponent>("Lives: ", livesFont, textColor);
	livesdisplay->AddComponent<LivesDisplayComponent>(pacman_go.get());
	scene.Add(livesdisplay);

	// SCORES
	auto scoreDisplay = std::make_shared<GameObject>();
	transform = &scoreDisplay->AddComponent<Transform>();
	transform->SetPosition({ 300.f, 15.f, 0.f });
	scoreDisplay.get()->SetLocalPosition(glm::vec3{300.f, 15.f, 0.f});
	
	scoreDisplay->AddComponent<RenderComponent>();
	scoreDisplay->AddComponent<TextComponent>("Score: ", scoreFont, textColor);
	scoreDisplay->AddComponent<ScoreDisplayComponent>(pacman_go.get());
	scene.Add(scoreDisplay);

	playerComponent->AddObserver(livesdisplay->GetComponent<LivesDisplayComponent>());
	playerComponent->AddObserver(scoreDisplay->GetComponent<ScoreDisplayComponent>());
	playerComponent->Start();

	const auto moveUp = std::make_shared<MoveCommand>(pacman_go.get(), Direction::UP);
	const auto moveDown = std::make_shared<MoveCommand>(pacman_go.get(), Direction::DOWN);
	const auto moveRight = std::make_shared<MoveCommand>(pacman_go.get(), Direction::RIGHT);
	const auto moveLeft = std::make_shared<MoveCommand>(pacman_go.get(), Direction::LEFT);

	const auto dieCommand = std::make_shared<DieCommand>(pacman_go.get());

	// UP
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadUp, SDL_SCANCODE_W, moveUp, 0, InputManager::KeyState::Down);
	// DOWN
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadDown, SDL_SCANCODE_S, moveDown, 0, InputManager::KeyState::Down);
	// RIGHT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadRight, SDL_SCANCODE_D, moveRight, 0, InputManager::KeyState::Down);
	// LEFT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadLeft, SDL_SCANCODE_A, moveLeft, 0, InputManager::KeyState::Down);

	// DIE
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::ButtonA, SDL_SCANCODE_E, dieCommand, 0, InputManager::KeyState::Down);

	scene.Add(pacman_go);

	LevelCreator::GetInstance().CreateLevel(L"../Data/level1.json", &scene);
	LevelCreator::GetInstance().SetPlayer1(pacman_go.get());

	// BLINKY
	float blinkX{ 312.f }, blinkY{ 256.f };
	InputManager::GetInstance().AddPlayer();
	auto blinky_go = std::make_shared<GameObject>();
	transform = &blinky_go->AddComponent<Transform>();
	transform->SetPosition({ blinkX, blinkY, 0.f });
	blinky_go.get()->SetLocalPosition(glm::vec3{blinkX, blinkY, 0.f});
	blinky_go.get()->SetStartPosition(glm::vec3{blinkX, blinkY, 0.f});
	RenderComponent* blinkyRender = &blinky_go->AddComponent<RenderComponent>();
	blinkyRender->SetTexture("blinky.png");
	&blinky_go->AddComponent<BlinkyComponent>(pacman_go.get(), 0);
	ColliderComponent* blinkyCollider = &blinky_go->AddComponent<ColliderComponent>("GHOST");
	blinkyCollider->SetDimensions(15.2f, 15.2f);
	blinkyCollider->SetPosition(blinkX, blinkY);
	scene.Add(blinky_go);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}
