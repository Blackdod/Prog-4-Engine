#include "Displays.h"

#include "GameComponents.h"

#pragma region LivesDisplay

LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* go, dae::GameObject* player)
	:Component(go)
	, m_pTextComponent{ GetOwner()->GetComponent<dae::TextComponent>() }
	,m_Text{ "Lives: " }
	,m_pPlayer(player)
{
}

LivesDisplayComponent::~LivesDisplayComponent()
{
	PlayerComponent* pPlayerComp = m_pPlayer->GetComponent<PlayerComponent>();
	pPlayerComp->RemoveObserver(this);
}


void LivesDisplayComponent::Update([[maybe_unused]] float deltaT)
{
}

void LivesDisplayComponent::Render() const
{
}


void LivesDisplayComponent::HandleEvent(Event event)
{
	switch (event)
	{
	case Event::PlayerStart:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetComponent<PlayerComponent>()->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	case Event::PlayerDied:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetComponent<PlayerComponent>()->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	default:
		break;
	}
}

void LivesDisplayComponent::OnSubjectDestroy()
{

}

#pragma endregion

#pragma region ScoreDisplay

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* go, dae::GameObject* player)
	:Component(go)
	,pTextComponent{ GetOwner()->GetComponent<dae::TextComponent>() }
	,m_Text{ "SCORE: " }
	,m_pPlayer(player)
{
}

ScoreDisplayComponent::~ScoreDisplayComponent()
{
	PlayerComponent* pPlayerComp = m_pPlayer->GetComponent<PlayerComponent>();
	pPlayerComp->RemoveObserver(this);
}

void ScoreDisplayComponent::Update([[maybe_unused]] float deltaT)
{
}

void ScoreDisplayComponent::Render() const
{
}

void ScoreDisplayComponent::HandleEvent(Event event)
{
	switch (event)
	{
	case Event::AddPoints:
		m_Score += 10;
		m_Text = "SCORE: " + std::to_string(m_Score);
		pTextComponent->SetText(m_Text);
		break;
	case Event::PlayerStart:
		m_Text = "SCORE: " + std::to_string(m_Score);
		pTextComponent->SetText(m_Text);
	default:
		break;
	}
}

void ScoreDisplayComponent::OnSubjectDestroy()
{

}

#pragma endregion