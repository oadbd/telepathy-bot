#ifndef __TP_BOT_HANDLER_H__
#define __TP_BOT_HANDLER_H__

#include "telepathy-bot.h"

#define TP_BOT_TYPE_HANDLER             (tp_bot_account_get_type ())
#define TP_BOT_HANDLER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_HANDLER, TpBotHandler))
#define TP_BOT_IS_HANDLER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_HANDLER))
#define TP_BOT_HANDLER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_HANDLER, TpBotHandlerClass))
#define TP_BOT_IS_HANDLER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_HANDLER))
#define TP_BOT_HANDLER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_HANDLER, TpBotHandlerClass))

typedef struct _TpBotHandler        TpBotHandler;
typedef struct _TpBotHandlerClass   TpBotHandlerClass;

struct _TpBotHandler
{
    GObject parent_instance;
    
    guint ready_signal_id;
	guint failure_signal_id;
	
    gpointer *bot;

    TpBaseClient *handler;
};

struct _TpBotHandlerClass
{
    GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_HANDLER */
GType tp_bot_handler_get_type (void);

void tp_bot_handler_spin_up (TpBotHandler *self, TpAccountManager *manager);
#endif
