#include "extension.h"
#include "shared/forwards.h"
#include "l4d2/forwards.h"
#include "RegNatives.h"

Left4downtown g_Left4downtown;
SMEXT_LINK(&g_Left4downtown);

IGameConfig *g_pGameConf = nullptr;
IBinTools *g_pBinTools = nullptr;
ISDKTools *g_pSDKTools = nullptr;

ICvar *icvar = nullptr;
CGlobalVars *gpGlobals = nullptr;

bool m_bDetoursEnabled;

ConVar left4downtown_version("left4downtown_redux_version", SMEXT_CONF_VERSION, FCVAR_SPONLY|FCVAR_NOTIFY,
    "Left 4 Downtown redux version");
ConVar l4d_maxplayers("l4d_maxplayers", "-1", FCVAR_SPONLY|FCVAR_NOTIFY,
    "Overrides maxplayers with this value");

class BaseAccessor : public IConCommandBaseAccessor {
public:
  bool RegisterConCommandBase(ConCommandBase *pCommandBase) {
    return META_REGCVAR(pCommandBase);
  }
} s_BaseAccessor;

SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int , int);

bool Left4downtown::SDK_OnLoad(char *error, size_t maxlen, bool late) {
  plsys->AddPluginsListener(this);

  sharesys->RegisterLibrary(myself, "left4downtown");
  sharesys->AddDependency(myself, "bintools.ext", true, true);
  sharesys->AddDependency(myself, "sdktools.ext", true, true);
  sharesys->AddNatives(myself, g_SharedNatives);
#if SOURCE_ENGINE == SE_LEFT4DEAD2
  sharesys->AddNatives(myself, g_L4D2Natives);
#endif

  if (!gameconfs->LoadGameConfigFile("left4downtown.games", &g_pGameConf, error, maxlen)) {
    return false;
  }

  CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
  CCallManager::Init(g_pGameConf);

  CreateSharedForwards();
  CreateSharedDetours();

#if SOURCE_ENGINE == SE_LEFT4DEAD2
  CreateL4D2Forwards();
  CreateL4D2Detours();
#endif

#ifdef _DEBUG
  smutils->LogMessage(myself, "Left 4 Downtown Redux v%s loaded", left4downtown_version.GetString());
#endif

  return true;
}

void Left4downtown::SDK_OnUnload() {
  g_RegNatives.UnregisterAll();

  gameconfs->CloseGameConfigFile(g_pGameConf);

  ReleaseSharedForwards();
  DestroySharedDetours();

#if SOURCE_ENGINE == SE_LEFT4DEAD2
  ReleaseL4D2Forwards();
  DestroyL4D2Detours();
#endif
}

void Left4downtown::SDK_OnAllLoaded() {
  SM_GET_LATE_IFACE(SDKTOOLS, g_pSDKTools);
  if (g_pSDKTools == nullptr) {
    smutils->LogError(myself, "SDKTools interface not found");
  }

  SM_GET_LATE_IFACE(BINTOOLS, g_pBinTools);
}

bool Left4downtown::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late) {
  GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

  g_pCVar = icvar;
  ConVar_Register(0, &s_BaseAccessor);

  gpGlobals = ismm->GetCGlobals();

  SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, gamedll, this, &Left4downtown::OnServerActivate, true);

  return true;
}

bool Left4downtown::SDK_OnMetamodUnload(char *error, size_t maxlen) {
  SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, gamedll, this, &Left4downtown::OnServerActivate, true);

  return true;
}

// void Left4downtown::OnPluginLoaded(IPlugin *plugin) {
//   if (!m_bDetoursEnabled) {
//     m_bDetoursEnabled = true;
//     CreateDetours();
//   }
// }

// void Left4downtown::OnPluginUnloaded(IPlugin *plugin) {
//   if (m_bDetoursEnabled) {
//     m_bDetoursEnabled = false;
//     DestroyDetours();
//   }
// }

void Left4downtown::OnServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
  InitializeValveGlobals();
}
