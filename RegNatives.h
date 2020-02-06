#ifndef _INCLUDE_LEFT4DOWNTOWN_REGNATIVES_H_
#define _INCLUDE_LEFT4DOWNTOWN_REGNATIVES_H_

#include <am-vector.h>

class RegNatives {
public:
  void Register(ICallWrapper *pWrapper);
  void UnregisterAll();
private:
  ke::Vector<ICallWrapper *> m_Natives;
};

extern RegNatives g_RegNatives;

#endif //_INCLUDE_LEFT4DOWNTOWN_REGNATIVES_H_
