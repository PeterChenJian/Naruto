/*  disable.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob;
    mapping m;
    string sk, sk_m;

    if( !arg || !(ob = SKILL_D(arg)) || ob==SKILL_D("none") )
        return notify_fail("你要停用什么技能？\n");

    if( me->is_busy() )
        return notify_fail("你正在忙著别的事, 无法专心。\n");

//    if (!me->query_learn(arg)) 
    if (!me->query_skill(arg,1)) 
        return notify_fail("你目前不会这项技能。\n");

    if( !function_exists("valid_enable", ob) ) {
        me->map_skill(arg, "none");
        write("你决定暂时停止使用所学有关“" + to_chinese(arg) + "”的技巧。\n");
    } else {
	int c = 0;
	m = me->query_skill_map();
	foreach(sk, sk_m in m) {
	    if( sk_m==arg ) {
		me->map_skill(sk);
		write("你决定停止在“" + to_chinese(sk) + "”方面停止使用所学有关“" + to_chinese(arg) + "”的技巧。\n");
		c++;
	    }
	}
	if( !c ) {
	    write("你决定停止使用所学有关“" + to_chinese(arg) + "”的技巧。\n");
	    me->map_skill(arg, "none");
	}
    }

    return 1;
}

int help()
{
    write(@TEXT
指令格式：disable <技能>

这个指令让你停止使用某种技能。只是停止使用而已，并不会让你遗忘技能。

其他相关指令：enable、skills、abandon
TEXT);
    return 1;
}
