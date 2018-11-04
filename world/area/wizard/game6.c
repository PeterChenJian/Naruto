#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(６)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 29,
                     "y_axis" : 36,
                  ]),		
		"north" : __DIR__"game3",		
		"west" : __DIR__"game5",		
		"south" : __DIR__"game9",		
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}