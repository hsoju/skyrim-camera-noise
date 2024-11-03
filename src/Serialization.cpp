#include "Serialization.h"

#include "CameraNoiseManager.h"

namespace Serialization
{
	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{

	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		CameraNoiseManager::GetSingleton()->ResetINIs();
	}

	void RevertCallback(SKSE::SerializationInterface*)
	{
		CameraNoiseManager::GetSingleton()->ResetINIs();
	}
}
