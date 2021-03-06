#include <ansi.h>
#include <armor.h>

inherit F_WRIST_EQ;

void create()
{
    set_name(GRN"玉手环"NOR, ({ "jade wrist", "wrist" }) );
    set_weight(100);
    setup_wrist_eq();

    if( !clonep() ) {
        set("unit", "个");
        set("value", 1600);
        set("long", "一个十分别致的玉手环，上头的纹路很规则。\n");
        set("wear_as", "wrist_eq");
        set("apply_armor/wrist_eq", ([
            "armor": 2,
            "con": 2
        ]));
    }

    setup();
}
