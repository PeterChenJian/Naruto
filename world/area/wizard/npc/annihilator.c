#include <npc.h>

void create()
{
	set_name("阿奈雷特", ({ "annihilator" }));
	set_race("human");
	set("title", "天朝帝国");
	set("age", 30);
	set("long", "一位来自天朝帝国的大神。\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}