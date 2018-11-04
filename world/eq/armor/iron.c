#include <ansi.h>
#include <armor.h>
inherit F_ARMOR;

void create()
{
    set_name(HIK"铁胸甲"NOR, ({ "iron armor", "armor" }) );

    set_weight(4000);
    setup_armor();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 500);
        set("long", "一件熟铁打造的胸甲，提供基础的保护。\n");
        set("wear_as", "armor");
        set("apply_armor/armor", ([
            "armor": 5,
        ]));
    }

    setup();
}

