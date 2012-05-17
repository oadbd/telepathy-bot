#ifndef __TP_BOT_CONFIG_H__
#define __TP_BOT_CONFIG_H__

#include <glib-object.h>
#include "telepathy-bot.h"

#define TP_BOT_TYPE_CONFIG             (tp_bot_account_get_type ())
#define TP_BOT_CONFIG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_CONFIG, TpBotConfig))
#define TP_BOT_IS_CONFIG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_CONFIG))
#define TP_BOT_CONFIG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_CONFIG, TpBotConfigClass))
#define TP_BOT_IS_CONFIG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_CONFIG))
#define TP_BOT_CONFIG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_CONFIG, TpBotConfigClass))

typedef struct _TpBotConfig        TpBotConfig;
typedef struct _TpBotConfigClass   TpBotConfigClass;

struct _TpBotConfig
{
  GObject parent_instance;
  gpointer *bot;
};

struct _TpBotConfigClass
{
  GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_CONFIG */
GType tp_bot_config_get_type (void);

/*
 * Method definitions.
 */


#endif
