#include <ansi.h>
#include <armor.h>

inherit F_LEG_EQ;

void create()
{
    set_name("绑腿", ({ "puttee" }) );

    set_weight(800);
    setup_leg_eq();

    if( !clonep() ) {
        set("unit", "对");
        set("value", 100);
        set("long", "布制的绑腿，提供一般的防护。\n");
        set("wear_as", "leg_eq");
        set("apply_armor/leg_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}

