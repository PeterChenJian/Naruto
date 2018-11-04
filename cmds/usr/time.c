/*  time.c */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    seteuid(getuid());
    write("游戏时间：" + NATURE_D->game_time() + "。\n");
    write("真实时间：" + CHINESE_D->chinese_date(time()) + "\n");
    write("火影忍者已经执行了" + CHINESE_D->chinese_period(uptime()) + "。\n");
    return 1;
}

int help(object me)
{
     write(@HELP
指令格式: time

这个指令让你(你)知道现在的时间。
HELP);
    return 1;
}
