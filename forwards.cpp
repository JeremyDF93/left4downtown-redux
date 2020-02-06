#include "extension.h"
#include "forwards.h"

CDetour *Detour_SpawnTank = nullptr;
CDetour *Detour_SpawnWitch = nullptr;
CDetour *Detour_ClearTeamScores = nullptr;
CDetour *Detour_SetCampaignScores = nullptr;
CDetour *Detour_OnFirstSurvivorLeftSafeArea = nullptr;
CDetour *Detour_TryOfferingTankBot = nullptr;
CDetour *Detour_OnMobRushStart = nullptr;

CDetour *Detour_GetScriptValueInt = nullptr;

IForward *g_pFwdOnSpawnTank = nullptr;
IForward *g_pFwdOnSpawnWitch = nullptr;
IForward *g_pFwdOnClearTeamScores = nullptr;
IForward *g_pFwdOnSetCampaignScores = nullptr;
IForward *g_pFwdOnFirstSurvivorLeftSafeArea = nullptr;
IForward *g_pFwdOnTryOfferingTankBot = nullptr;
IForward *g_pFwdOnMobRushStart = nullptr;

IForward *g_pFwdOnGetScriptValueInt = nullptr;

//   _____                   __    _      __  ___
//  /__  /  ____  ____ ___  / /_  (_)__  /  |/  /___ _____  ____ _____ ____  _____
//    / /  / __ \/ __ `__ \/ __ \/ / _ \/ /|_/ / __ `/ __ \/ __ `/ __ `/ _ \/ ___/
//   / /__/ /_/ / / / / / / /_/ / /  __/ /  / / /_/ / / / / /_/ / /_/ /  __/ /
//  /____/\____/_/ /_/ /_/_.___/_/\___/_/  /_/\__,_/_/ /_/\__,_/\__, /\___/_/
//                                                             /____/

// SpawnTank(Vector const&, QAngle const&)
DETOUR_DECL_MEMBER2(SpawnTank, int, Vector *, pVector, QAngle *, pAngles) {
  if (!g_pFwdOnSpawnTank) {
    return DETOUR_MEMBER_CALL(SpawnTank)(pVector, pAngles);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "SpawnTank(%0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f)",
      pVector->x, pVector->y, pVector->z, pAngles->x, pAngles->y, pAngles->z);
#endif

  cell_t vector[3] = { sp_ftoc(pVector->x), sp_ftoc(pVector->y), sp_ftoc(pVector->z) };
  cell_t angles[3] = { sp_ftoc(pAngles->x), sp_ftoc(pAngles->y), sp_ftoc(pAngles->z) };
  cell_t result = Pl_Continue;

  g_pFwdOnSpawnTank->PushArray(vector, 3);
  g_pFwdOnSpawnTank->PushArray(angles, 3);
  g_pFwdOnSpawnTank->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(SpawnTank)(pVector, pAngles);
  }

  return 0;
}

// SpawnWitch(Vector const&, QAngle const&)
DETOUR_DECL_MEMBER2(SpawnWitch, int, Vector *, pVector, QAngle *, pAngles) {
  if (!g_pFwdOnSpawnWitch) {
    return DETOUR_MEMBER_CALL(SpawnWitch)(pVector, pAngles);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "SpawnWitch(%0.3f, %0.3f, %0.3f, %0.3f, %0.3f, %0.3f)",
      pVector->x, pVector->y, pVector->z, pAngles->x, pAngles->y, pAngles->z);
#endif

  cell_t vector[3] = { sp_ftoc(pVector->x), sp_ftoc(pVector->y), sp_ftoc(pVector->z) };
  cell_t angles[3] = { sp_ftoc(pAngles->x), sp_ftoc(pAngles->y), sp_ftoc(pAngles->z) };
  cell_t result = Pl_Continue;

  g_pFwdOnSpawnWitch->PushArray(vector, 3);
  g_pFwdOnSpawnWitch->PushArray(angles, 3);
  g_pFwdOnSpawnWitch->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(SpawnWitch)(pVector, pAngles);
  }

  return 0;
}

//     __________  _                __
//    / ____/ __ \(_)_______  _____/ /_____  _____
//   / /   / / / / / ___/ _ \/ ___/ __/ __ \/ ___/
//  / /___/ /_/ / / /  /  __/ /__/ /_/ /_/ / /
//  \____/_____/_/_/   \___/\___/\__/\____/_/
//

