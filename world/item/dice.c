// dice.c

inherit ITEM;

void create()
{
	set_name("骰子", ({ "dice"}));
	set_weight(3);
	if( !clonep() ) {
		set("unit", "粒");
		set("value", 5);
		set("long",
			"一粒骰子，你可以用 roll 掷骰子玩。\n");
	}
	setup();
}

void init()
{
	add_action("do_roll", ({ "roll", "throw" }));
}

int do_roll(string arg)
{
	if( !id(arg) ) return 0;

    if( environment(this_player())->is_area() )
    	if( !area_environment(this_player(), this_object()) ) return 0;

	message_vision("$N拿出骰子一掷，骰子滚了滚停下来，结果是个" + (random(6)+1) + "点。\n", this_player());
	return 1;
}
