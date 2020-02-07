#include "extension.h"
#include "forwards.h"

CDetour *Detour_SpawnTank = nullptr;
CDetour *Detour_SpawnWitch = nullptr;
CDetour *Detour_ClearTeamScores = nullptr;
CDetour *Detour_SetCampaignScores = nullptr;
CDetour *Detour_OnFirstSurvivorLeftSafeArea = nullptr;
CDetour *Detour_TryOfferingTankBot = nullptr;
CDetour *Detour_OnMobRushStart = nullptr;
CDetour *Detour_ReplaceTank = nullptr;
CDetour *Detour_TakeOverBot = nullptr;
CDetour *Detour_TakeOverZombieBot = nullptr;
CDetour *Detour_ReplaceWithBot = nullptr;
CDetour *Detour_SetHumanSpectator = nullptr;
CDetour *Detour_EndVersusModeRound = nullptr;

IForward *g_pFwdOnSpawnTank = nullptr;
IForward *g_pFwdOnSpawnWitch = nullptr;
IForward *g_pFwdOnClearTeamScores = nullptr;
IForward *g_pFwdOnSetCampaignScores = nullptr;
IForward *g_pFwdOnFirstSurvivorLeftSafeArea = nullptr;
IForward *g_pFwdOnTryOfferingTankBot = nullptr;
IForward *g_pFwdOnMobRushStart = nullptr;
IForward *g_pFwdReplaceTank = nullptr;
IForward *g_pFwdTakeOverBot = nullptr;
IForward *g_pFwdTakeOverZombieBot = nullptr;
IForward *g_pFwdReplaceWithBot = nullptr;
IForward *g_pFwdSetHumanSpectator = nullptr;
IForward *g_pFwdEndVersusModeRound = nullptr;

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

/* ZombieManager::ReplaceTank(CTerrorPlayer*, CTerrorPlayer*) */
DETOUR_DECL_MEMBER2(ReplaceTank, bool, CTerrorPlayer *, pOldTank, CTerrorPlayer *, pNewTank) {
  if (!g_pFwdReplaceTank) {
    return DETOUR_MEMBER_CALL(ReplaceTank)(pOldTank, pNewTank);
  }

  int oldTank = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(pOldTank));
  int newTank = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(pNewTank));

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "ReplaceTank(%d, %d)", oldTank, newTank);
#endif

  cell_t result = Pl_Continue;
  g_pFwdReplaceTank->PushCell(oldTank);
  g_pFwdReplaceTank->PushCell(newTank);
  g_pFwdReplaceTank->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(ReplaceTank)(pOldTank, pNewTank);
  }

  return false;
}

/* CTerrorPlayer::TakeOverBot(bool) */
DETOUR_DECL_MEMBER1(TakeOverBot, bool, bool, flag) {
  if (!g_pFwdTakeOverBot) {
    return DETOUR_MEMBER_CALL(TakeOverBot)(flag);
  }

  int bot = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(this));

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "CTerrorPlayer(%d)::TakeOverBot(%d)", bot, flag);
#endif

  cell_t result = Pl_Continue;
  g_pFwdTakeOverBot->PushCell(bot);
  g_pFwdTakeOverBot->PushCell(flag);
  g_pFwdTakeOverBot->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(TakeOverBot)(flag);
  }

  return false;
}

/* CTerrorPlayer::TakeOverZombieBot(CTerrorPlayer*) */
DETOUR_DECL_MEMBER1(TakeOverZombieBot, void, CTerrorPlayer *, pBot) {
  if (!g_pFwdTakeOverBot) {
    DETOUR_MEMBER_CALL(TakeOverZombieBot)(pBot);
    return;
  }

  int client = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(this));
  int bot = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(pBot));

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "CTerrorPlayer(%d)::TakeOverZombieBot(%d)", client, bot);
#endif

  cell_t result = Pl_Continue;
  g_pFwdTakeOverZombieBot->PushCell(client);
  g_pFwdTakeOverZombieBot->PushCell(bot);
  g_pFwdTakeOverZombieBot->Execute(&result);

  if (result == Pl_Continue) {
    DETOUR_MEMBER_CALL(TakeOverZombieBot)(pBot);
  }
}

/* CTerrorPlayer::ReplaceWithBot(bool) */
DETOUR_DECL_MEMBER1(ReplaceWithBot, bool, bool, flag) {
  if (!g_pFwdTakeOverBot) {
    return DETOUR_MEMBER_CALL(ReplaceWithBot)(flag);
  }

  int client = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(this));

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "CTerrorPlayer(%d)::ReplaceWithBot(%d)", client, flag);
#endif

  cell_t result = Pl_Continue;
  g_pFwdReplaceWithBot->PushCell(client);
  g_pFwdReplaceWithBot->PushCell(flag);
  g_pFwdReplaceWithBot->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(ReplaceWithBot)(flag);
  }

  return false;
}

/* SurvivorBot::SetHumanSpectator(CTerrorPlayer*) */
DETOUR_DECL_MEMBER1(SetHumanSpectator, bool, CTerrorPlayer *, pPlayer) {
  if (!g_pFwdTakeOverBot) {
    return DETOUR_MEMBER_CALL(SetHumanSpectator)(pPlayer);
  }

  int bot = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(this));
  int client = gamehelpers->EntityToBCompatRef(reinterpret_cast<CBaseEntity *>(pPlayer));

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "SurvivorBot(%d)::SetHumanSpectator(%d)", bot, client);
#endif

  cell_t result = Pl_Continue;
  g_pFwdSetHumanSpectator->PushCell(bot);
  g_pFwdSetHumanSpectator->PushCell(client);
  g_pFwdSetHumanSpectator->Execute(&result);

  if (result == Pl_Continue) {
    return DETOUR_MEMBER_CALL(SetHumanSpectator)(pPlayer);
  }

  return false;
}

