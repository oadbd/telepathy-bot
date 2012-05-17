#include "telepathy-bot.h"

G_DEFINE_TYPE(TpBotBot, tp_bot_bot, G_TYPE_OBJECT);

static void
tp_bot_bot_class_init(TpBotBotClass *self)
{

}

static void 
tp_bot_bot_init(TpBotBot *self)
{
	g_print("Initializing tp_bot");
	self->type = "TpBotBot";
	
	self->config = g_object_new(TP_BOT_TYPE_CONFIG, NULL);
	self->account = g_object_new(TP_BOT_TYPE_ACCOUNT, NULL);
	self->handler = g_object_new(TP_BOT_TYPE_HANDLER, NULL);
	self->socket = g_object_new(TP_BOT_TYPE_SOCKET, NULL);
	self->signal_handler = g_object_new(TP_BOT_TYPE_SIGNAL_HANDLER, NULL);
	
	self->config->bot = self;
	self->account->bot = self;
	self->handler->bot = self;
	self->socket->bot = self;
	self->signal_handler->bot = self;
}

static void
tp_bot_bot_account_ready_cb (
    TpBotAccount *account,
    TpBotBot *self)
{
    g_print("Account is ready. Spinning up handler\n");
    tp_bot_handler_spin_up(self->handler, account->account_manager);
}

static void tp_bot_bot_connect_signals(TpBotBot *self) 
{
    g_signal_connect(self->account, "ready", 
        G_CALLBACK(tp_bot_bot_account_ready_cb), self);

}

void tp_bot_bot_run(TpBotBot *bot)
{
    tp_bot_bot_connect_signals(bot);
    tp_bot_account_spin_up(bot->account);
    g_main_loop_run(bot->main_loop);
}
