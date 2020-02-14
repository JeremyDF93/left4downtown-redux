#include "extension.h"
#include "forwards.h"

CDetour *Detour_GetScriptValueInt = nullptr;
CDetour *Detour_GetMissionVersusBossSpawning = nullptr;

IForward *g_pFwdOnGetScriptValueInt = nullptr;
IForward *g_pFwdOnGetMissionVersusBossSpawning = nullptr;

// GetScriptValue(char const*, int)
DETOUR_DECL_MEMBER2(GetScriptValueInt, int, const char *, key, int, def) {
  if (!g_pFwdOnGetScriptValueInt) {
    return DETOUR_MEMBER_CALL(GetScriptValueInt)(key, def);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "GetScriptValueInt(%s, %d)", key, def);
#endif

  int orig = DETOUR_MEMBER_CALL(GetScriptValueInt)(key, def);
  int ref = orig;

  cell_t result = Pl_Continue;

  g_pFwdOnGetScriptValueInt->PushString(key);
  g_pFwdOnGetScriptValueInt->PushCellByRef(&ref);
  g_pFwdOnGetScriptValueInt->Execute(&result);

  if (result > Pl_Continue) {
    return ref;
  }

  return orig;
}

/* CDirectorVersusMode::GetMissionVersusBossSpawning(float&, float&, float&, float&) */
DETOUR_DECL_MEMBER4(GetMissionVersusBossSpawning, void, float &, min, float &, max, float &, tankChance, float &, witchChance) {
  if (!g_pFwdOnGetScriptValueInt) {
    return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(min, max, tankChance, witchChance);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "GetMissionVersusBossSpawning(%s, %d)", min, max, tankChance, witchChance);
#endif

  float minRef = min;
  float maxRef = max;
  float tankChanceRef = tankChance;
  float witchChanceRef = witchChance;

  cell_t result = Pl_Continue;

  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&minRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&maxRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&tankChanceRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&witchChanceRef);
  g_pFwdOnGetMissionVersusBossSpawning->Execute(&result);

  if (result == Pl_Changed)
    return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(minRef, maxRef, tankChanceRef, witchChanceRef);
  if (result == Pl_Handled)
    return;

  return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(min, max, tankChance, witchChance);
}

//      ______                 __  _
//     / ____/_  ______  _____/ /_(_)___  ____  _____
//    / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
//   / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
//  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
//

void CreateL4D2Forwards() {
  Detour_GetScriptValueInt = DETOUR_CREATE_MEMBER(GetScriptValueInt, "GetScriptValueInt");
  if (Detour_GetScriptValueInt) {
    g_pFwdOnGetScriptValueInt = forwards->CreateForward("L4D2_OnGetScriptValueInt", ET_Event, 2, nullptr,
        Param_String, Param_CellByRef);
    Detour_GetScriptValueInt->EnableDetour();
  } else {
    smutils->LogError(myself, "\"%s\" detour could not be initialized", "GetScriptValueInt");
  }

  Detour_GetMissionVersusBossSpawning = DETOUR_CREATE_MEMBER(GetMissionVersusBossSpawning, "GetMissionVersusBossSpawning");
  if (Detour_GetMissionVersusBossSpawning) {
    g_pFwdOnGetMissionVersusBossSpawning = forwards->CreateForward("L4D2_OnGetMissionVSBossSpawning", ET_Event, 4, nullptr,
        Param_FloatByRef, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef);
    Detour_GetMissionVersusBossSpawning->EnableDetour();
  } else {
    smutils->LogError(myself, "\"%s\" detour could not be initialized", "GetMissionVersusBossSpawning");
  }
}

void ReleaseL4D2Forwards() {
  if (Detour_GetScriptValueInt) {
    forwards->ReleaseForward(g_pFwdOnGetScriptValueInt);
    Detour_GetScriptValueInt->Destroy();
  }

  if (Detour_GetMissionVersusBossSpawning) {
    forwards->ReleaseForward(g_pFwdOnGetMissionVersusBossSpawning);
    Detour_GetMissionVersusBossSpawning->Destroy();
  }
}
