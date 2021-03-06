/*  abandon.c

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

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    SECURED_COMMAND;

    if( !arg ) return notify_fail("你要放弃继续学习什么技能？\n");

    if( sscanf(arg, "-c %s", arg)==1 )
        me->abandon_skill(arg, 1);
    else
        me->abandon_skill(arg);

    write("Ok.\n");
    return 1;
}

int help()
{
    write(@TEXT
指令格式：abandon <技能名称>

这个指令让你“中止”学习一项技能，这个技能往后不会再经由使用或学习而进步
。但这并不表示你会完全忘记这项技能。中止学习的技能仍然可以使用，只是停止
学习而已。

如果有需要，你可以用 abandon -c <技能名称> 来恢复被你中止的技能。

其它相关指令：enable、disable、skills
TEXT);
    return 1;
}
