#include "calls.h"

IGameConfig *CCallManager::gameconf = nullptr;

void CCallManager::Init(IGameConfig *gameconf) {
  CCallManager::gameconf = gameconf;
}

void *CCallManager::GetAddress(const char* signame) {
  void *addr;
  if (signame && !gameconf->GetMemSig(signame, &addr)) {
    smutils->LogError(myself, "Could not locate %s", signame);
    return nullptr;
  }

  if (!addr) {
    smutils->LogError(myself, "Sigscan for %s failed", signame);
    return nullptr;
  }

  return addr;
}