// ClearTeamScores(bool)
DETOUR_DECL_MEMBER1(ClearTeamScores, void, bool, newCampaign) {
  if (!g_pFwdOnClearTeamScores) {
    return DETOUR_MEMBER_CALL(ClearTeamScores)(newCampaign);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "ClearTeamScores(%d)", newCampaign);
#endif

  cell_t result = Pl_Continue;

  g_pFwdOnClearTeamScores->PushCell(newCampaign);
  g_pFwdOnClearTeamScores->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(ClearTeamScores)(newCampaign);
  }
}

// SetCampaignScores(int, int)
DETOUR_DECL_MEMBER2(SetCampaignScores, void, int, scoreA, int, scoreB) {
  if (!g_pFwdOnSetCampaignScores) {
    return DETOUR_MEMBER_CALL(SetCampaignScores)(scoreA, scoreB);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "SetCampaignScores(%d, %d)", scoreA, scoreB);
#endif

  int valA = scoreA;
  int valB = scoreB;

  cell_t result = Pl_Continue;

  g_pFwdOnSetCampaignScores->PushCellByRef(&valA);
  g_pFwdOnSetCampaignScores->PushCellByRef(&valB);
  g_pFwdOnSetCampaignScores->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(SetCampaignScores)(scoreA, scoreB);
  } else if (result == Pl_Changed) {
    return DETOUR_MEMBER_CALL(SetCampaignScores)(valA, valB);
  }
}

// OnFirstSurvivorLeftSafeArea(CTerrorPlayer*)
DETOUR_DECL_MEMBER1(OnFirstSurvivorLeftSafeArea, void, CTerrorPlayer *, pPlayer) {
  if (!g_pFwdOnFirstSurvivorLeftSafeArea) {
    return DETOUR_MEMBER_CALL(OnFirstSurvivorLeftSafeArea)(pPlayer);
  }

  int client = 0;
  if (pPlayer) {
    client = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(pPlayer));
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "OnFirstSurvivorLeftSafeArea(%d)", client);
#endif

  cell_t result = Pl_Continue;
  g_pFwdOnFirstSurvivorLeftSafeArea->PushCell(client);
  g_pFwdOnFirstSurvivorLeftSafeArea->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(OnFirstSurvivorLeftSafeArea)(pPlayer);
  }
}

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

  if (result == Pl_Changed || result == Pl_Handled) {
    return ref;
  }

  return orig;
}

// TryOfferingTankBot(CBaseEntity*, bool)
DETOUR_DECL_MEMBER2(TryOfferingTankBot, void, CBaseEntity *, pEntity, bool, flag) {
  if (!g_pFwdOnTryOfferingTankBot) {
    return DETOUR_MEMBER_CALL(TryOfferingTankBot)(pEntity, flag);
  }

  int client = 0;
  if (pEntity) {
    client = gamehelpers->EntityToBCompatRef(pEntity);
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "TryOfferingTankBot(%d, %d)", client, flag);
#endif

  cell_t result = Pl_Continue;
  g_pFwdOnTryOfferingTankBot->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(TryOfferingTankBot)(pEntity, flag);
  }
}

// OnMobRushStart()
DETOUR_DECL_MEMBER0(OnMobRushStart, void) {
  if (!g_pFwdOnMobRushStart) {
    return DETOUR_MEMBER_CALL(OnMobRushStart)();
  }

#ifdef _DEBUG
  smutils->LogMessage(myself, "OnMobRushStart()");
#endif

  cell_t result = Pl_Continue;
  g_pFwdOnMobRushStart->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(OnMobRushStart)();
  }
}

//      ______                 __  _
//     / ____/_  ______  _____/ /_(_)___  ____  _____
//    / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
//   / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
//  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
//

