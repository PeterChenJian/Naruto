// beast.c

#define BASE_WEIGHT 10000

#include <ansi.h>
#include <statistic.h>
#include <race.h>

inherit HUMANOID;

mapping *combat_actions = ({
	([ "action": "$N扑上来张嘴往$n$l咬去", ]),
	([ "action": "$N往$n$l扑了过去﹐张嘴便咬", ]),
	([ "action": "$N用爪子往$n$l狠狠一抓", ]),
	([ "action": "$N龇牙裂嘴﹐奋不顾身地往$n$l狠狠一咬", ]),
});

mapping query_action()
{
    return combat_actions[random(sizeof(combat_actions))];
}

void create()
{
    ::create();

    seteuid(getuid());

    set("unit", "只");

    set("gender", "male-animal");

    set("limbs", ({ "头部", "身体" }) );

    set("default_actions", (: call_other, __FILE__, "query_action" :) );

    DAEMON_D->register_race_daemon("beast");
}

void setup(object ob)
{
    ::setup(ob);

	ob->set_default_object(__FILE__);
	
	if( !ob->query_weight() ) ob->set_weight(BASE_WEIGHT);

	// Natural armor against physical damage.
	ob->set_temp("apply/armor", 3);
}

void initialize(object ob)
{
    seteuid(geteuid(ob));

    if( !ob->query("age") )
	ob->set("age", 1);
    ob->set_default_object(__FILE__);
}
/*
// 野兽是不会拿武器的
int valid_wield(object me, object ob, string skill) { return 1; }

// 野兽是不会穿防具的
int valid_wear(object me, object ob, string part) {	return 1; }
*/