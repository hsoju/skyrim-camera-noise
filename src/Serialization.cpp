#include "Serialization.h"

#include "CameraNoiseManager.h"

namespace Serialization
{
	void Save(SKSE::SerializationInterface* a_intfc, std::vector<float>& noise_data, uint32_t label)
	{
		if (!a_intfc->OpenRecord(label, 1)) {
			logger::error("Failed to open record for noise data.");
		} else {
			std::size_t size = noise_data.size();
			if (!a_intfc->WriteRecordData(size)) {
				logger::error("Failed to write size for noise data.");
			} else {
				for (float& value : noise_data) {
					if (!a_intfc->WriteRecordData(value)) {
						logger::error("Failed to write value(s) for noise data.");
						break;
					}
				}
			}
		}
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<float> _data = CameraNoiseManager::GetSingleton()->GetData();
		Save(a_intfc, _data, 'ARR_');

		if (CameraNoiseManager::GetSingleton()->bInterpolation) {
			if (!a_intfc->WriteRecord('ITB_', 1, CameraNoiseManager::GetSingleton()->bInterpolation)) {
				logger::error("Failed to write interpolation.");
			}
			std::vector<float> _interp_data = CameraNoiseManager::GetSingleton()->GetData(true);
			Save(a_intfc, _interp_data, 'ITR_');
		}

		std::unordered_set<std::string> inis = CameraNoiseManager::GetSingleton()->inis;
		if (!a_intfc->OpenRecord('INI_', 1)) {
			logger::error("Failed to open record for loaded inis.");
		} else {
			std::size_t size = inis.size();
			if (!a_intfc->WriteRecordData(size)) {
				logger::error("Failed to write size for loaded inis.");
			} else {
				const std::size_t c_size = sizeof(char);
				for (auto& a_ini : inis) {
					const char* c_data = a_ini.c_str();
					std::size_t iniLength = strlen(c_data);
					if (!a_intfc->WriteRecordData(iniLength)) {
						logger::error("Failed to write length of ini file path.");
					} else {
						if (!a_intfc->WriteRecordData(c_data, iniLength * c_size)) {
							logger::error("Failed to write ini.");
						}
					}
				}
			}
		}
	}

	void Load(SKSE::SerializationInterface* a_intfc, std::vector<float>& noise_data)
	{
		std::size_t size;
		if (!a_intfc->ReadRecordData(size)) {
			logger::error("Failed to load size of noise data.");
		} else {
			for (std::uint32_t i = 0; i < size; i++) {
				float value;
				if (!a_intfc->ReadRecordData(value)) {
					logger::error("Failed to load value(s) of noise data.");
				} else {
					noise_data.push_back(value);
				}
			}
		}
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<float> _data;
		bool was_interpolating = false;
		std::vector<float> _interp_data;
		std::unordered_set<std::string> _inis;

		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			switch (type) {
			case 'ARR_':
				Load(a_intfc, _data);
				break;
			case 'ITB_':
				if (!a_intfc->ReadRecordData(was_interpolating)) {
					logger::error("Failed to load interpolation.");
				}
				break;
			case 'ITR_':
				Load(a_intfc, _interp_data);
				break;
			case 'INI_':
				std::size_t iniSize;
				if (!a_intfc->ReadRecordData(iniSize)) {
					logger::error("Failed to load size of loaded inis.");
				} else {
					const std::size_t c_size = sizeof(char);
					for (std::uint32_t i = 0; i < iniSize; i++) {
						std::size_t a_iniLength;
						if (!a_intfc->ReadRecordData(a_iniLength)) {
							logger::error("Failed to load length of ini file path.");
						} else {
							char* a_ini = new char[a_iniLength];
							if (!a_intfc->ReadRecordData(a_ini, a_iniLength * c_size)) {
								logger::error("Failed to load ini.");
							} else {
								a_ini[a_iniLength] = '\0';
								std::string str_ini = std::string(a_ini);
								delete[] a_ini;
								_inis.insert(str_ini);
							}
						}
					}
				}
				break;
			default:
				logger::error("Unrecognized signature type!");
				break;
			}
		}

		if (!_data.empty() && (!_inis.empty() || was_interpolating)) {
			CameraNoiseManager::GetSingleton()->SetData(_data);
		}

		if (was_interpolating) {
			CameraNoiseManager::GetSingleton()->bInterpolation = true;
			if (!_interp_data.empty()) {
				CameraNoiseManager::GetSingleton()->SetData(_interp_data, true);
			}
		}

		if (!_inis.empty()) {
			CameraNoiseManager::GetSingleton()->inis = std::move(_inis);
		}
	}

	void RevertCallback(SKSE::SerializationInterface*)
	{

	}
}
