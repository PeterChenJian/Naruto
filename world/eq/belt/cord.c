#include <ansi.h>
#include <armor.h>

inherit F_WAIST_EQ;

void create()
{
    set_name("绳子", ({ "cord" }) );
    set_weight(800);
    setup_waist_eq();

    if( !clonep() ) {
        set("unit", "条");
        set("value", 100);
        set("long", "一条很普通的绳子。\n");
        set("wear_as", "waist_eq");
        set("apply_armor/waist_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}
