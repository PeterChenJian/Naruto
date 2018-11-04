#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(４)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
        "west" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 23,
                     "y_axis" : 36,
                  ]),				
		"north" : __DIR__"game1",		
		"east" : __DIR__"game5",		
		"south" : __DIR__"game7",		
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}