/* CDirectorVersusMode::EndVersusModeRound(bool) */
DETOUR_DECL_MEMBER1(EndVersusModeRound, void, bool, survived) {
  if (!g_pFwdEndVersusModeRound) {
    DETOUR_MEMBER_CALL(EndVersusModeRound)(survived);
    return;
  }

#ifdef _DEBUG
  g_pSM->LogMessage(myself, "EndVersusModeRound(%d)", survived);
#endif

  cell_t result = Pl_Continue;
  g_pFwdEndVersusModeRound->PushCell(survived);
  g_pFwdEndVersusModeRound->Execute(&result);

  if (result == Pl_Continue) {
    DETOUR_MEMBER_CALL(EndVersusModeRound)(survived);
    return;
  }
}

//      ______                 __  _
//     / ____/_  ______  _____/ /_(_)___  ____  _____
//    / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
//   / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
//  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
//

void CreateSharedDetours() {
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

  Detour_ReplaceTank = DETOUR_CREATE_MEMBER(ReplaceTank, "ReplaceTank");
  if (Detour_ReplaceTank) Detour_ReplaceTank->EnableDetour();

  Detour_TakeOverBot = DETOUR_CREATE_MEMBER(TakeOverBot, "TakeOverBot");
  if (Detour_TakeOverBot) Detour_TakeOverBot->EnableDetour();

  Detour_TakeOverZombieBot = DETOUR_CREATE_MEMBER(TakeOverZombieBot, "TakeOverZombieBot");
  if (Detour_TakeOverZombieBot) Detour_TakeOverZombieBot->EnableDetour();

  Detour_ReplaceWithBot = DETOUR_CREATE_MEMBER(ReplaceWithBot, "ReplaceWithBot");
  if (Detour_ReplaceWithBot) Detour_ReplaceWithBot->EnableDetour();

  Detour_SetHumanSpectator = DETOUR_CREATE_MEMBER(SetHumanSpectator, "SetHumanSpectator");
  if (Detour_SetHumanSpectator) Detour_SetHumanSpectator->EnableDetour();

  Detour_EndVersusModeRound = DETOUR_CREATE_MEMBER(EndVersusModeRound, "EndVersusModeRound");
  if (Detour_EndVersusModeRound) Detour_EndVersusModeRound->EnableDetour();

}

void DestroySharedDetours() {
  if (Detour_SpawnTank) Detour_SpawnTank->Destroy();
  if (Detour_SpawnWitch) Detour_SpawnWitch->Destroy();
  if (Detour_ClearTeamScores) Detour_ClearTeamScores->Destroy();
  if (Detour_SetCampaignScores) Detour_SetCampaignScores->Destroy();
  if (Detour_OnFirstSurvivorLeftSafeArea) Detour_OnFirstSurvivorLeftSafeArea->Destroy();
  if (Detour_TryOfferingTankBot) Detour_TryOfferingTankBot->Destroy();
  if (Detour_OnMobRushStart) Detour_TryOfferingTankBot->Destroy();
  if (Detour_ReplaceTank) Detour_ReplaceTank->Destroy();
  if (Detour_TakeOverBot) Detour_TakeOverBot->Destroy();
  if (Detour_TakeOverZombieBot) Detour_TakeOverZombieBot->Destroy();
  if (Detour_ReplaceWithBot) Detour_ReplaceWithBot->Destroy();
  if (Detour_SetHumanSpectator) Detour_SetHumanSpectator->Destroy();
  if (Detour_EndVersusModeRound) Detour_EndVersusModeRound->Destroy();
}

void CreateSharedForwards() {
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
  g_pFwdReplaceTank = forwards->CreateForward("L4D_OnReplaceTank", ET_Event, 2, nullptr,
      Param_Cell, Param_Cell);
  g_pFwdTakeOverBot = forwards->CreateForward("L4D_OnTakeOverBot", ET_Event, 2, nullptr,
      Param_Cell, Param_Cell);
  g_pFwdTakeOverZombieBot = forwards->CreateForward("L4D_OnTakeOverZombieBot", ET_Event, 2, nullptr,
      Param_Cell, Param_Cell);
  g_pFwdReplaceWithBot = forwards->CreateForward("L4D_OnReplaceWithBot", ET_Event, 2, nullptr,
      Param_Cell, Param_Cell);
  g_pFwdSetHumanSpectator = forwards->CreateForward("L4D_OnSetHumanSpectator", ET_Event, 2, nullptr,
      Param_Cell, Param_Cell);
  g_pFwdEndVersusModeRound = forwards->CreateForward("L4D_OnEndVersusModeRound", ET_Event, 1, nullptr,
      Param_Cell);
}

void ReleaseSharedForwards() {
  forwards->ReleaseForward(g_pFwdOnSpawnTank);
  forwards->ReleaseForward(g_pFwdOnSpawnWitch);
  forwards->ReleaseForward(g_pFwdOnClearTeamScores);
  forwards->ReleaseForward(g_pFwdOnSetCampaignScores);
  forwards->ReleaseForward(g_pFwdOnFirstSurvivorLeftSafeArea);
  forwards->ReleaseForward(g_pFwdOnTryOfferingTankBot);
  forwards->ReleaseForward(g_pFwdOnMobRushStart);
  forwards->ReleaseForward(g_pFwdReplaceTank);
  forwards->ReleaseForward(g_pFwdTakeOverBot);
  forwards->ReleaseForward(g_pFwdTakeOverZombieBot);
  forwards->ReleaseForward(g_pFwdReplaceWithBot);
  forwards->ReleaseForward(g_pFwdSetHumanSpectator);
  forwards->ReleaseForward(g_pFwdEndVersusModeRound);
}
