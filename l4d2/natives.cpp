#include "extension.h"

static cell_t L4D2_GetCampaignScores(IPluginContext *pContext, const cell_t *params) {
  cell_t *addr;
  pContext->LocalToPhysAddr(params[1], &addr);
  *addr = static_cast<cell_t>(-1);

  pContext->LocalToPhysAddr(params[2], &addr);
  *addr = static_cast<cell_t>(-1);

  return 1;
}

CALL_DECL(OnBeginRoundSetupTime, void, (void));
static cell_t L4D2_ScavengeBeginRoundSetupTime(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN(OnBeginRoundSetupTime, "OnBeginRoundSetupTime"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  CALL_INVOKE(g_pDirector, OnBeginRoundSetupTime)();

  return 1;
}

static cell_t L4D2_GetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params) {
  return 1;
}

static cell_t L4D2_SetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params) {
  return 1;
}

CALL_DECL(ResetMobTimer, void, (void));
static cell_t L4D2_ResetMobTimer(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN(ResetMobTimer, "ResetMobTimer"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  CALL_INVOKE(g_pDirector, ResetMobTimer)();

  return 1;
}

/* CDirector::IsFirstMapInScenario() const */
CALL_DECL(IsFirstMapInScenario, bool, (void));
static cell_t L4D2_IsFirstMapInScenario(IPluginContext *pContext, const cell_t *params) {
  if (!g_pDirector)
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");
  if (!CALL_JOIN(IsFirstMapInScenario, "IsFirstMapInScenario"))
    return pContext->ThrowNativeError("Error detected in native call (see error logs)");

  return CALL_INVOKE(g_pDirector, IsFirstMapInScenario)();
}

sp_nativeinfo_t g_L4D2Natives[] = {
  {"L4D2_GetCampaignScores",              L4D2_GetCampaignScores},
  {"L4D2_ScavengeBeginRoundSetupTime",    L4D2_ScavengeBeginRoundSetupTime},
  {"L4D2_GetVersusMaxCompletionScore",    L4D2_GetVersusMaxCompletionScore},
  {"L4D2_SetVersusMaxCompletionScore",    L4D2_SetVersusMaxCompletionScore},
  {"L4D2_ResetMobTimer",                  L4D2_ResetMobTimer},
  {"L4D2_IsFirstMapInScenario",            L4D2_IsFirstMapInScenario},
  {nullptr,                               nullptr}
};
