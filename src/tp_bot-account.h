#ifndef __TP_BOT_ACCOUNT_H__
#define __TP_BOT_ACCOUNT_H__

#include "telepathy-bot.h"

#define TP_BOT_TYPE_ACCOUNT             (tp_bot_account_get_type ())
#define TP_BOT_ACCOUNT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_ACCOUNT, TpBotAccount))
#define TP_BOT_IS_ACCOUNT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_ACCOUNT))
#define TP_BOT_ACCOUNT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_ACCOUNT, TpBotAccountClass))
#define TP_BOT_IS_ACCOUNT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_ACCOUNT))
#define TP_BOT_ACCOUNT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_ACCOUNT, TpBotAccountClass))

typedef struct _TpBotAccount        TpBotAccount;
typedef struct _TpBotAccountClass   TpBotAccountClass;

struct _TpBotAccount
{
	GObject parent_instance;
    gchar *type;

    guint ready_signal_id;
	guint failure_signal_id;
	
	gpointer *bot;
	  
	TpAccount *account;
	TpAccountManager *account_manager;
  	TpConnection *connection;
};

struct _TpBotAccountClass
{
  GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_ACCOUNT */
GType tp_bot_account_get_type (void);

void tp_bot_account_spin_up (TpBotAccount*);

#endif
