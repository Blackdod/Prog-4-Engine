#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"
#include "InputManager.h"

using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	//BACKGROUND

	auto go = std::make_shared<GameObject>();
	go->AddComponent<Transform>();
	go->AddComponent<RenderComponent>();
	go->SetLocalPosition(glm::vec3( 320.f , 240.f, 0.f));
	go->GetComponent<RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	//LOGO

	go = std::make_shared<GameObject>();
	go->AddComponent<Transform>();
	go->AddComponent<RenderComponent>();
	go->SetLocalPosition(glm::vec3(320.f, 200.f, 0.f));
	go->GetComponent<RenderComponent>()->SetTexture("logo.tga");
	scene.Add(go);

	//TITLE
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	
	const auto to = std::make_shared<GameObject>();
	to->AddComponent<Transform>();
	to->AddComponent<RenderComponent>();
	to->AddComponent<TextComponent>( "Programming 4 Assignment", font);
	to->SetLocalPosition(glm::vec3(320.f, 150.f, 0.f));
	scene.Add(to);

	//FPS COUNTER
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	const auto fps = std::make_shared<GameObject>();
	fps->AddComponent<Transform>();
	fps->AddComponent<RenderComponent>();
	fps->AddComponent<TextComponent>("0", font);
	fps->AddComponent<FPSComponent>();
	fps->SetLocalPosition( glm::vec3(40.f, 15.f, 0.f));
	//fps->GetText()->SetColor({ 0, 255, 0 });
	scene.Add(fps);

	//Moving gameObjects (sceneGraph)
	//const auto circleCenter = std::make_shared<GameObject>();
	//circleCenter->AddComponent<Transform>();
	//circleCenter->SetLocalPosition(glm::vec3(320.f, 350.f, 0.f));
	//scene.Add(circleCenter);

	const auto digger = std::make_shared<GameObject>();
	digger->AddComponent<Transform>();
	digger->AddComponent<RenderComponent>();
	//digger->AddComponent<Encircle>(20.f, 20.f, false);
	digger->GetComponent<RenderComponent>()->SetTexture("SingleDriller.png");
	digger->SetLocalPosition(glm::vec3(200.f, 300.f, 0.f));
	scene.Add(digger);
	//digger->SetParent(circleCenter.get());

	const auto hoggin = std::make_shared<GameObject>();
	hoggin->AddComponent<Transform>();
	hoggin->AddComponent<RenderComponent>();
	//hoggin->AddComponent<Encircle>(20.f, 5.f, false);
	hoggin->GetComponent<RenderComponent>()->SetTexture("SingleHogging.png");
	hoggin->SetLocalPosition(glm::vec3(400.f, 300.f, 0.f));
	scene.Add(hoggin);
	//hoggin->SetParent(digger.get());

	//ImGUI

	//const auto ImGUI = std::make_shared<dae::GameObject>();
	//ImGUI->AddComponent<dae::ImGUIComponenent>();
	//scene.Add(ImGUI);

	//Input

	auto& input = InputManager::GetInstance();
	const unsigned int controllerOne{ 0 };
	const unsigned int controllerTwo{ 1 };
	const float speed{ 100.f };

	// player 1
	input.AssignButtonToCommand(controllerOne, XBox360Controller::ControllerButton::DPadDown,
		new MoveCommand(digger, speed, glm::f32vec2{ 0.f, 1.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerOne, XBox360Controller::ControllerButton::DPadUp,
		new MoveCommand(digger, speed, glm::f32vec2{ 0.f, -1.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerOne, XBox360Controller::ControllerButton::DPadLeft,
		new MoveCommand(digger, speed, glm::f32vec2{ -1.f, 0.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerOne, XBox360Controller::ControllerButton::DPadRight,
		new MoveCommand(digger, speed, glm::f32vec2{ 1.f, 0.f }), InputType::pressed);

	// player 1
	input.AssignButtonToCommand(controllerTwo, XBox360Controller::ControllerButton::DPadDown,
		new MoveCommand(hoggin, speed * 2, glm::f32vec2{ 0.f, 1.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerTwo, XBox360Controller::ControllerButton::DPadUp,
		new MoveCommand(hoggin, speed * 2, glm::f32vec2{ 0.f, -1.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerTwo, XBox360Controller::ControllerButton::DPadLeft,
		new MoveCommand(hoggin, speed * 2, glm::f32vec2{ -1.f, 0.f }), InputType::pressed);
	input.AssignButtonToCommand(controllerTwo, XBox360Controller::ControllerButton::DPadRight,
		new MoveCommand(hoggin, speed * 2, glm::f32vec2{ 1.f, 0.f }), InputType::pressed);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}