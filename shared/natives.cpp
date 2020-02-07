#include "extension.h"

CALL_DECL_MEMBER(CTerrorGameRules, GetTeamScore, int, (int, bool));
static cell_t L4D_GetTeamScore(IPluginContext *pContext, const cell_t *params) {
  int logicalTeam = params[1];
  bool type = params[2] ? true : false; // 0 = m_iSurvivorScore
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

  CALL_INVOKE_MEMBER(g_pGameRules, NotifyNetworkStateChanged)();

  return 1;
}

CALL_DECL_MEMBER(ZombieManager, SpawnTank, int, (Vector *, QAngle *));
static cell_t L4D_SpawnTank(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(SpawnTank, "SpawnTank"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  cell_t *vec_addr;
  pContext->LocalToPhysAddr(params[1], &vec_addr);

  cell_t *ang_addr;
  pContext->LocalToPhysAddr(params[2], &ang_addr);

  Vector vec(sp_ctof(vec_addr[0]), sp_ctof(vec_addr[1]), sp_ctof(vec_addr[2]));
  QAngle ang(sp_ctof(ang_addr[0]), sp_ctof(ang_addr[1]), sp_ctof(ang_addr[2]));

  return CALL_INVOKE_MEMBER(g_pZombieManager, SpawnTank)(&vec, &ang);
}

CALL_DECL_MEMBER(ZombieManager, SpawnWitch, int, (Vector *, QAngle *));
static cell_t L4D_SpawnWitch(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(SpawnWitch, "SpawnWitch"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  cell_t *vec_addr;
  pContext->LocalToPhysAddr(params[1], &vec_addr);

  cell_t *ang_addr;
  pContext->LocalToPhysAddr(params[2], &ang_addr);

  Vector vec(sp_ctof(vec_addr[0]), sp_ctof(vec_addr[1]), sp_ctof(vec_addr[2]));
  QAngle ang(sp_ctof(ang_addr[0]), sp_ctof(ang_addr[1]), sp_ctof(ang_addr[2]));

  return CALL_INVOKE_MEMBER(g_pZombieManager, SpawnWitch)(&vec, &ang);
}

CALL_DECL_MEMBER(CTerrorPlayer, TakeOverBot, bool, (bool));
static cell_t L4D_TakeOverBot(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(TakeOverBot, "TakeOverBot"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  CTerrorPlayer *pBot = reinterpret_cast<CTerrorPlayer *>(UTIL_GetCBaseEntity(params[1], true));
  bool flag = params[2] ? true : false;

  if (pBot == nullptr)
    return pContext->ThrowNativeError("Invalid bot client index %d", params[1]);

  return CALL_INVOKE_MEMBER(pBot, TakeOverBot)(flag);
}

CALL_DECL_MEMBER(CTerrorPlayer, TakeOverZombieBot, void, (CTerrorPlayer *));
static cell_t L4D_TakeOverZombieBot(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(TakeOverZombieBot, "TakeOverZombieBot"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  CTerrorPlayer *pPlayer = reinterpret_cast<CTerrorPlayer *>(UTIL_GetCBaseEntity(params[1], true));
  CTerrorPlayer *pBot = reinterpret_cast<CTerrorPlayer *>(UTIL_GetCBaseEntity(params[2], true));

  if (pPlayer == nullptr)
    return pContext->ThrowNativeError("Invalid client index %d", params[1]);
  if (pBot == nullptr)
    return pContext->ThrowNativeError("Invalid bot client index %d", params[2]);

  CALL_INVOKE_MEMBER(pPlayer, TakeOverZombieBot)(pBot);

  return 1;
}

CALL_DECL_MEMBER(CTerrorPlayer, ReplaceWithBot, bool, (bool));
static cell_t L4D_ReplaceWithBot(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(ReplaceWithBot, "ReplaceWithBot"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  CTerrorPlayer *pPlayer = reinterpret_cast<CTerrorPlayer *>(UTIL_GetCBaseEntity(params[1], true));
  bool flag = params[2] ? true : false;

  if (pPlayer == nullptr)
    return pContext->ThrowNativeError("Invalid client index %d", params[1]);

  return CALL_INVOKE_MEMBER(pPlayer, ReplaceWithBot)(flag);
}

CALL_DECL_MEMBER(SurvivorBot, SetHumanSpectator, bool, (CTerrorPlayer *));
static cell_t L4D_SetHumanSpectator(IPluginContext *pContext, const cell_t *params) {
  if (!g_pZombieManager)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN_MEMBER(SetHumanSpectator, "SetHumanSpectator"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  SurvivorBot *pBot = reinterpret_cast<SurvivorBot *>(UTIL_GetCBaseEntity(params[1], true));
  CTerrorPlayer *pPlayer = reinterpret_cast<CTerrorPlayer *>(UTIL_GetCBaseEntity(params[2], true));

  if (pBot == nullptr)
    return pContext->ThrowNativeError("Invalid bot client index %d", params[1]);
  if (pPlayer == nullptr)
    return pContext->ThrowNativeError("Invalid client index %d", params[2]);

  return CALL_INVOKE_MEMBER(pBot, SetHumanSpectator)(pPlayer);
}

sp_nativeinfo_t g_SharedNatives[] = {
  {"L4D_SpawnTank",                       L4D_SpawnTank},
  {"L4D_SpawnWitch",                      L4D_SpawnWitch},
  {"L4D_GetTeamScore",                    L4D_GetTeamScore},
  {"L4D_RestartScenarioFromVote",         L4D_RestartScenarioFromVote},
  {"L4D_LobbyUnreserve",                  L4D_LobbyUnreserve},
  {"L4D_LobbyIsReserved",                 L4D_IsLobbyReserved},
  {"L4D_IsMissionFinalMap",               L4D_IsMissionFinalMap},
  {"L4D_TakeOverBot",                     L4D_TakeOverBot},
  {"L4D_TakeOverZombieBot",               L4D_TakeOverZombieBot},
  {"L4D_ReplaceWithBot",                  L4D_ReplaceWithBot},
  {"L4D_SetHumanSpectator",               L4D_SetHumanSpectator},
  {nullptr,                               nullptr}
};
