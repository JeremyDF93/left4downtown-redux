#ifndef _INCLUDE_LEFT4DOWNTOWN_EXTENSION_PROPER_H_
#define _INCLUDE_LEFT4DOWNTOWN_EXTENSION_PROPER_H_

#include "smsdk_ext.h"
#include "CDetour/detours.h"
#include "CCall/calls.h"
#include <IBinTools.h>
#include <ISDKTools.h>
#include "vglobals.h"

class Left4downtown :
  public SDKExtension,
  public IPluginsListener
{
public:
  /**
   * @brief This is called after the initial loading sequence has been processed.
   *
   * @param error   Error message buffer.
   * @param maxlen  Size of error message buffer.
   * @param late    Whether or not the module was loaded after map load.
   * @return        True to succeed loading, false to fail.
   */
  virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);

  /**
   * @brief This is called right before the extension is unloaded.
   */
  virtual void SDK_OnUnload();

  /**
   * @brief This is called once all known extensions have been loaded.
   * Note: It is is a good idea to add natives here, if any are provided.
   */
  virtual void SDK_OnAllLoaded();

  /**
   * @brief Called when the pause state is changed.
   */
  //virtual void SDK_OnPauseChange(bool paused);

  /**
   * @brief this is called when Core wants to know if your extension is working.
   *
   * @param error   Error message buffer.
   * @param maxlen  Size of error message buffer.
   * @return        True if working, false otherwise.
   */
  //virtual bool QueryRunning(char *error, size_t maxlen);
public:
#if defined SMEXT_CONF_METAMOD
  /**
   * @brief Called when Metamod is attached, before the extension version is called.
   *
   * @param error     Error buffer.
   * @param maxlen    Maximum size of error buffer.
   * @param late      Whether or not Metamod considers this a late load.
   * @return          True to succeed, false to fail.
   */
  virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);

  /**
   * @brief Called when Metamod is detaching, after the extension version is called.
   * NOTE: By default this is blocked unless sent from SourceMod.
   *
   * @param error     Error buffer.
   * @param maxlen    Maximum size of error buffer.
   * @return          True to succeed, false to fail.
   */
  virtual bool SDK_OnMetamodUnload(char *error, size_t maxlen);

  /**
   * @brief Called when Metamod's pause state is changing.
   * NOTE: By default this is blocked unless sent from SourceMod.
   *
   * @param paused    Pause state being set.
   * @param error     Error buffer.
   * @param maxlen    Maximum size of error buffer.
   * @return          True to succeed, false to fail.
   */
  //virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlen);
#endif
public: // IPluginsListener
  // virtual void OnPluginLoaded(IPlugin *plugin);
  // virtual void OnPluginUnloaded(IPlugin *plugin);
public:
  void OnServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
};

extern IGameConfig *g_pGameConf;
extern IBinTools *g_pBinTools;
extern ISDKTools *g_pSDKTools;

extern ICvar *icvar;
extern CGlobalVars *gpGlobals;

extern sp_nativeinfo_t g_MyNatives[];

//void OnServerActivate(edict_t *pEdictList, int edictCount, int clientMax);

#endif // _INCLUDE_LEFT4DOWNTOWN_EXTENSION_PROPER_H_
