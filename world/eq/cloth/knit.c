#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("针织衣", ({ "knit cloth", "cloth" }) );

    set_weight(1200);

    setup_cloth();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 500);
        set("long", "一件纯手工编织的衣服，很耐穿。\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 5,
        ]));
    }
    setup();
}

