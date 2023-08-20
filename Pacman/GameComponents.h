#pragma once
#include "Components.h"
#include "Enums.h"
#include "Events.h"
#include "Subject.h"

class PlayerComponent : public dae::Component
{
public:
	PlayerComponent(dae::GameObject* owner, bool isGhost, int playerNr);

	void Update(float dt) override;
	void Initialize();
	void SetDirection(Direction direction);

	//bool GhostCollision(glm::vec3 pos);
	bool IsPoweredUp() const { return m_IsPoweredUp; }
	int GetLives() const { return m_Lives; }
	int GetScore() const { return m_Score; }
	bool IsGhost() const { return m_IsGhost; }
	void AddObserver(dae::Observer<Event>* obs);
	void RemoveObserver(dae::Observer<Event>* obs);

	void AddScore(int amount)
	{
		m_Score += amount;
		m_pPlayerSubject->Notify(Event::AddPoints);
	}

	void Start();
	void Die();

	const Direction& GetDirection() const { return m_Direction; }

private:
	void Respawn();
	bool CollidesInTargetDir(float dt);

	Direction m_Direction = Direction::NONE;
	Direction m_TargetDirection = Direction::NONE;

	//score
	int m_Score = 0;
	dae::TextComponent* m_pScoreText = nullptr;
	bool m_IsGhost = false;
	bool m_IsPoweredUp = false;
	float m_PowerupTimer = 0;

	glm::vec3 m_StartPosition = glm::vec3(-50, -50, -50);

	int m_Lives{3};
	std::vector <std::shared_ptr<dae::GameObject>>  m_LivesArr;

	float m_RespawnTimer = 2;
	float m_MovementSpeed = 50.f;

	std::unique_ptr<dae::Subject<Event>> m_pPlayerSubject;

	int m_PlayerNr{};
};

class PacDotComponent : public dae::Component
{
public:
	PacDotComponent(dae::GameObject* owner);
	~PacDotComponent() override;
	void AddPoints();
private:
	int m_PointAmnt{ 10 };
};