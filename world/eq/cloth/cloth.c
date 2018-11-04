#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("布衣", ({ "cloth" }) );

    set_weight(1500);
    setup_cloth();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 100);
        set("long", "一件寻常的布衣。\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 1,
        ]));
    }
    setup();
}

