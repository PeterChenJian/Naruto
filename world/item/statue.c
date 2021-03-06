/* statue.c write by -Acme-
*/

#include <login.h>
#include <ansi.h>

inherit ITEM;

void create()
{
    set_name(HIW"雕像"NOR, ({ "statue"}));
    set_weight(3);
    if( !clonep() ) {
        set("unit", "尊");
        set("value", 0);
        set("no_get", 1);
        set("long", "一尊来路不明的雕像，不知为何会出现在这个地方，你可以发现雕像\n"
                    "表面非常光滑，好像常常有人去触摸(touch)的样子。");
    }
    setup();
}

void init()
{
    add_action("do_touch", "touch");
}

int do_touch(string arg)
{
    object room;

    if( environment(this_object())->is_area() )
        if( !area_environment(this_object(), this_player()) ) return 0;

    if( this_player()->is_busy() || this_player()->is_fighting() ) 
        return notify_fail("你现在没有空。\n");

    if( !arg ) return notify_fail("你想触摸什么东西？\n");

    if( !this_object()->id(arg) ) return notify_fail("你想触摸什么东西？\n");

    if( !objectp(room = load_object(START_ROOM)) ) {
        message_vision("$N伸手往$n抚摸了几次，并发出“咯咯咯”的笑声...\n", this_player(), this_object());
        return 1;
    }

    message_vision("$N伸手往$n抚摸了几次，并发出“咯咯咯”的笑声，接著就消失了。\n", this_player(), this_object());

    this_player()->move(room);
    
    message_vision("$N突然从天上掉了下来...\n", this_player());

    this_player()->start_busy(3);

    return 1;
}