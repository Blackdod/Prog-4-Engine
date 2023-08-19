#pragma once
#include <memory>

#include "SoundSystems.h"

class ServiceLocator final
{
	static std::unique_ptr<SoundSystem> _ss_instance;
public:
	static SoundSystem& GetSoundSystem() { return *_ss_instance; }
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
	{
		if(ss == nullptr)
		{
			_ss_instance = std::make_unique<NullSoundSystem>();
		}
		else
		{
			_ss_instance = std::move(ss);
		}
	}
};

