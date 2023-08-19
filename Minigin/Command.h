#pragma once
#include "GameObject.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Execute([[maybe_unused]] float deltaTime) = 0;
		virtual void Undo([[maybe_unused]] float deltaTime) = 0;
	};
}

