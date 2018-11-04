#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "神之力");
	set("long", @LONG
这个房间存在著一些不可思议的力量，传说中在这里施展一些特
殊的禁忌巫术，可以改变这个世界的所有事物，实在是太不可思议了
。在这里充满紧张的气氛，你不由得想赶紧离开。
LONG
	);

	set("exits", ([
		"north" : __DIR__"guildhall",
	]));

	setup();

    load_object("/daemon/board/chat");
}