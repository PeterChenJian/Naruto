#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(HIW"珍珠"NOR"项炼", ({ "pearl necklace", "necklace" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "条");
        set("value", 1500);
        set("long", "用一颗颗的珍珠串起来的项炼，相当的高雅。\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "hp": 30,
        ]));
    }

    setup();
}
