#include "RSGGobi.h"
#if 0
#include "plugin.h"
USER_OBJECT_
RS_GGOBI(getPlugins)(USER_OBJECT_ gobiId)
{

  ggobid *gg = GGOBI_GGOBI(toGGobi(gobiId));
  USER_OBJECT_ ans, names;
  GList *tmp;
  gint n, i;

  n = g_list_length(gg->pluginInstances);

  PROTECT(ans = NEW_LIST(n));
  PROTECT(names = NEW_CHARACTER(n));
  for(i = 0, tmp = gg->pluginInstances; i < n; tmp=tmp->next, i++) {
     PluginInstance *inst = tmp->data;
     SET_VECTOR_ELT(ans, i, toRPointer(tmp->data, "GGobiPluginInstance"));
     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(inst->info->details->name));
  }
  SET_NAMES(ans, names);
  UNPROTECT(2);

  return(ans);
}


static USER_OBJECT_
R_getPluginInfo(GGobiPluginInfo *info)
{
  SEXP klass = MAKE_CLASS("GGobiPluginInfo");
  USER_OBJECT_ ans;

  PROTECT(ans = NEW(klass));
  SET_SLOT(ans, Rf_install("dll"), mkString(info->details->dllName));
  SET_SLOT(ans, Rf_install("name"), mkString(info->details->name));
  SET_SLOT(ans, Rf_install("description"), mkString(info->details->description));
  SET_SLOT(ans, Rf_install("author"), mkString(info->details->author));

  UNPROTECT(1);

  return(ans);
}


USER_OBJECT_
RS_GGOBI(getPluginInfo)(USER_OBJECT_ gobiId)
{
  USER_OBJECT_ ans, names;
  GList *tmp, *plugins;
  gint n, i;
  plugins = GGOBI_getSessionOptions()->info->plugins;
  n = g_list_length(plugins);

  PROTECT(ans = NEW_LIST(n));
  PROTECT(names = NEW_CHARACTER(n));
  for(i = 0, tmp = plugins; i < n; tmp=tmp->next, i++) {
     GGobiPluginInfo *inst = tmp->data;
     SET_VECTOR_ELT(ans, i, R_getPluginInfo(inst));
     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(inst->details->name));
  }
  SET_NAMES(ans, names);
  UNPROTECT(2);

  return(ans);
}
#endif

