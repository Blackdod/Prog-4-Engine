#pragma once
#include "Command.h"
#include "GameComponents.h"

class UpCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void Up(PlayerComponent* player);
};

class DownCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void Down(PlayerComponent* player);
};

class RightCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void Right(PlayerComponent* player);
};

class LeftCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void Left(PlayerComponent* player);
};

class StartCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void Start(PlayerComponent* player);
};

class JoinMsPacmanCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void JoinMsPacman(PlayerComponent* player);
};

class JoinGhostCommand : public dae::Command
{
public:
	void Execute([[maybe_unused]] float deltaTime) override;

	void JoinGhost(PlayerComponent* player);
};