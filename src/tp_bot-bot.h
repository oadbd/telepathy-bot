#ifndef __TP_BOT_BOT_H__
#define __TP_BOT_BOT_H__

#include <glib-object.h>
#include "telepathy-bot.h"

#define TP_BOT_TYPE_BOT             (tp_bot_bot_get_type ())
#define TP_BOT_BOT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_BOT, TpBotBot))
#define TP_BOT_IS_BOT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_BOT))
#define TP_BOT_BOT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_BOT, TpBotBotClass))
#define TP_BOT_IS_BOT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_BOT))
#define TP_BOT_BOT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_BOT, TpBotBotClass))

typedef struct _TpBotBot        TpBotBot;
typedef struct _TpBotBotClass   TpBotBotClass;

struct _TpBotBot
{
  GObject parent_instance;

  TpBotConfig *config;
  TpBotAccount *account;
  TpBotHandler *handler;
  TpBotSocket *socket;
  TpBotSignalHandler *signal_handler;
};

struct _TpBotBotClass
{
  GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_BOT */
GType tp_bot_bot_get_type (void);

/*
 * Method definitions.
 */

#endif
