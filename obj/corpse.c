/*  corpse.c - the coprse object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ITEM;

static int decayed = 0;
static int decay_interval = 120;

void create()
{
    set_name("无名尸体", ({ "corpse" }) );
    set("long", "这是一具无名尸体。\n");
    set("unit", "具" );
    setup();
}

void
start_decay(int interval)
{
    if( interval < 1 ) interval = 0;

    decay_interval = interval;

    remove_call_out("decay");
    decayed = 0;
    call_out("decay", decay_interval * 2);
}

int is_corpse() { return decayed < 2; }
int is_character() { return decayed < 1; }

string short() { return name() + "(" + capitalize(query("id")) + ")"; }

void decay()
{
    switch(++decayed) {
        case 1:
            say( query("name") + "开始腐烂了﹐发出一股难闻的恶臭。\n" );
            switch(query("gender")) {
                case "male":
                    set_name("腐烂的男尸", ({ "corpse" }) );
                    break;
                case "female":
                    set_name("腐烂的女尸", ({ "corpse" }) );
                    break;
                default:
                    set_name("腐烂的尸体", ({ "corpse" }) );
                    break;
            }
            set("long", "这具尸体显然已经躺在这里有一段时间了﹐正散发著一股腐尸的味道。\n");
            call_out("decay", decay_interval);
            break;
        case 2:
            say( query("name") + "被风吹乾了﹐变成一具骸骨。\n" );
            set_name("枯\乾的骸骨", ({ "skeleton" }) );
            set("long", "这副骸骨已经躺在这里很久了。\n");
            call_out("decay", decay_interval);
            break;
        case 3:
            say( "一阵风吹过﹐把" + query("name") + "化成骨灰吹散了。\n" );
            destruct(this_object());
            break;
    }
}

int receive_object(object oob, int from_inventory) { return 1; }
