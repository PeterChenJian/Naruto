#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(MAG"双截棍"NOR, ({ "pair rod", "rod" }));
    set_weight(6000);
    setup_weapon(24, 32, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "把");
        set("value", 5656);
        set("long", "两根棍子之间用铁炼接在一起的双截棍。\n" );
    }

    setup();
}
