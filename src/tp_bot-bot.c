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

