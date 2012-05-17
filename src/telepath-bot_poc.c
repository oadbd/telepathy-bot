#include "config.h"
#include "telepathy-bot.h"

typedef struct _BotRefs 
{
  TpAccount *account;
  TpAccountManager *account_manager;
  TpConnection *connection;
  GMainLoop *loop;
} BotRefs;

static void 
shutdown(BotRefs *bot)
{
  g_main_loop_quit (bot->loop);
  
  g_object_unref (bot->account_manager);
  g_object_unref (bot->account);
  g_main_loop_unref (bot->loop);
}

static gboolean
is_bot_account(TpAccount *account)
{
  gchar *protocol = tp_account_get_protocol(account);
  gchar *name = tp_account_get_nickname(account);
  if (g_strcmp0(protocol, "local-xmpp") == 0 && g_strcmp0(name,g_getenv("BOTNAME")) == 0) 
    return TRUE;
  else 
    return FALSE;
}

static void
echo_message (TpTextChannel *channel,
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
  if (pending)
    g_print("pending '%s' %s\n", text, comment);
  else 
    g_print("received '%s' %s\n", text, comment);
  
  up = g_ascii_strup(text, -1);
  g_print("send: %s\n", up);

  reply = tp_client_message_new_text(TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, up);
  tp_text_channel_send_message_async(channel, reply, 0, NULL, NULL);
  
  g_free(text);
  g_free(up);
  g_object_unref(reply);
}

static void
message_received_cb (TpTextChannel *channel,
		     TpMessage *message,
		     gpointer user_data)
{
  echo_message(channel, message, FALSE);
  tp_text_channel_ack_message_async(channel, message, NULL, NULL);
}

static void
display_pending_messages (TpTextChannel *channel)
{
  GList *messages, *l;
  messages = tp_text_channel_get_pending_messages(channel);
  
  for (l = messages; l != NULL; l = g_list_next(l))
    {
      TpMessage *msg = l->data;
      echo_message(channel, msg, TRUE);
    }
  tp_text_channel_ack_message_async(channel, messages, NULL, NULL);
  
  g_list_free(messages);
}

static void
handle_channels_cb (TpSimpleHandler *self,
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
  BotRefs *bot = user_data;  

  //  if (is_bot_account(account))
  //    {
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
			   G_CALLBACK(message_received_cb), NULL);
      
	  display_pending_messages(text_chan);
	}
      tp_handle_channels_context_accept(context);
      //    }
}

static void
accept_with_cb(GObject *source,
	       GAsyncResult *result,
	       gpointer user_data)
{
  TpChannelDispatchOperation *cdo = TP_CHANNEL_DISPATCH_OPERATION (source);
  GError *error;
  if (!tp_channel_dispatch_operation_handle_with_finish (cdo, result, &error))
    {
      g_print ("AcceptWith() failed: %s\n", error->message);
      g_error_free (error);
      return;
    }
  
  g_print ("AcceptWith() succeeded\n");
}

static void
add_dispatch_operation_cb(TpSimpleApprover *self,
			  TpAccount *account,
			  TpConnection *connection,
			  GList *channels,
			  TpChannelDispatchOperation *cdo,
			  TpAddDispatchOperationContext *context,
			  gpointer user_data)
{
  g_print("Approving batch of channels\n");

  //  tp_add_dispatch_operation_context_accept(context);
  //  tp_channel_dispatch_operation_handle_with_async(cdo, NULL, NULL, NULL);
}
			  


static void
account_online_cb (GObject *object,
		   GAsyncResult *res,
		   gpointer user_data)
{
  TpAccount *account = (TpAccount *) object;
  BotRefs *bot = user_data;  
  GError *error = NULL;

  TpBaseClient *handler;
  TpBaseClient *approver;
  TpAutomaticClientFactory *factory;


  if (!tp_account_request_presence_finish (account, res, &error))
    {
      g_print ("Error bringing account online: %s\n", error->message);
      g_error_free(error);
      shutdown(bot);
      return;
    }
  
  g_print("ACCOUNT ONLINE\n");
  


  handler = tp_simple_handler_new_with_am(bot->account_manager, 
      FALSE, FALSE,
      "BotHandler", FALSE, 
      handle_channels_cb,
      bot, NULL);

  tp_base_client_take_handler_filter(handler, 
      tp_asv_new (
		  TP_PROP_CHANNEL_CHANNEL_TYPE, G_TYPE_STRING, TP_IFACE_CHANNEL_TYPE_TEXT,
		  TP_PROP_CHANNEL_TARGET_HANDLE_TYPE, G_TYPE_UINT, TP_HANDLE_TYPE_CONTACT,
		  TP_PROP_CHANNEL_REQUESTED, G_TYPE_BOOLEAN, FALSE,
		  NULL)
      );
  
  
  if (!tp_base_client_register(handler, &error))
    {
      g_warning("Failed to register Handler: %s\n", error->message);
      g_error_free(error);
      shutdown(bot);
      return;
    }

  /*  
  approver = tp_simple_approver_new_with_am(bot->account_manager,
      "BotApprover", 
      FALSE,
      add_dispatch_operation_cb,
      bot,
      NULL);    

  tp_base_client_take_approver_filter(approver, 
     tp_asv_new (
		 TP_PROP_CHANNEL_CHANNEL_TYPE, G_TYPE_STRING, TP_IFACE_CHANNEL_TYPE_TEXT,
		 TP_PROP_CHANNEL_TARGET_HANDLE_TYPE, G_TYPE_UINT, TP_HANDLE_TYPE_CONTACT,
		 TP_PROP_CHANNEL_REQUESTED, G_TYPE_BOOLEAN, FALSE,
		 NULL)
     );

  if (!tp_base_client_register(approver, &error))
    {
      g_warning("Failed to register Approver: %s\n", error->message);
      g_error_free(error);
      shutdown(bot);
      return;
    }
  */  


  g_print("Waiting for channels\n");
  return;
}

