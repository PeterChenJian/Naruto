#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "练习场(１)");
	set("long", @LONG
这里摆放了几个假人，可以在这里练技格斗技能。
LONG);

	set("exits", ([
		"east" : __DIR__"prac2",
		"south" : __DIR__"prac4",
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}