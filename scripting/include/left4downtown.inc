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

native int L4D_GetTeamScore(int logicalTeam, bool campaignScore=false);
native void L4D_RestartScenarioFromVote(const char[] map);
native void L4D_LobbyUnreserve();
native bool L4D_LobbyIsReserved();
native bool L4D_IsMissionFinalMap();
native void L4D_NotifyNetworkStateChanged();

/***
 *        __   __ __  ____ ___
 *       / /  / // / / __ \__ \
 *      / /  / // /_/ / / /_/ /
 *     / /__/__  __/ /_/ / __/
 *    /_____/ /_/ /_____/____/
 *
 */

forward Action L4D2_OnGetScriptValueInt(const char[] key, int &retVal);

native void L4D2_GetCampaignScores(int &scoreA, int &scoreB);
native void L4D2_ScavengeBeginRoundSetupTime();
native int L4D2_GetVersusMaxCompletionScore();
native void L4D2_SetVersusMaxCompletionScore(int score);
native void L4D2_ResetMobTimer();

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
