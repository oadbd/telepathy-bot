#include "telepathy-bot.h"

int
main(int argc, char **argv)
{

    g_type_init();
    TpBotBot *bot = g_object_new(TP_BOT_TYPE_BOT, NULL);
    
    bot->main_loop = g_main_loop_new(NULL, FALSE);
    
    tp_bot_bot_run(bot);
    return 0;
}
