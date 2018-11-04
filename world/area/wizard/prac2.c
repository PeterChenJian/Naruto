#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "练习场(２)");
	set("long", @LONG
这里摆放了几个假人，可以在这里练技格斗技能。
LONG);

	set("exits", ([
        "north" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 10,
                     "y_axis" : 33,
                  ]),		
		"west" : __DIR__"prac1",
		"south" : __DIR__"prac5",
		"east" : __DIR__"prac3",
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}