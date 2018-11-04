inherit F_FOOD;

void create()
{
	set_name("大型还阳丹", ({ "big hp pill", "pill" }));
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "颗");
		set("base_value", 200);            // 初期的药，不需要太贵，让玩家买得起
		set("base_weight", 10);
		set("food_stuff", 10);             // 食物量，食品一定要有食物量
		set("long", "一颗吃了可以加快恢复你的精神量。\n");
		set("condition", ([ "hpheal3": ([ "duration": 10 ]) ]) );
	}
	setup();
}
