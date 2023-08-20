#pragma once
#include "Command.h"
#include "GameComponents.h"

class DieCommand : public dae::Command
{
public:
	DieCommand(dae::GameObject* pOwner) : Command(), m_pOwner(pOwner){};
	virtual void Execute([[maybe_unused]] float deltaTime) override;
	virtual void Undo([[maybe_unused]] float deltaTime) override;
private:
	dae::GameObject* m_pOwner{};
};

class MoveCommand : public dae::Command
{
public:
	MoveCommand(dae::GameObject* pOwner, Direction dir) : Command(), m_Dir{ dir }, m_pOwner(pOwner) {};
	virtual void Execute([[maybe_unused]] float deltaTime) override;
	virtual void Undo([[maybe_unused]] float deltaTime) override;
private:
	Direction m_Dir{};
	Direction m_PreviousDirection{};
	dae::GameObject* m_pOwner{};
};

class StartCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;
	virtual void Undo([[maybe_unused]] float deltaTime) override;

	void Start(PlayerComponent* player);
};

class JoinMsPacmanCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;
	virtual void Undo([[maybe_unused]] float deltaTime) override = 0;

	void JoinMsPacman(PlayerComponent* player);
};

class JoinGhostCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;
	virtual void Undo([[maybe_unused]] float deltaTime) override = 0;

	void JoinGhost(PlayerComponent* player);
};