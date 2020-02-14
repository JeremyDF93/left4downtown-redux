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
DETOUR_DECL_MEMBER4(GetMissionVersusBossSpawning, void, float &, fMin, float &, fMax, float &, fTankChance, float &, fWitchChance) {
  if (!g_pFwdOnGetScriptValueInt) {
    return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(fMin, fMax, fTankChance, fWitchChance);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "GetMissionVersusBossSpawning(%s, %d)", fMin, fMax, fTankChance, fWitchChance);
#endif

  float fMinRef = fMin;
  float fMaxRef = fMax;
  float fTankChanceRef = fTankChance;
  float fWitchChanceRef = fWitchChance;

  cell_t result = Pl_Continue;

  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&fMinRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&fMaxRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&fTankChanceRef);
  g_pFwdOnGetMissionVersusBossSpawning->PushFloatByRef(&fWitchChanceRef);
  g_pFwdOnGetMissionVersusBossSpawning->Execute(&result);

  if (result == Pl_Changed)
    return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(fMinRef, fMaxRef, fTankChanceRef, fWitchChanceRef);
  if (result == Pl_Handled)
    return;

  return DETOUR_MEMBER_CALL(GetMissionVersusBossSpawning)(fMin, fMax, fTankChance, fWitchChance);
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
