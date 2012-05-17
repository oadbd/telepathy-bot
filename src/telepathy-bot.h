#ifndef __TELEPATHY_BOT_H__
#define __TELEPATHY_BOT_H__

#include "config.h"

#include <glib.h>
#include <telepathy-glib/telepathy-glib.h>


#define FAIL(self, details) do { g_print("Fail: %s\n", details); \
                                 g_signal_emit(self, self->failure_signal_id, NULL); \
                                 return; } while (0)

#include "tp_bot-config.h"
#include "tp_bot-signal_handler.h"
#include "tp_bot-account.h"
#include "tp_bot-handler.h"
#include "tp_bot-socket.h"
#include "tp_bot-bot.h"


#endif
