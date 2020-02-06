#include "vglobals.h"

CTerrorGameRules *g_pGameRules = nullptr;
CBaseServer *g_pServer = nullptr;
CDirector *g_pDirector = nullptr;
ZombieManager *g_pZombieManager = nullptr;

void InitializeValveGlobals() {
  g_pGameRules = (CTerrorGameRules *) g_pSDKTools->GetGameRules();
  if (!g_pGameRules) {
    smutils->LogError(myself, "GameRules not found");
  }

  g_pServer = (CBaseServer *) g_pSDKTools->GetIServer();
  if (!g_pServer) {
    smutils->LogError(myself, "IServer not found");
  }

  g_pDirector = (CDirector *) GetAddress("TheDirector");
  g_pZombieManager = (ZombieManager *) GetAddress("TheZombieManager");
}

void *GetAddress(const char *signame) {
  void *addr;
  if (!g_pGameConf->GetAddress(signame, &addr)) {
    smutils->LogError(myself, "Could not locate \"%s\"", signame);
  } else {
    if (!addr) {
      smutils->LogError(myself, "Failed to find address for \"%s\"", signame);
    }
  }

  return addr;
}
