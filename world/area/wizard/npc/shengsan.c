#include <npc.h>

void create()
{
	set_name("蓝蓝的天", ({ "shengsan" }));
	set_race("human");
	set("age", 30);
	set("title", "梦幻七域");
	set("long", "一位来自梦幻七域的大神。\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}