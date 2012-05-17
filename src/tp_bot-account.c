#include "telepathy-bot.h"

G_DEFINE_TYPE(TpBotAccount, tp_bot_account, G_TYPE_OBJECT);

static void
tp_bot_account_class_init(TpBotAccountClass *self)
{
    g_signal_newv("ready", TP_BOT_TYPE_ACCOUNT, G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
    g_signal_newv("failure", TP_BOT_TYPE_ACCOUNT, G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
}

static void
tp_bot_account_init(TpBotAccount *self)
{
    self->type = "TpBotAccount";
    self->ready_signal_id = g_signal_lookup("ready", TP_BOT_TYPE_ACCOUNT);
    self->failure_signal_id = g_signal_lookup("failure", TP_BOT_TYPE_ACCOUNT);
}


static gboolean
tp_bot_account_is_bot_account(TpAccount *account)
{
  gchar *protocol = tp_account_get_protocol(account);
  gchar *name = tp_account_get_nickname(account);
  if (g_strcmp0(protocol, "local-xmpp") == 0 && g_strcmp0(name,g_getenv("BOTNAME")) == 0) 
    return TRUE;
  else 
    return FALSE;
}




static void
tp_bot_account_account_online_cb(
    GObject *object,
    GAsyncResult *res,
    gpointer user_data)
{
    TpAccount *account = (TpAccount *) object;
    TpBotAccount *self = user_data;  
    GError *error = NULL;

    if (!tp_account_request_presence_finish (account, res, &error))
        FAIL(self, error->message);
    
    g_print("ACCOUNT ONLINE\n");
    g_signal_emit(self, self->ready_signal_id, NULL);

    return;
}




static void
tp_bot_account_connect_account(TpBotAccount *self)
{
    g_print("In connect account.\n");
    tp_account_request_presence_async(self->account,
        TP_CONNECTION_PRESENCE_TYPE_AVAILABLE,
        "ready to server you",
        NULL,
        tp_bot_account_account_online_cb,
        self);
}

static void
tp_bot_account_account_created_cb(
    GObject *object,
    GAsyncResult *res,
	gpointer user_data)
{
    g_print("In account_created_cb.\n");
    TpAccountManager *manager = (TpAccountManager *) object;
    TpBotAccount *self = user_data;  
    GError *error = NULL;

    self->account = tp_account_manager_create_account_finish(manager, res, &error);
    
    if (error != NULL) FAIL(self, error->message);

    g_print("Account created and ready.\n");

    tp_bot_account_connect_account(self);
}

static void
tp_bot_account_account_manager_prepared_cb (
    GObject *object,
    GAsyncResult *res,
    gpointer user_data)
{
    g_print("In account_manager_prepared_cb\n");
    TpAccountManager *manager = (TpAccountManager *) object;
    TpBotAccount *self = user_data;  
    GList *accounts;
  
    GError *error = NULL;
    
      
    if (!tp_proxy_prepare_finish (object, res, &error)) 
        FAIL(self, error->message);
  
    TpAccount *account = NULL;
    for (accounts = tp_account_manager_get_valid_accounts (manager);
         accounts != NULL; accounts = g_list_delete_link (accounts, accounts))
    {
        account = accounts->data;
        if (tp_bot_account_is_bot_account(account))
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
            tp_bot_account_account_created_cb, 
            self);
        g_print("Waiting for account_created_cb\n");
    }
    else
    {
        g_print("Account already exists.\n");
        self->account = g_object_ref(account);
        tp_bot_account_connect_account(self);
    }
}

static void
tp_bot_account_prepare_manager(TpBotAccount *self)
{
    g_print("In prepare_manager\n");
    TpSimpleClientFactory *factory;
  
    self->account_manager = tp_account_manager_dup();
  
    factory = tp_proxy_get_factory (self->account_manager);
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

    tp_proxy_prepare_async(self->account_manager, NULL, tp_bot_account_account_manager_prepared_cb, self);
} 


void tp_bot_account_spin_up(TpBotAccount *account)
{
    tp_bot_account_prepare_manager(account);
}