static void
connect_account (TpAccount *account, 
		 BotRefs *bot)
{

  //factory = tp_automatic_client_factory_new(dbus);
  //handler = tp_simple_handler_with_new_factory(factory,

  g_print("In connect account.\n");
  tp_account_request_presence_async(account,
      TP_CONNECTION_PRESENCE_TYPE_AVAILABLE,
      "ready to server you",
      NULL,
      account_online_cb,
      bot);
}

static void
account_created_cb (GObject *object,
		    GAsyncResult *res,
		    gpointer user_data)
{
  g_print("In account_created_cb.\n");
  TpAccountManager *manager = (TpAccountManager *) object;
  BotRefs *bot = user_data;  
  GError *error = NULL;

  TpAccount *account = tp_account_manager_create_account_finish(manager, res, &error);

  if (error != NULL)
    {
      g_print ("Error creating account: %s\n", error->message);
      g_error_free(error);
      shutdown(bot);
      return;
    }
  g_print("Account created and ready.\n");

  bot->account = g_object_ref(account);
  connect_account(bot->account, bot);
}

static void
account_manager_prepared_cb (GObject *object,
			     GAsyncResult *res,
			     gpointer user_data)
{
  g_print("In account_manager_prepared_cb\n");
  TpAccountManager *manager = (TpAccountManager *) object;
  BotRefs *bot = user_data;  
  GList *accounts;
  
  GError *error = NULL;
  
  
  if (!tp_proxy_prepare_finish (object, res, &error))
    {
      g_print ("Error preparing AM: %s\n", error->message);
      g_error_free(error);
      shutdown(bot);
      return;
    }
  
  TpAccount *account = NULL;
  for (accounts = tp_account_manager_get_valid_accounts (manager);
       accounts != NULL; accounts = g_list_delete_link (accounts, accounts))
    {
      account = accounts->data;
      if (is_bot_account(account))
	{
	  g_print("We have our account\n");
	  break;
	}
      account = NULL;
    }
  
  if (account == NULL)
    {
      g_print("Ready to create new account\n");
      g_print("Creating new account\n");
      tp_account_manager_create_account_async(manager,
	      "salut",
	      "local-xmpp",
	      "People nearby",
	      tp_asv_new(
	          "nickname", G_TYPE_STRING, g_getenv("BOTNAME"),
		  "first-name", G_TYPE_STRING, g_getenv("BOTNAME"),
		  "last-name", G_TYPE_STRING, g_getenv("BOTNAME"),
		  NULL),
	      tp_asv_new(
 		  TP_PROP_ACCOUNT_ENABLED, G_TYPE_BOOLEAN, TRUE,
		  TP_PROP_ACCOUNT_NICKNAME, G_TYPE_STRING, g_getenv("BOTNAME"),
		  NULL),
	      account_created_cb,
	      bot);
      g_print("Waiting for account_created_cb\n");
    }
  else
    {
      g_print("Account already exists.\n");
      bot->account = g_object_ref(account);
      connect_account(bot->account, bot);
    }
}

static TpAccountManager*
prepare_manager(BotRefs *bot)
{
  g_print("In prepare_manager\n");
  TpAccountManager *manager;
  TpSimpleClientFactory *factory;
  
  manager = tp_account_manager_dup ();
  bot->account_manager = manager;

  factory = tp_proxy_get_factory (manager);
  tp_simple_client_factory_add_account_features_varargs (factory,
      TP_ACCOUNT_FEATURE_CONNECTION,
      0);
  tp_simple_client_factory_add_connection_features_varargs (factory,
      TP_CONNECTION_FEATURE_CONTACT_LIST,
      0);
  tp_simple_client_factory_add_contact_features_varargs (factory,
      TP_CONTACT_FEATURE_ALIAS,
      TP_CONTACT_FEATURE_CONTACT_GROUPS,
      TP_CONTACT_FEATURE_INVALID);

  tp_proxy_prepare_async (manager, NULL, account_manager_prepared_cb, bot);
  
  return manager;
}  

int
main (int argc,
      char **argv)
{
  BotRefs bot;
  g_type_init();
  bot.loop = g_main_loop_new(NULL, FALSE);

  GError *error = NULL;
  //  bot.dbus = tp_dbus_daemon_new(&error);
  if (error != NULL)
    {
      g_print("Failed to get connection to DBus Daemon: %s\n", error->message);
      g_error_free(error);
      shutdown(&bot);
    } 
  else 
    {
      prepare_manager(&bot);
      g_main_loop_run(bot.loop);
    }
  
  return 0;
}
