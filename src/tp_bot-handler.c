#include "telepathy-bot.h"

G_DEFINE_TYPE(TpBotHandler, tp_bot_handler, G_TYPE_OBJECT);


static void
tp_bot_handler_class_init(TpBotHandlerClass *self) 
{
    g_signal_newv("ready", TP_BOT_TYPE_HANDLER, G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
    g_signal_newv("failure", TP_BOT_TYPE_HANDLER, G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
}

static void
tp_bot_handler_init(TpBotHandler *self) 
{
    self->ready_signal_id = g_signal_lookup("ready", TP_BOT_TYPE_HANDLER);
    self->failure_signal_id = g_signal_lookup("failure", TP_BOT_TYPE_HANDLER);
}




static void
tp_bot_handler_echo_message (
    TpTextChannel *channel,
    TpMessage *message,
    gboolean pending)
{
    gchar *text;
    gchar *up;

    TpMessage *reply;
    TpChannelTextMessageFlags flags;
    const gchar *comment = "";
  
    text = tp_message_to_text (message, &flags);
  
    if (flags & TP_CHANNEL_TEXT_MESSAGE_FLAG_NON_TEXT_CONTENT)
    {
        comment = " (and some non-text content we don't understand)";
    }
  
    up = g_ascii_strup(text, -1);

    reply = tp_client_message_new_text(TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, up);
    tp_text_channel_send_message_async(channel, reply, 0, NULL, NULL);
  
    g_free(text);
    g_free(up);
    g_object_unref(reply);
}

static void
tp_bot_handler_message_received_cb(
    TpTextChannel *channel,
    TpMessage *message,
    gpointer user_data)
{
    tp_bot_handler_echo_message(channel, message, FALSE);
    tp_text_channel_ack_message_async(channel, message, NULL, NULL);
}

static void
tp_bot_handler_display_pending_messages (TpTextChannel *channel)
{
    GList *messages, *l;
    messages = tp_text_channel_get_pending_messages(channel);
  
    for (l = messages; l != NULL; l = g_list_next(l))
    {
        TpMessage *msg = l->data;
        tp_bot_handler_echo_message(channel, msg, TRUE);

    }
    tp_text_channel_ack_messages_async(channel, messages, NULL, NULL);    
  
    g_list_free(messages);
}

static void
tp_bot_handler_handle_channels_cb (
    TpSimpleHandler *client,
    TpAccount *account,
    TpConnection *connection,
    GList *channels,
    GList *requests_satisfied,
    gint64 user_action_time,
    GList *requests,
    TpHandleChannelsContext *context,
    gpointer user_data)
{
    GList *l;
    TpBotHandler *self = user_data;  

    g_print("Handling channels for our bots account\n");

    for (l = channels; l != NULL; l = g_list_next (l))
	{
        TpChannel *channel = l->data;
        TpTextChannel *text_chan = l->data;
      
        if (!TP_IS_TEXT_CHANNEL(channel))
            continue;
      
        g_print("Handling text channel with %s\n", 
            tp_channel_get_identifier(channel));


        g_signal_connect(channel, "message-received",
            G_CALLBACK(tp_bot_handler_message_received_cb), NULL);
      
        tp_bot_handler_display_pending_messages(text_chan);
	}
}

void tp_bot_handler_spin_up(TpBotHandler *self, TpAccountManager *manager)
{
    GError *error = NULL;
    self->handler = tp_simple_handler_new_with_am(manager, 
        FALSE, FALSE, "BotHandler", FALSE, tp_bot_handler_handle_channels_cb, self, NULL);

    tp_base_client_take_handler_filter(self->handler, 
        tp_asv_new (
            TP_PROP_CHANNEL_CHANNEL_TYPE, G_TYPE_STRING, TP_IFACE_CHANNEL_TYPE_TEXT,
            TP_PROP_CHANNEL_TARGET_HANDLE_TYPE, G_TYPE_UINT, TP_HANDLE_TYPE_CONTACT,
            TP_PROP_CHANNEL_REQUESTED, G_TYPE_BOOLEAN, FALSE,
        NULL)
    );
  
      
    if (!tp_base_client_register(self->handler, &error)) FAIL(self, error->message);
}
