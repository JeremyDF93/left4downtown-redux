#include "util.h"

CBaseEntity *UTIL_GetCBaseEntity(int num, bool onlyPlayers) {
  edict_t *pEdict = gamehelpers->EdictOfIndex(num);
  if (!pEdict || pEdict->IsFree()) {
    return NULL;
  }

  if (num > 0 && num <= playerhelpers->GetMaxClients()) {
    IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(pEdict);
    if (!pPlayer || !pPlayer->IsConnected()) {
      return NULL;
    }
  } else if (onlyPlayers) {
    return NULL;
  }

  IServerUnknown *pUnk;
  if ((pUnk=pEdict->GetUnknown()) == NULL) {
    return NULL;
  }

  return pUnk->GetBaseEntity();
}
