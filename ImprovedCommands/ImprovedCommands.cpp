#include <fstream>
#include <ArkPluginLibrary/ArkPluginLibrary.h>
#include <ArkPluginLibrary/json.hpp>
#include "API/Ark/Ark.h"
#include "Commands/DestroyAllStructuresForTeamIdCommand.h"
#include "Commands/DestroyDinosForTeamIdCommand.h"
#include "Commands/DestroyMyDinoCommand.h"
#include "Commands/DestroyStructuresForTeamIdAtPositionCommand.h"
#include "Commands/DoRespecCommand.h"
#include "Commands/DoRespecDinoCommand.h"
#include "Commands/FeedDinosForTeamIdCommand.h"
#include "Commands/MyDinoStatsCommand.h"
#include "Commands/DroppedEggsCommand.h"
#include "Commands/NextImprintCommand.h"
#include "Commands/ImprintOnCommand.h"
#include "Commands/SuicideCommand.h"

#pragma comment(lib, "ArkApi.lib")
#pragma comment(lib, "ArkPluginLibrary.lib")
#pragma comment(lib, "Permissions.lib")

nlohmann::json json;

void LoadConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/ImprovedCommands/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->error("Could not open file config.json");
		throw;
	}

	file >> json;
	file.close();
}

void Load()
{
	try
	{
		Log::Get().Init("ImprovedCommands");

		LoadConfig();
		ArkLibrary::LoadCommandsAndFeatures(json);

		auto& commands = ArkApi::GetCommands();

		// chat
		ArkLibrary::AddCommand(CommandName_MyDinoStats_Chat, 
			[&commands](wchar_t* name) { commands.AddChatCommand(name, &MyDinoStatsChatCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyMyDino_Chat, 
			[&commands](wchar_t* name) { commands.AddChatCommand(name, &DestroyMyDinoChatCommand); });
		ArkLibrary::AddCommand(CommandName_Suicide_Chat,
			[&commands](wchar_t* name) { commands.AddChatCommand(name, &SuicideChatCommand); });

		// console
		ArkLibrary::AddCommand(CommandName_DoRespec_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DoRespecConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_DoRespecDino_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DoRespecDinoConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyAllStructuresForTeamId_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DestroyAllStructuresForTeamIdConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyStructuresForTeamIdAtPosition_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DestroyStructuresForTeamIdAtPositionConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyDinosForTeamId_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DestroyDinosForTeamIdConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_DroppedEggs_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &DroppedEggsConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_FeedDinosForTeamId_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &FeedDinosForTeamIdConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_NextImprint_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &NextImprintConsoleCommand); });
		ArkLibrary::AddCommand(CommandName_ImprintOn_Console,
			[&commands](wchar_t* name) { commands.AddConsoleCommand(name, &ImprintOnConsoleCommand); });

		// rcon
		ArkLibrary::AddCommand(CommandName_DoRespec_Rcon, 
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DoRespecRconCommand); });
		ArkLibrary::AddCommand(CommandName_DoRespecDino_Rcon,
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DoRespecDinoRconCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyAllStructuresForTeamId_Rcon, 
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DestroyAllStructuresForTeamIdRconCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyStructuresForTeamIdAtPosition_Rcon, 
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DestroyStructuresForTeamIdAtPositionRconCommand); });
		ArkLibrary::AddCommand(CommandName_DestroyDinosForTeamId_Rcon, 
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DestroyDinosForTeamIdRconCommand); });
		ArkLibrary::AddCommand(CommandName_DroppedEggs_Rcon,
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &DroppedEggsRconCommand); });
		ArkLibrary::AddCommand(CommandName_FeedDinosForTeamId_Rcon, 
			[&commands](wchar_t* name) { commands.AddRconCommand(name, &FeedDinosForTeamIdRconCommand); });
	}
	catch (const std::exception& error)
	{
		Log::GetLog()->error(error.what());
		throw;
	}
}

void Unload()
{
	auto& commands = ArkApi::GetCommands();

	// chat
	ArkLibrary::RemoveCommand(CommandName_MyDinoStats_Chat, 
		[&commands](wchar_t* name) { commands.RemoveChatCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyMyDino_Chat, 
		[&commands](wchar_t* name) { commands.RemoveChatCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_Suicide_Chat,
		[&commands](wchar_t* name) { commands.RemoveChatCommand(name); });

	// console
	ArkLibrary::RemoveCommand(CommandName_DoRespec_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DoRespecDino_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyAllStructuresForTeamId_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyStructuresForTeamIdAtPosition_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyDinosForTeamId_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DroppedEggs_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_FeedDinosForTeamId_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_NextImprint_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_ImprintOn_Console,
		[&commands](wchar_t* name) { commands.RemoveConsoleCommand(name); });

	// rcon
	ArkLibrary::RemoveCommand(CommandName_DoRespec_Rcon, 
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DoRespecDino_Rcon,
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyAllStructuresForTeamId_Rcon, 
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyStructuresForTeamIdAtPosition_Rcon, 
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DestroyDinosForTeamId_Rcon, 
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_DroppedEggs_Rcon,
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
	ArkLibrary::RemoveCommand(CommandName_FeedDinosForTeamId_Rcon, 
		[&commands](wchar_t* name) { commands.RemoveRconCommand(name); });
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}
