#ifndef _INCLUDE_LEFT4DOWNTOWN_CALLS_H_
#define _INCLUDE_LEFT4DOWNTOWN_CALLS_H_

#include "extension.h"

#define CALL_DECL_MEMBER(name, ret, args) \
  class name##Class { \
  public: \
    static ret (name##Class::*p##name)args; \
  }; \
  ret (name##Class::*p##name)args = nullptr;
#define CALL_JOIN_MEMBER(name, signame) (*((void **)&p##name) = CCallManager::GetAddress(signame))
#define CALL_INVOKE_MEMBER(obj, name) (reinterpret_cast<name##Class *>(obj)->*p##name)

class CCallManager {
  public:
    static void Init(IGameConfig *gameconf);
    static void *GetAddress(const char *signame);
  private:
    static IGameConfig *gameconf;
};

#endif // _INCLUDE_LEFT4DOWNTOWN_CALLS_H_
