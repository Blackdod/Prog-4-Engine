#pragma once
#include <vector>

#include "Components.h"
#include "Observer.h"
//#define MAX_AMOUNT_OF_PLAYERS 2

namespace dae {
	class TextComponent;
	class ScoreBoardComponent final : public Component, public Observer
	{
	public:
		ScoreBoardComponent(GameObject* pOwner, const std::shared_ptr<Subject>& subject, int maxLives = 3);

		void OnNotify(const GameObject& gameObject, Event event, int optionalValue = 0) override;

	private:
		//void PlayerDied(const GameObject& gameObject);
		void UpdateLives(const GameObject& gameObject, int idx);

		int m_playerLives;
	};
}