#include "ScoreBoard.h"
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"

dae::ScoreBoardComponent::ScoreBoardComponent(GameObject* pOwner, const std::shared_ptr<Subject>& subject, int maxLives)
	:Component(pOwner)
	,m_playerLives(maxLives)
{
	UpdateLives(*pOwner, 0);
	subject->AddObserver(this);
}

void dae::ScoreBoardComponent::OnNotify(const GameObject& gameObject, Event event, int optionalValue)
{
	switch (event)
	{
	case Event::PlayerDied:
		//PlayerDied(gameObject);
		break;
	case Event::UpdateLives:
		UpdateLives(gameObject, optionalValue);
		break;
	default:
		break;
	}
}

void dae::ScoreBoardComponent::UpdateLives(const GameObject& gameObject, int idx)
{
	TextComponent* pText = GetOwner()->GetComponent<TextComponent>();
	if(!pText)
	{
		std::cout << "No TextComponent found\n";
		return;
	}

	std::string textToWrite{};
	if(&gameObject == GetOwner())
	{
		textToWrite += "Lives player " + std::to_string(1) + ": " + std::to_string(m_playerLives);
		
		pText->SetText(textToWrite);
		return;
	}

	if (idx != gameObject.GetComponent<PlayerComponent>()->GetIndex())
	{
		return;
	}
	--m_playerLives;

	textToWrite += "Lives player " + std::to_string(idx + 1) + ": " + std::to_string(m_playerLives);

	pText->SetText(textToWrite);
}
