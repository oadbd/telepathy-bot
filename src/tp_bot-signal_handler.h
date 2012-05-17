#ifndef __TP_BOT_SIGNAL_HANDLER_H__
#define __TP_BOT_SIGNAL_HANDLER_H__

#include "telepathy-bot.h"

#define TP_BOT_TYPE_SIGNAL_HANDLER             (tp_bot_account_get_type ())
#define TP_BOT_SIGNAL_HANDLER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_SIGNAL_HANDLER, TpBotSignalHandler))
#define TP_BOT_IS_SIGNAL_HANDLER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_SIGNAL_HANDLER))
#define TP_BOT_SIGNAL_HANDLER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_SIGNAL_HANDLER, TpBotSignalHandlerClass))
#define TP_BOT_IS_SIGNAL_HANDLER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_SIGNAL_HANDLER))
#define TP_BOT_SIGNAL_HANDLER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_SIGNAL_HANDLER, TpBotSignalHandlerClass))

typedef struct _TpBotSignalHandler        TpBotSignalHandler;
typedef struct _TpBotSignalHandlerClass   TpBotSignalHandlerClass;

struct _TpBotSignalHandler
{
  GObject parent_instance;
  gpointer *bot;
};

struct _TpBotSignalHandlerClass
{
  GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_SIGNAL_HANDLER */
GType tp_bot_signal_handler_get_type (void);


void tp_bot_signal_handler_setup(TpBotSignalHandler *self);

#endif
