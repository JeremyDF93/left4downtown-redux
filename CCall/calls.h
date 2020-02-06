#ifndef _INCLUDE_LEFT4DOWNTOWN_CALLS_H_
#define _INCLUDE_LEFT4DOWNTOWN_CALLS_H_

#include "extension.h"

#define CALL_DECL_MEMBER(cls, func, ret, args) \
  class func##Class : public cls { \
  public: \
    static ret (cls::*p##func)args; \
  }; \
  ret (cls::*p##func)args = nullptr;
#define CALL_JOIN_MEMBER(func, signame) (*((void **)&p##func) = CCallManager::GetAddress(signame))
#define CALL_INVOKE_MEMBER(obj, func) (obj->*p##func)

class CCallManager {
  public:
    static void Init(IGameConfig *gameconf);
    static void *GetAddress(const char *signame);
  private:
    static IGameConfig *gameconf;
};

#endif // _INCLUDE_LEFT4DOWNTOWN_CALLS_H_
