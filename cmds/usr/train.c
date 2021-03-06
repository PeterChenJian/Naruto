/* train.c 训练技能指令 by -Acme- */

inherit F_CLEAN_UP;

int do_train(object me, object trainer, string skill, int count)
{
	if( !objectp(me) || !objectp(trainer) ) return notify_fail("这里没有这个人。\n");
	
    // 不是训练师
	if( !trainer->is_trainer() ) return notify_fail("这个人似乎不想教你。\n");
	
    // 如果是在区域环境中，不同座标，不会有动作
	if( environment(me)->is_area() )
		if( !area_environment(me, trainer) ) return notify_fail("这里没有这个人。\n");

    // all: 不分忍者村, 否则看是否相同的忍者村
    if( trainer->query("guild") != "all" &&  trainer->query("guild") != me->query("guild") )
        return notify_fail("对方似乎不想教你。\n");
	
	return trainer->train_skill(me, skill, count);
}

int main(object me, string arg)
{
	int count;
	object trainer;
	string skill, who;
	
	if( !arg ) return notify_fail("你想向谁讨教并训练你的技能？\n");
	
	if( sscanf(arg, "%s from %s for %d", skill, who, count) == 3 ) {
        if( !objectp(trainer = present(who, environment(me))) )
            return notify_fail("你的附近没有这个人。\n");
	} else if( sscanf(arg, "%s from %s", skill, who) == 2 ) {
        if( !objectp(trainer = present(who, environment(me))) )
            return notify_fail("你的附近没有这个人。\n");
		count = 1;
	}

    if( count <= 0 ) count = 1;

    return do_train(me, trainer, skill, count);
}

int help()
{
    write(@TEXT
指令格式﹕train <技能名称> from <人物> [for <次数>]

这个指令可以向某人讨教并训练你的技能，不过对方愿不愿意教你，
并不是你可以决定的。一般人是不会轻易的教导别人武功，就算是愿
意教导，也通常不会倾囊相授。
TEXT);
    return 1;
}
