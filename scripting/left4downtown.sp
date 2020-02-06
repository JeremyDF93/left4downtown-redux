#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <sdktools>
#undef REQUIRE_EXTENSIONS
#include <left4downtown>

public Plugin myinfo = {
  name = "Left 4 Downtown",
  author = "Kiwi, JeremyDF93",
  description = "Extention for L4D1/2",
  version = LEFT4DOWNTOWN_VERSION,
  url = "http://praisethemoon.com/"
};

enum SDK {
  Handle:SDK_GetTeamScore,
  Handle:SDK_OnBeginRoundSetupTime,
  Handle:SDK_RestartScenarioFromVote,
  Handle:SDK_IsMissionFinalMap,
  Handle:SDK_ResetMobTimer,
  Handle:SDK_NotifyNetworkStateChanged,
  Handle:SDK_SetReservationCookie,
}

EngineVersion g_iSourceEngine;

Handle g_hGameConf;
Handle g_hSDKToolsConf;

Handle g_hSDKCall[SDK];

// Address g_pServer = Address_Null;
Address g_pGameRulesProxy = Address_Null;
Address g_pZombieManager = Address_Null;
Address g_pDirector = Address_Null;

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max) {
  g_iSourceEngine = GetEngineVersion();
  if (g_iSourceEngine != Engine_Left4Dead && g_iSourceEngine != Engine_Left4Dead2) {
    strcopy(error, err_max, "Left 4 Downtown is incompatible with this game.");
    return APLRes_SilentFailure;
  }

  RegPluginLibrary("left4downtown");

  // Shared
  //CreateNative("L4D_GetTeamScore", Native_GetTeamScore);
  //CreateNative("L4D_RestartScenarioFromVote", Native_RestartScenarioFromVote);
  //CreateNative("L4D_LobbyUnreserve", Native_LobbyUnreserve); // TODO
  //CreateNative("L4D_LobbyIsReserved", Native_LobbyIsReserved); // TODO
  //CreateNative("L4D_IsMissionFinalMap", Native_IsMissionFinalMap);
  //CreateNative("L4D_NotifyNetworkStateChanged", Native_NotifyNetworkStateChanged);
  // L4D2
  CreateNative("L4D2_GetCampaignScores", Native_GetCampaignScores); // TODO
  CreateNative("L4D2_ScavengeBeginRoundSetupTime", Native_ScavengeBeginRoundSetupTime); // TODO
  CreateNative("L4D2_GetVersusMaxCompletionScore", Native_GetVersusMaxCompletionScore); // TODO
  CreateNative("L4D2_SetVersusMaxCompletionScore", Native_SetVersusMaxCompletionScore); // TODO
  CreateNative("L4D2_ResetMobTimer", Native_ResetMobTimer);

  return APLRes_Success;
}

public void OnPluginStart() {
  RegAdminCmd("l4do_debug", ConCmd_Debug, ADMFLAG_ROOT);

  g_hGameConf = LoadGameConfigFile("left4downtown.games");
  if (g_hGameConf == INVALID_HANDLE) SetFailState("Error parsing gameconfig file \"left4downtown.games\"");

  g_hSDKToolsConf = LoadGameConfigFile("sdktools.games");
  if (g_hSDKToolsConf == INVALID_HANDLE) SetFailState("Error parsing gameconfig file \"sdktools.games\"");

/*
  g_pServer = GameConfGetAddress(g_hSDKToolsConf, "sv");
  if (g_pServer == Address_Null) SetFailState("Failed to get address of \"sv\"");
*/

  g_pZombieManager = GameConfGetAddress(g_hGameConf, "TheZombieManager");
  if (g_pZombieManager == Address_Null) SetFailState("Failed to get address of \"TheZombieManager\"");

  g_pDirector = GameConfGetAddress(g_hGameConf, "TheDirector");
  if (g_pDirector == Address_Null) SetFailState("Failed to get address of \"TheDirector\"");

  if (g_iSourceEngine == Engine_Left4Dead) PrepLeft4DeadCalls();
  if (g_iSourceEngine == Engine_Left4Dead2) PrepLeft4Dead2Calls();
  PrepSharedCalls();
}

public void OnMapStart() {
  char netClass[255];
  GameConfGetKeyValue(g_hSDKToolsConf, "GameRulesProxy", netClass, sizeof(netClass));
  if (strlen(netClass) == 0) SetFailState("Failed to get key value of \"GameRulesProxy\"");

  int entity = FindEntityByNetClass(GetMaxClients() - 1, netClass);
  if (!IsValidEntity(entity)) SetFailState("Failed to get entity of \"GameRulesProxy\"");

  g_pGameRulesProxy = GetEntityAddress(entity);
  if (g_pGameRulesProxy == Address_Null) SetFailState("Failed to get address of \"GameRulesProxy\"");
}

