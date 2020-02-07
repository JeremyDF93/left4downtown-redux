#include "extension.h"
#include "forwards.h"

CDetour *Detour_GetScriptValueInt = nullptr;

IForward *g_pFwdOnGetScriptValueInt = nullptr;

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
}

void ReleaseL4D2Forwards() {
  if (Detour_GetScriptValueInt) {
    forwards->ReleaseForward(g_pFwdOnGetScriptValueInt);
    Detour_GetScriptValueInt->Destroy();
  }
}
