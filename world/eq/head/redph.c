#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIR"红色"HIW"羽毛头巾"NOR, ({ "red plume hood", "hood" }) );

    set_weight(1000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "条");
        set("value", 650);
        set("long", "一条相当有个人风格的头巾，上头还插著几根红色羽毛。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
        	"armor": 2,
            "exact": 3,
        ]));
    }

    setup();
}
