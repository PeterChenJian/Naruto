#include <room.h>

inherit BANK;

void create()
{
	set("short", "天上掉下来的银行");
	set("long", @LONG
天上人间的银行，一个相当神秘的地方。
    存钱：deposit
    提钱：withdraw
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 36,
                  ]),
    ]));
	setup();
}