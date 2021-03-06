/*  backup.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    int last_backup, delta_t;
    string save_file;

    SECURED_COMMAND;

    seteuid(getuid());

    if( (last_backup = me->query_temp("last_backup"))
    &&  (delta_t = (time()/60 - last_backup)) < 5 ) {
	write( sprintf("不用急，%d 分钟前你才刚备份过，%d 分钟后再试吧。\n",
		delta_t, (5-delta_t)) );
	return 1;
    }

#ifdef SAVE_USER
    save_file = me->query_save_file();
    if( cp(save_file, save_file+".backup") ) {
	write("备份完毕。\n");
	me->set_temp("last_backup", (int)(time()/60) );
    }
    else
	write("备份失败！\n");
    return 1;
#else
    return notify_fail("系统目前不储存使用者资料。\n");
#endif
}

int help(object me)
{
    write(@HELP
指令格式﹕backup

把你辛苦奋斗的结果备份起来。为了提升系统效率，目前两次 backup 之时间间距最
短为五分钟。

这个指令的目的是为了避免使用者遭受到 <非设计者原意且无法避免的因素> ，而造
成无法弥补之重大损失。

当大巫师或神认为有必要时，会将你的备份档取回。
HELP
    );
    return 1;
}
