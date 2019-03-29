#include <stdafx.h>
#include <string>
#include <event/game/PlayerEvents.h>
#include "ModEvents.h"
#include "ModInfo.h"

void* request_data(EventType type) {
	auto pointer = (void*(WINAPI*)(EventType))get_function((HMODULE)module, "request_data_other");
	return pointer(type);
}

void run_event(EventType type, std::vector<void*>& data) {
	auto pointer = (void(WINAPI*)(EventType, std::vector<void*>&))get_function((HMODULE)module, "request_run_event");
	if (pointer == NULL) {
		log(LogType::Error, "Pointer is null");
		return;
	}
	pointer(type, data);
}

// custom events
void suicide() {
	//void* controller = request_data(EventType::PlayerBeginPlay);
	if (localPlayerController == NULL) {
		log(LogType::Error, "Local Controller was NULL when requested.");
		return;
	}

	auto args = std::vector<void*>{
		localPlayerController
	};

	// runs a mod loader function
	run_event(EventType::PlayerSuicide, args);
}

void get_player() {
	if (worldInstance == NULL) {
		log(LogType::Error, "World Instance was NULL when requested.");
		return;
	}

	auto args = std::vector<void*>{
		worldInstance
	};

	// runs a mod loader function
	run_event(EventType::GameGetPlayer, args);
}

// global events
EXTERN_DLL_EXPORT void player_begin_play_event(std::vector<void*>& args) {
	localPlayerController = args[0];
}

EXTERN_DLL_EXPORT void game_instance_get_world_event(std::vector<void*>& args) {
	worldInstance = args[0];
}

EXTERN_DLL_EXPORT void player_sent_message_event(std::vector<void*>& args) {
	get_player();
}