#ifndef _INCLUDE_LEFT4DOWNTOWN_VGLOBALS_H_
#define _INCLUDE_LEFT4DOWNTOWN_VGLOBALS_H_

#include "extension.h"

extern CTerrorGameRules *g_pGameRules;
extern CBaseServer *g_pServer;
extern CDirector *g_pDirector;
extern ZombieManager *g_pZombieManager;

void InitializeValveGlobals();
void *GetAddress(const char *signame);

#endif // _INCLUDE_LEFT4DOWNTOWN_VGLOBALS_H_
