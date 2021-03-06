/* write by -Acme- */

#include <room.h>

// 设成INN者，会自动成为非战区，并且是重生点
inherit INN;

void create()
{
    set("short", "巫师神殿");
    set("long", @LONG
这里住著一群拥有高级魔法的巫师，巫师所拥有的能力不是一般
人可以瞭解的，你可以看见前面的巫师手舞足蹈地，周身不时散发出
七彩光芒，不知在使什么魔法。
LONG
    );
    set("exits", ([
        "west" : __DIR__"bug",
        "north" : __DIR__"academy",
        "east" : __DIR__"propose",
        "up" : __DIR__"meetingroom",
        "south": __DIR__"power",
    ]));

    set("objects", ([
        __DIR__"npc/annihilator" : 1,
        __DIR__"npc/luky" : 1,
        __DIR__"npc/shengsan" : 1,
        __DIR__"npc/tmr" : 1, 
        __DIR__"npc/acme" : 1, 
    ]));

    setup();
    load_object("/daemon/board/wizard");
    replace_program(ROOM);
}