public int Native_GetCampaignScores(Handle plugin, int numArgs) {}

public int Native_GetTeamScore(Handle plugin, int numArgs) {
  int logical_team = GetNativeCell(1);
  bool campaign_score = GetNativeCell(2);

  return SDKCall(g_hSDKCall[SDK_GetTeamScore], g_pGameRulesProxy, logical_team, campaign_score);
}

public int Native_ScavengeBeginRoundSetupTime(Handle plugin, int numArgs) {
  return SDKCall(g_hSDKCall[SDK_OnBeginRoundSetupTime], g_pDirector);
}

public int Native_RestartScenarioFromVote(Handle plugin, int numArgs) {
  char map[MAX_TARGET_LENGTH]; GetNativeString(1, map, sizeof(map));

  return SDKCall(g_hSDKCall[SDK_RestartScenarioFromVote], g_pDirector, map);
}

public int Native_LobbyUnreserve(Handle plugin, int numArgs) {}

public int Native_LobbyIsReserved(Handle plugin, int numArgs) {}

public int Native_GetVersusMaxCompletionScore(Handle plugin, int numArgs) {}

public int Native_SetVersusMaxCompletionScore(Handle plugin, int numArgs) {}

public int Native_IsMissionFinalMap(Handle plugin, int numArgs) {
  return SDKCall(g_hSDKCall[SDK_IsMissionFinalMap], g_pGameRulesProxy);
}
public int Native_ResetMobTimer(Handle plugin, int numArgs) {
  return SDKCall(g_hSDKCall[SDK_ResetMobTimer], g_pDirector);
}

public int Native_NotifyNetworkStateChanged(Handle plugin, int numArgs) {
  return SDKCall(g_hSDKCall[SDK_NotifyNetworkStateChanged], g_pGameRulesProxy);
}

public Action ConCmd_Debug(int client, int args) {
  PrintToServer("IsMissionFinalMap: %d", L4D_IsMissionFinalMap());
  return Plugin_Handled;
}

void PrepLeft4DeadCalls() {

}

void PrepLeft4Dead2Calls() {
  // OnBeginRoundSetupTime()
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "OnBeginRoundSetupTime");
  g_hSDKCall[SDK_OnBeginRoundSetupTime] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_OnBeginRoundSetupTime] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"OnBeginRoundSetupTime\"");

  // ResetMobTimer()
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "ResetMobTimer");
  g_hSDKCall[SDK_ResetMobTimer] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_ResetMobTimer] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"ResetMobTimer\"");
}

void PrepSharedCalls() {
  // GetTeamScore(int, bool)
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "GetTeamScore");
  PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_Bool, SDKPass_Plain);
  PrepSDKCall_SetReturnInfo(SDKType_PlainOldData, SDKPass_Plain);
  g_hSDKCall[SDK_GetTeamScore] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_GetTeamScore] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"GetTeamScore\"");

  // RestartScenarioFromVote(char const*)
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "RestartScenarioFromVote");
  PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
  g_hSDKCall[SDK_RestartScenarioFromVote] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_RestartScenarioFromVote] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"RestartScenarioFromVote\"");

  // IsMissionFinalMap()
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "IsMissionFinalMap");
  PrepSDKCall_SetReturnInfo(SDKType_Bool, SDKPass_Plain);
  g_hSDKCall[SDK_IsMissionFinalMap] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_IsMissionFinalMap] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"IsMissionFinalMap\"");

  // NotifyNetworkStateChanged()
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "NotifyNetworkStateChanged");
  g_hSDKCall[SDK_NotifyNetworkStateChanged] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_NotifyNetworkStateChanged] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"NotifyNetworkStateChanged\"");

  // SetReservationCookie(unsigned long long, char const*, ...)
  StartPrepSDKCall(SDKCall_Raw);
  PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "SetReservationCookie");
  PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
  g_hSDKCall[SDK_SetReservationCookie] = EndPrepSDKCall();
  if (g_hSDKCall[SDK_SetReservationCookie] == INVALID_HANDLE)
    SetFailState("Failed to create SDKCall for \"SetReservationCookie\"");
}

stock int FindEntityByNetClass(int startEnt, const char[] netClass) {
  int maxEntities = GetMaxEntities();
  for (int i = startEnt + 1; i <= maxEntities; i++) {
    if (!IsValidEdict(i)) continue;

    char name[255];
    GetEntityNetClass(i, name, sizeof(name));

    if (strcmp(name, netClass, false) == 0) {
      startEnt = i;
      return i;
    }
  }

  return -1;
}
