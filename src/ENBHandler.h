#pragma once
#include "ENB/ENBSeriesAPI.h"

#include "CameraNoiseManager.h"

class ENBHandler
{
public:
	static void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kPostLoad:
			ENB_API::ENBSDKALT1001* g_ENB = reinterpret_cast<ENB_API::ENBSDKALT1001*>(ENB_API::RequestENBAPI(ENB_API::SDKVersion::V1001));
			if (g_ENB) {
				logger::info("Obtained ENB API");
				g_ENB->SetCallbackFunction([](ENBCallbackType calltype) {
					switch (calltype) {
					case ENBCallbackType::ENBCallback_PostLoad:
						CameraNoiseManager::GetSingleton()->RefreshUI();
						CameraNoiseManager::GetSingleton()->LoadINI();
						break;
					case ENBCallbackType::ENBCallback_PreSave:
						CameraNoiseManager::GetSingleton()->SaveINI();
						break;
					case ENBCallbackType::ENBCallback_PostReset:
						CameraNoiseManager::GetSingleton()->RefreshUI();
						break;
					}
				});
			} else
				logger::info("Unable to acquire ENB API");
			break;
		}
	}
};
