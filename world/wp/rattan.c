#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(GRN"藤条"NOR, ({ "rattan" }));
    set_weight(2000);
    setup_weapon(4, 8, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "根");
        set("value", 1212);
        set("long", "一根长长的藤条，应该可以拿来打人。\n" );
    }

    setup();
}