void CreateDetours() {
  Detour_SpawnTank = DETOUR_CREATE_MEMBER(SpawnTank, "SpawnTank");
  if (Detour_SpawnTank) Detour_SpawnTank->EnableDetour();

  Detour_SpawnWitch = DETOUR_CREATE_MEMBER(SpawnWitch, "SpawnWitch");
  if (Detour_SpawnWitch) Detour_SpawnWitch->EnableDetour();

  Detour_ClearTeamScores = DETOUR_CREATE_MEMBER(ClearTeamScores, "ClearTeamScores");
  if (Detour_ClearTeamScores) Detour_ClearTeamScores->EnableDetour();

  Detour_SetCampaignScores = DETOUR_CREATE_MEMBER(SetCampaignScores, "SetCampaignScores");
  if (Detour_SetCampaignScores) Detour_SetCampaignScores->EnableDetour();

  Detour_OnFirstSurvivorLeftSafeArea = DETOUR_CREATE_MEMBER(OnFirstSurvivorLeftSafeArea, "OnFirstSurvivorLeftSafeArea");
  if (Detour_OnFirstSurvivorLeftSafeArea) Detour_OnFirstSurvivorLeftSafeArea->EnableDetour();

  Detour_TryOfferingTankBot = DETOUR_CREATE_MEMBER(TryOfferingTankBot, "TryOfferingTankBot");
  if (Detour_TryOfferingTankBot) Detour_TryOfferingTankBot->EnableDetour();

  Detour_OnMobRushStart = DETOUR_CREATE_MEMBER(OnMobRushStart, "OnMobRushStart");
  if (Detour_OnMobRushStart) Detour_OnMobRushStart->EnableDetour();

#if SOURCE_ENGINE == SE_LEFT4DEAD2
  Detour_GetScriptValueInt = DETOUR_CREATE_MEMBER(GetScriptValueInt, "GetScriptValueInt");
  if (Detour_GetScriptValueInt) Detour_GetScriptValueInt->EnableDetour();
#endif
}

void DestroyDetours() {
  if (Detour_SpawnTank) Detour_SpawnTank->Destroy();
  if (Detour_SpawnWitch) Detour_SpawnWitch->Destroy();
  if (Detour_ClearTeamScores) Detour_ClearTeamScores->Destroy();
  if (Detour_SetCampaignScores) Detour_SetCampaignScores->Destroy();
  if (Detour_OnFirstSurvivorLeftSafeArea) Detour_OnFirstSurvivorLeftSafeArea->Destroy();
  if (Detour_TryOfferingTankBot) Detour_TryOfferingTankBot->Destroy();
  if (Detour_OnMobRushStart) Detour_TryOfferingTankBot->Destroy();

  if (Detour_GetScriptValueInt) Detour_GetScriptValueInt->Destroy();
}

void CreateForwards() {
  g_pFwdOnSpawnTank = forwards->CreateForward("L4D_OnSpawnTank", ET_Event, 2, nullptr,
      Param_Array, Param_Array);
  g_pFwdOnSpawnWitch = forwards->CreateForward("L4D_OnSpawnWitch", ET_Event, 2, nullptr,
      Param_Array, Param_Array);
  g_pFwdOnClearTeamScores = forwards->CreateForward("L4D_OnClearTeamScores", ET_Event, 1, nullptr,
      Param_Cell);
  g_pFwdOnSetCampaignScores = forwards->CreateForward("L4D_OnSetCampaignScores", ET_Event, 2, nullptr,
      Param_CellByRef, Param_CellByRef);
  g_pFwdOnFirstSurvivorLeftSafeArea = forwards->CreateForward("L4D_OnFirstSurvivorLeftSafeArea", ET_Event, 1, nullptr,
      Param_Cell);
  g_pFwdOnTryOfferingTankBot = forwards->CreateForward("L4D_OnTryOfferingTankBot", ET_Event, 0, nullptr);
  g_pFwdOnMobRushStart = forwards->CreateForward("L4D_OnMobRushStart", ET_Event, 0, nullptr);

  g_pFwdOnGetScriptValueInt = forwards->CreateForward("L4D2_OnGetScriptValueInt", ET_Event, 2, nullptr,
      Param_String, Param_CellByRef);
}

void ReleaseForwards() {
  forwards->ReleaseForward(g_pFwdOnSpawnTank);
  forwards->ReleaseForward(g_pFwdOnSpawnWitch);
  forwards->ReleaseForward(g_pFwdOnClearTeamScores);
  forwards->ReleaseForward(g_pFwdOnSetCampaignScores);
  forwards->ReleaseForward(g_pFwdOnFirstSurvivorLeftSafeArea);
  forwards->ReleaseForward(g_pFwdOnTryOfferingTankBot);
  forwards->ReleaseForward(g_pFwdOnMobRushStart);

  forwards->ReleaseForward(g_pFwdOnGetScriptValueInt);
}
