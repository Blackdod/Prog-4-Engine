#pragma once
#include "Components.h"
#include "Observer.h"

enum class Event;

class LivesDisplayComponent final : public  dae::Component, public dae::Observer<Event>
{
public:
	LivesDisplayComponent(dae::GameObject* go, dae::GameObject* player);

	~LivesDisplayComponent() override;

	LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent(LivesDisplayComponent&& other) noexcept = delete;
	LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent& operator=(LivesDisplayComponent&& other) noexcept = delete;

	void Update([[maybe_unused]] float deltaT) override;
	void Render() const override;
	void HandleEvent(Event event) override;
	void OnSubjectDestroy() override;

private:
	dae::TextComponent* m_pTextComponent;
	std::string m_Text;
	dae::GameObject* m_pPlayer{};
};

class ScoreDisplayComponent final : public dae::Component, public dae::Observer<Event>
{
public:
	ScoreDisplayComponent(dae::GameObject* go, dae::GameObject* player);

	~ScoreDisplayComponent();

	ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent(ScoreDisplayComponent&& other) noexcept = delete;
	ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) noexcept = delete;

	void Update([[maybe_unused]] float deltaT) override;
	void Render() const override;
	void HandleEvent(Event event) override;
	void OnSubjectDestroy() override;

private:
	dae::TextComponent* pTextComponent;
	std::string m_Text;
	dae::GameObject* m_pPlayer{};
	int m_Score{ 0 };
};
