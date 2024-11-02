#pragma once

#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 1,
		kUniqueID = 'CNOI'
	};

	
	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void Save(SKSE::SerializationInterface* a_intfc, std::vector<float>& noise_data, uint32_t label);
	void Load(SKSE::SerializationInterface* a_intfc, std::vector<float>& noise_data);
	void RevertCallback(SKSE::SerializationInterface*);
}
