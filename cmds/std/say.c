// say.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object *ob;
	if (!arg) return notify_fail("你要说什么﹖\n");

	write("你说道﹕" + HIG + arg + "\n" NOR);

	// The mudlib interface of say
	// 解谜用相关回应, 增加区域判断 by -Acme-
	if( environment(me)->is_area() ) {
		mapping info;
		info = me->query("area_info");
		ob = environment(me)->query_inventory(info["x_axis"], info["y_axis"]);
        tell_area(environment(me), info["x_axis"], info["y_axis"], me->name() + "说道﹕" + HIG + arg + "\n" NOR, ({me}));
	} else {
        message("sound", me->name() + "说道﹕" + HIG + arg + "\n" NOR, environment(me), me);
	    ob = all_inventory(environment(me));
	}

//    message("sound", me->name() + "说道﹕" + HIG + arg + "\n" NOR, environment(me), me);
	
	ob -= ({ me });
	ob->relay_say(me, arg);

	return 1;
}

int help (object me)
{
	write(@HELP
指令格式: say <讯息>
 
说话﹐所有跟你在同一个房间的人都会听到你说的话。
 
注: 本指令可用 ' 取代.
 
HELP
	);
	return 1;
}
