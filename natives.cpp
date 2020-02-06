#include "extension.h"
#include "RegNatives.h"

/***
 *       _____ __                        __
 *      / ___// /_  ____ _________  ____/ /
 *      \__ \/ __ \/ __ `/ ___/ _ \/ __  /
 *     ___/ / / / / /_/ / /  /  __/ /_/ /
 *    /____/_/ /_/\__,_/_/   \___/\__,_/
 *
 */

CALL_DECL_MEMBER(CTerrorGameRules, GetTeamScore, int, (int, bool));
static cell_t L4D_GetTeamScore(IPluginContext *pContext, const cell_t *params) {
  int logicalTeam = (int) params[1];
  bool type = (bool) params[2]; // 0 = m_iSurvivorScore
                                // 1 = m_iCampaignScore

  if (!g_pGameRules)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(GetTeamScore, "GetTeamScore"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (logicalTeam != 1 && logicalTeam != 2)
    return pContext->ThrowNativeError("Logical team \"%d\" is invalid - options are \"1\" or \"2\"", logicalTeam);

  return CALL_INVOKE_MEMBER(g_pGameRules, GetTeamScore)(logicalTeam, type);
}

CALL_DECL_MEMBER(CBaseServer, SetReservationCookie, void, (uint64_t, const char *, ...));
static cell_t L4D_LobbyUnreserve(IPluginContext *pContext, const cell_t *params) {
  if (!g_pServer)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(SetReservationCookie, "SetReservationCookie"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  uint64_t reservationCookie = 0;
  const char* formatString = "Manually unreserved by Left 4 Downtown Extension";
  CALL_INVOKE_MEMBER(g_pServer, SetReservationCookie)(reservationCookie, formatString);

  return 1;
}

static cell_t L4D_IsLobbyReserved(IPluginContext *pContext, const cell_t *params) {
  static ConVarRef sv_hosting_lobby("sv_hosting_lobby");

  return sv_hosting_lobby.GetBool();
}

CALL_DECL_MEMBER(CTerrorGameRules, IsMissionFinalMap, bool, (void));
static cell_t L4D_IsMissionFinalMap(IPluginContext *pContext, const cell_t *params) {
  if (!g_pGameRules)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(IsMissionFinalMap, "IsMissionFinalMap"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  return CALL_INVOKE_MEMBER(g_pGameRules, IsMissionFinalMap)();
}

CALL_DECL_MEMBER(CDirector, RestartScenarioFromVote, void, (char const*));
static cell_t L4D_RestartScenarioFromVote(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(IsMissionFinalMap, "RestartScenarioFromVote"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  char *map = nullptr;
  pContext->LocalToString(params[1], &map);
  CALL_INVOKE_MEMBER(g_pDirector, RestartScenarioFromVote)(map);

  return 1;
}

CALL_DECL_MEMBER(CTerrorGameRules, NotifyNetworkStateChanged, void, (void));
static cell_t L4D_NotifyNetworkStateChanged(IPluginContext *pContext, const cell_t *params) {
  if (!g_pGameRules)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(IsMissionFinalMap, "NotifyNetworkStateChanged"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  char *map = nullptr;
  pContext->LocalToString(params[1], &map);
  CALL_INVOKE_MEMBER(g_pGameRules, NotifyNetworkStateChanged)();

  return 1;
}

/***
 *        __   __ __  ____ ___
 *       / /  / // / / __ \__ \
 *      / /  / // /_/ / / /_/ /
 *     / /__/__  __/ /_/ / __/
 *    /_____/ /_/ /_____/____/
 *
 */

static cell_t L4D2_GetCampaignScores(IPluginContext *pContext, const cell_t *params) {
  cell_t *addr;
  pContext->LocalToPhysAddr(params[1], &addr);
  *addr = static_cast<cell_t>(-1);

  pContext->LocalToPhysAddr(params[2], &addr);
  *addr = static_cast<cell_t>(-1);

  return 1;
}

CALL_DECL_MEMBER(CDirector, OnBeginRoundSetupTime, void, (void));
static cell_t L4D2_ScavengeBeginRoundSetupTime(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(OnBeginRoundSetupTime, "OnBeginRoundSetupTime")) {
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  }

  CALL_INVOKE_MEMBER(g_pDirector, OnBeginRoundSetupTime)();

  return 1;
}

static cell_t L4D2_GetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params) {
  return 1;
}

static cell_t L4D2_SetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params) {
  return 1;
}

CALL_DECL_MEMBER(CDirector, ResetMobTimer, void, (void));
static cell_t L4D2_ResetMobTimer(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(ResetMobTimer, "ResetMobTimer")) {
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  }

  CALL_INVOKE_MEMBER(g_pDirector, ResetMobTimer)();

  return 1;
}

sp_nativeinfo_t g_MyNatives[] = {
  {"L4D_GetTeamScore",                    L4D_GetTeamScore},
  {"L4D_RestartScenarioFromVote",         L4D_RestartScenarioFromVote},
  {"L4D_LobbyUnreserve",                  L4D_LobbyUnreserve},
  {"L4D_LobbyIsReserved",                 L4D_IsLobbyReserved},
  {"L4D_IsMissionFinalMap",               L4D_IsMissionFinalMap},
  {"L4D_NotifyNetworkStateChanged",       L4D_NotifyNetworkStateChanged},
  {"L4D2_GetCampaignScores",              L4D2_GetCampaignScores},
  {"L4D2_ScavengeBeginRoundSetupTime",    L4D2_ScavengeBeginRoundSetupTime},
  {"L4D2_GetVersusMaxCompletionScore",    L4D2_GetVersusMaxCompletionScore},
  {"L4D2_SetVersusMaxCompletionScore",    L4D2_SetVersusMaxCompletionScore},
  {"L4D2_ResetMobTimer",                  L4D2_ResetMobTimer},
  {nullptr,                               nullptr}
};
