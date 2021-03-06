#include <ansi.h>
inherit SKILL;

mapping *combat_actions = ({
    ([ "action": "$N用手中的$w往$n$l挥去", ]),
    ([ "action": "$N挥动$w，往$n$l打去", ]),
    ([ "action": "$N将手中$w对准$n$l挥去", ]),
    ([ "action": "$N挥动手中$w，攻向$n$l", ]),
    ([ "action": "$N寻著空隙，一个箭步上前手中$w便往$n$l击落", ]),
});

string *interattack = ({
    "$N伺机而动……\n",
});

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("combat");
    setup();
}

// 使用战斗技巧 (影响命中率)
int combat_using(object me, int ability)
{
    return me->query_skill("combat");
}

void attack_using(object me, object opponent, string skill, object weapon)
{
    int damage,sk;

    if( !me->skill_mapped(skill) ) return 0;

    if( !opponent ) {
        if( me->query_temp("last_attacked_target") )
	    message_vision(CYN + interattack[random(sizeof(interattack))] + NOR, me, weapon);
        return;
    }

    if( !weapon )
        damage = COMBAT_D->fight(me, opponent, skill , me->query("default_actions"), me);
    else
        damage = COMBAT_D->fight(me, opponent, skill , combat_actions[random(sizeof(combat_actions))], weapon);

   if( damage > 0 ) {
     	// 技能经验
        me->improve_skill(skill, 1);
        // 升级经验
        me->gain_score("combat exp", 1);
    }
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);					// 技能等级

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 3000000); break; // 关卡3 : 189升190
    	case 149: me->set_skill_require(skill, 2000000); break; // 关卡2 : 149升150
    	case  99: me->set_skill_require(skill, 1000000); break; // 关卡1 :  99升100
    	default:  me->set_skill_require(skill, lv*lv*15); break;
    }
}