#include <ansi.h>
#include <armor.h>
inherit F_FINGER_EQ;

void create()
{
    set_name(HIK"����ָ"NOR, ({ "silver ring","ring" }) );
    set_weight(100);
    setup_finger_eq();

    if( !clonep() ) {
        set("unit", "ö");
        set("value", 480);
        set("long", "һö����ָ��������ָ��������������á�\n");
        set("wear_as", "finger_eq");
        set("apply_armor/finger_eq", ([
            "combat": 4,
        ]));
    }

    setup();
}