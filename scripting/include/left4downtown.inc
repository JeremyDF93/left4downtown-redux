#if defined _left4downtown_included
 #endinput
#endif
#define _left4downtown_included

#tryinclude <left4downtown_version_auto>
#include <left4downtown_version>
#include <sourcemod>

/***
 *       _____ __                        __
 *      / ___// /_  ____ _________  ____/ /
 *      \__ \/ __ \/ __ `/ ___/ _ \/ __  /
 *     ___/ / / / / /_/ / /  /  __/ /_/ /
 *    /____/_/ /_/\__,_/_/   \___/\__,_/
 *
 */

forward Action L4D_OnSpawnTank(const float vector[3], const float qangle[3]);
forward Action L4D_OnSpawnWitch(const float vector[3], const float qangle[3]);
forward Action L4D_OnClearTeamScores(bool newCampaign);
forward Action L4D_OnSetCampaignScores(int &scoreA, int &scoreB);
forward Action L4D_OnFirstSurvivorLeftSafeArea(int client);
forward Action L4D_OnEnterGhostState(int client);
forward Action L4D_OnTryOfferingTankBot();
forward Action L4D_OnMobRushStart();
forward Action L4D_OnReplaceTank(int oldTank, int newTank);
forward Action L4D_OnTakeOverBot(int bot, bool flag);
forward Action L4D_OnTakeOverZombieBot(int client, int bot);
forward Action L4D_OnReplaceWithBot(int client, bool flag);
forward Action L4D_OnSetHumanSpectator(int bot, int client);
forward Action L4D_OnEndVersusModeRound(bool survived);

native int L4D_SpawnTank(const float vector[3], const float qangle[3]);
native int L4D_SpawnWitch(const float vector[3], const float qangle[3]);
native int L4D_GetTeamScore(int logicalTeam, bool campaignScore=false);
native void L4D_RestartScenarioFromVote(const char[] map);
native void L4D_LobbyUnreserve();
native bool L4D_LobbyIsReserved();
native bool L4D_IsMissionFinalMap();
native void L4D_NotifyNetworkStateChanged();
native void L4D_TakeOverBot(int client, bool flag=true);
native void L4D_TakeOverZombieBot(int client, int bot);
native void L4D_ReplaceWithBot(int client, bool flag=true);
native void L4D_SetHumanSpectator(int bot, int client);
native void L4D_ReplaceTank(int oldTank, int newTank);

/***
 *        __   __ __  ____ ___
 *       / /  / // / / __ \__ \
 *      / /  / // /_/ / / /_/ /
 *     / /__/__  __/ /_/ / __/
 *    /_____/ /_/ /_____/____/
 *
 */

forward Action L4D2_OnGetScriptValueInt(const char[] key, int &retVal);
forward Action L4D2_OnGetMissionVSBossSpawning(float &min, float &max, float &tankChance, float &witchChance);

native void L4D2_GetCampaignScores(int &scoreA, int &scoreB);
native void L4D2_ScavengeBeginRoundSetupTime();
native int L4D2_GetVersusMaxCompletionScore();
native void L4D2_SetVersusMaxCompletionScore(int score);
native void L4D2_ResetMobTimer();
native bool L4D2_IsFirstMapInScenario();

/*
public SharedPlugin __pl_left4downtown = {
  name = "Left 4 Downtown",
  file = "left4downtown.smx",
#if defined REQUIRE_PLUGIN
  required = 1,
#else
  required = 0,
#endif
};
*/

public Extension __ext_left4downtown = {
  name = "Left 4 Downtown",
  file = "left4downtown.ext",
#if defined AUTOLOAD_EXTENSIONS
  autoload = 1,
#else
  autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
  required = 1,
#else
  required = 0,
#endif
};
