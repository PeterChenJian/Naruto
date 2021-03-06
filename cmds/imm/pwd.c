/*  pwd.c

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

int main(object me, string arg)
{
    write("目前工作目录：" + (string)me->query("cwd") +  "\n");
    write("目前工作档案：" + (string)me->query("cwf") +  "\n");
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : pwd
 
这个指令会显示出你目前的工作目录(cwd)及工作档案(cwf)，当你使用 ls 或其他
与档案有关的指令时，会先搜寻目前的工作目录。

工作档案是你上一次使用 update 所更新的档案。 
HELP
    );
    return 1;
}
