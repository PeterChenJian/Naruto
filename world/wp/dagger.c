#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIK"苦无"NOR, ({ "dagger" }));
    set_weight(2500);
    setup_weapon(12, 12, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "把");
        set("value", 2424);
        set("long", "一把忍者专用的寻常忍具。\n" );
    }

    setup();
}
