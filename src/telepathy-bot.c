#include "telepathy-bot.h"

int
main(int argc, char **argv)
{
	TpBotBot *bot = g_object_new(TP_BOT_TYPE_BOT, NULL);
	g_object_unref(bot);
	return 0;
}
