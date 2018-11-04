#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIR"红色头巾"NOR, ({ "red hood", "hood" }) );

    set_weight(500);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "条");
        set("value", 450);
        set("long", "一条相当有个人风格的头巾。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "exact": 3,
        ]));
    }

    setup();
}
