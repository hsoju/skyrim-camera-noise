#pragma once

#include "CameraNoiseManager.h"

class ENBHandler
{
public:
	static void RefreshUI();
	static void MessageHandler(SKSE::MessagingInterface::Message* a_msg);
};
