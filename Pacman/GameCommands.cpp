#include "GameCommands.h"

void DieCommand::Execute([[maybe_unused]] float deltaTime)
{
	PlayerComponent* pPlayer = m_pOwner->GetComponent<PlayerComponent>();
	if (!pPlayer)
	{
		return;
	}

	pPlayer->Die();
}

void DieCommand::Undo([[maybe_unused]] float deltaTime)
{
	//no undo on this (yet?)
}

void MoveCommand::Execute([[maybe_unused]] float deltaTime)
{
	PlayerComponent* myPlayer = m_pOwner->GetComponent<PlayerComponent>();

	if (!myPlayer)
	{
		return;
	}

	m_PreviousDirection = myPlayer->GetDirection();
	myPlayer->SetDirection(m_Dir);
}

void MoveCommand::Undo([[maybe_unused]] float deltaTime)
{
	PlayerComponent* myPlayer = m_pOwner->GetComponent<PlayerComponent>();

	if (!myPlayer)
	{
		return;
	}

	myPlayer->SetDirection(m_PreviousDirection);
}