/*  nick.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    if( !arg ) return notify_fail("你要替自己取什么绰号﹖\n");

    arg = color(arg);

    if( strlen(uncolor(arg)) > 14 )
	    return notify_fail("你的绰号太长了﹐想一个短一点的、响亮一点的。\n");

    seteuid(getuid());
    me->set("nickname", arg);    // use arg + NOR if colorful nick is open
    write("Ok。\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : nick <外号, 绰号>
 
这个指令可以让你为自己取一个响亮的名号或头衔。
绰号可以使用 ANSI 颜色控制码，请输入 help ansi
HELP);
        return 1;
}
