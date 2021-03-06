/* parry.c 折招卸力
*/

#include <ansi.h>
inherit SKILL;

string *parry_msg = ({
    "但是$p意图格开",
    "但是$p企图挡开",
});

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("parry");
    setup();
}

// 显示用
int parry_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("parry") ) return 0;
    if( living(me) ) me->set_temp("defend_message", parry_msg[random(sizeof(parry_msg))]);

    return me->query_skill("parry");
}

// 使用parry
int parry_using(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("parry") ) return 0;
    if( living(me) ) me->set_temp("defend_message", parry_msg[random(sizeof(parry_msg))]);

    // 增加经验
    me->improve_skill("parry", 1);

    return me->query_skill("parry");
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