#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(９)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
		"north" : __DIR__"game6",
		"west" : __DIR__"game8",
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}