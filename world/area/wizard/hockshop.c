#include <room.h>

inherit HOCKSHOP;

void create()
{
	set("short", "天上掉下来的当铺");
	set("long", @LONG
天上人间的当铺，一个相当神秘的地方。
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 40,
                  ]),
    ]));

	setup();
}