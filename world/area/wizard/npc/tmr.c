#include <npc.h>

void create()
{
	set_name("奇", ({ "tmr" }));
	set_race("human");
	set("age", 20);
	set("title", "猎人世界");
	set("long", "一位来自猎人世界的大神。\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}