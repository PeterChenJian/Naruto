/*  summon.c

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

int main(object me, string str)
{
    object ob;
    int i;

    seteuid(getuid(me));
    if(!str) return notify_fail("指令格式: summon <使用者>\n");
    ob = LOGIN_D->find_body(str);
    if(!ob) return notify_fail("线上的使用者中没有这个人。\n");

    if( environment(me)->is_area() ) {
    	mapping info;
    	info = me->query("area_info");
    	if( !info ) return 1;

		if( environment(ob)->is_area() )
	   		tell_area(environment(ob), ob->query("area_info/x_axis_old"), ob->query("area_info/y_axis"), "天空中伸出一只大手把"+ ob->name() + "抓了起来﹐然后不见了。\n", ({ob}));
	    else tell_room(environment(ob), "天空中伸出一只大手把"+ ob->name() + "抓了起来﹐然后不见了。\n", ob);

	    if( area_move(environment(me), ob, info["x_axis"], info["y_axis"]) ) 
       		tell_area(environment(me), info["x_axis"], info["y_axis"], ob->name() + "突然出现在你眼前。\n", ({ob,me}));

    } else {
        if( environment(ob)->is_area() )
    		tell_area(environment(me), ob->query("area_info/x_axis"), ob->query("area_info/y_axis"), "天空中伸出一只大手把"+ ob->name() + "抓了起来﹐然后不见了。\n", ({ob}));
	    else tell_room(environment(ob), "天空中伸出一只大手把"+ ob->name() + "抓了起来﹐然后不见了。\n", ob);
		if( ob->move(environment(me)) )
			tell_room(environment(ob), ob->name() + "突然出现在你眼前。\n", ({me,ob}) );
    }

    tell_object(me, "你把" + ob->name() + "抓到你的面前。\n");
    tell_object(ob,"一只手把你抓了起来﹐你眼前一阵黑 ....\n");
    tell_object(ob,".... 醒来时发现是" + me->name() + "把你弄过来的。\n");

    log_file("static/SUMMON", sprintf("%s(%s) summon %s(%s) 到 %s%s at %s\n",
				me->name(1),
				geteuid(me),
				ob->name(1),
				geteuid(ob),
				file_name(environment(me)),
		        environment(me)->is_area() ? "(" + me->query("area_info/x_axis") + "," + me->query("area_info/y_axis") + ")" : "",
				ctime(time()) ) );

    // ok ..
    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : summon <某人>

此指令可让你(你)将某人抓到你面前。
HELP
    );
    return 1;
}