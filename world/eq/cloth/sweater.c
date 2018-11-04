#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("毛衣", ({ "sweater" }) );

    set_weight(1500);
    setup_cloth();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 200);
        set("long", "一件寻常的毛衣，看起来就很保暖。\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 2,
        ]));
    }
    setup();
}

