#ifndef __TP_BOT_SOCKET_H__
#define __TP_BOT_SOCKET_H__

#include "telepathy-bot.h"

#define TP_BOT_TYPE_SOCKET             (tp_bot_account_get_type ())
#define TP_BOT_SOCKET(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TP_BOT_TYPE_SOCKET, TpBotSocket))
#define TP_BOT_IS_SOCKET(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TP_BOT_TYPE_SOCKET))
#define TP_BOT_SOCKET_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TP_BOT_TYPE_SOCKET, TpBotSocketClass))
#define TP_BOT_IS_SOCKET_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TP_BOT_TYPE_SOCKET))
#define TP_BOT_SOCKET_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TP_BOT_TYPE_SOCKET, TpBotSocketClass))

typedef struct _TpBotSocket        TpBotSocket;
typedef struct _TpBotSocketClass   TpBotSocketClass;

struct _TpBotSocket
{
  GObject parent_instance;
  gpointer *bot;
};

struct _TpBotSocketClass
{
  GObjectClass parent_class;
};

/* used by TP_BOT_TYPE_SOCKET */
GType tp_bot_socket_get_type (void);

#endif
