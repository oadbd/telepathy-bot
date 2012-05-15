#include "config.h"
#include "telepathy-bot.h"

static void 
shutdown(BotRefs *bot)
{
	g_main_loop_quit (bot->loop);

	g_object_unref (bot->account_manager);
	g_object_unref (bot->account);
	g_main_loop_unref (bot->loop);
}

static void
account_online_cb (GObject *object,
	GAsyncResult *res,
	gpointer user_data)
{
	TpAccount *account = (TpAccount *) object;
	BotRefs *bot = user_data;  
	GError *error = NULL;
	
	if (!tp_account_request_presence_finish (account, res, &error))
    {
		g_print ("Error bringing account online: %s\n", error->message);
		shutdown(bot);
		return;
    }
    
	g_print("ACCOUNT ONLINE\n");
}

static void
connect_account (TpAccount *account, 
	BotRefs *bot)
{
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

	gchar *protocol;
	gchar *name;
	gchar normname[256];
	
	GError *error = NULL;


	if (!tp_proxy_prepare_finish (object, res, &error))
    {
		g_print ("Error preparing AM: %s\n", error->message);
		shutdown(bot);
		return;
    }
  
	TpAccount *account = NULL;
	for (accounts = tp_account_manager_get_valid_accounts (manager);
		accounts != NULL; accounts = g_list_delete_link (accounts, accounts))
    {
		account = accounts->data;
		protocol = tp_account_get_protocol(account);
		name = tp_account_get_nickname(account);
      
		if (protocol == "local-xmpp" && name == g_getenv("BOTNAME")) 
		{
			break;
		}
		account = NULL;
	}

	if (account == NULL)
	{
		g_print("Ready to create new account\n");
		g_snprintf(&normname, sizeof(normname), "%s@%s", g_getenv("BOTNAME"), g_getenv("HOSTNAME"));
		g_print("Creating new account\n");
		tp_account_manager_create_account_async(manager,
			"salut",
			"local-xmpp",
			"Telepathy BOT",
			tp_asv_new(
				"nickname", G_TYPE_STRING, g_getenv("BOTNAME"),
				"first-name", G_TYPE_STRING, g_getenv("BOTNAME"),
				"last-name", G_TYPE_STRING, g_getenv("BOTNAME"),
			NULL),
			tp_asv_new(
				"Nickname", G_TYPE_STRING, g_getenv("BOTNAME"),
				"NormalizedName", G_TYPE_STRING, &normname,
			NULL),
			account_created_cb,
			bot
		);
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
	bot.account_manager = prepare_manager(&bot);
	g_main_loop_run(bot.loop);
	return 0;
}
