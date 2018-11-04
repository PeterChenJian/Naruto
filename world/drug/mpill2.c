inherit F_FOOD;

void create()
{
	set_name("中型天地丹", ({ "middle mp pill", "pill" }));
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "颗");
		set("base_value", 80);             // 初期的药，不需要太贵，让玩家买得起
		set("base_weight", 10);
		set("food_stuff", 5);              // 食物量，食品一定要有食物量
		set("long", "一颗吃了可以加快恢复你的查克拉。\n");
		set("condition", ([ "mpheal2": ([ "duration": 10 ]) ]) );
	}
	setup();
}
