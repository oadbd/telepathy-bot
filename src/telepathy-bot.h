#ifndef TELEPATHY_BOT_H
#define TELEPATHY_BOT_H
#include <glib.h>
#include <telepathy-glib/telepathy-glib.h>

typedef struct _BotRefs 
{
  TpAccount *account;
  TpAccountManager *account_manager;
  TpConnection *connection;
  //  TpDBusDaemon *dbus;
  GMainLoop *loop;
} BotRefs;

#endif
