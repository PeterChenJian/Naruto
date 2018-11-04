#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(YEL"萝卜"NOR, ({ "radish" }));
    set_weight(2000);
    setup_weapon(1, 3, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "根");
        set("value", 404);
        set("long", "一根乾黄的萝卜，摸起来硬邦邦的，应该可以拿来打人。\n" );
    }

    setup();
}
