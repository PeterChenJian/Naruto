#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(BWHT+BLK"写轮"NOR+HIW"眼镜"NOR, ({ "copy glasses", "glasses" }) );

    set_weight(800);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "副");
        set("value", 1500);
        set("long", "一副镜片上头画著写轮眼样子的眼镜，是用来欺敌的战术？\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
        	"armor": 1,
            "str": 2,
        ]));
    }
    setup();
}
