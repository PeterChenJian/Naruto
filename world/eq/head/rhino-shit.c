#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"犀牛"NOR+YEL"大便帽"NOR, ({ "rhino shit hat", "hat" }) );

    set_weight(3300);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "顶");
        set("value", 3700);
        set("long", "一顶犀牛造型加上大便造型的帽子，让你搞笑又活力充沛。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 6,
            "int": 1,
        ]));
    }

    setup();
}
