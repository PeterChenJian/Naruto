/* shadow.c 忍术：影分身术 */
    
#include <ansi.h>

#define SKILL_NAME "shadow"
#define BASE_SKILL "alter ego"
#define SHADOW_NUM 5

inherit SKILL;

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}


// 应用于 忍术(ninja)
int valid_enable(string base_skill) { return base_skill == "ninja"; }

// 忍术
string query_class() { return "ninja"; }

// 创造影分身函式
void construct_shadow(object me)
{
    int i, count;
    object shadow;
    mapping map;
    
    if( me->query_skill(SKILL_NAME, 1) < 40 || me->query_skill(BASE_SKILL, 1) < 40) {
        write("你的忍术影分身术或分身术技能等级不足，每多四十级可多增加一只影分身。\n");
        return;
    }

    // 拥有者消秏一些查克拉
    if( me->query_stat("mp") < 50 ) {
        write("你目前没有足够的查克拉来使用影分身术。\n");
        return;
    }
    

    map = me->query_temp("shadow");
    count = sizeof(map);
    
    // 寻找可用编号 count
    if( count ) {
        count = 0;
        for(i=0;i<SHADOW_NUM;i++)
            if( objectp(map["shadow"+i]) ) count++;
    }

    if( count >= SHADOW_NUM || count >= me->query_skill(SKILL_NAME, 1)/40 || 
                               count >= me->query_skill(BASE_SKILL, 1)/40 ) {
        write("你的影分身数量已经够多了，无法再创造多余的影分身。\n");
        return;
    }
    
    shadow = new(__DIR__"obj/shadow.c");
    if( !objectp(shadow) ) {
        write("你似乎无法创造影分身...\n");
        destruct(shadow);
        return;
    }

    message_vision("$N双手快速的结印，施展了"HIY"影分身术"NOR"！！\n", me);

    me->damage_stat("mp", 50, me);
    me->start_busy(3);

    if( environment(me)->is_area() ) {
        if( !area_move(environment(me), shadow, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) {
            write("你的影分身出来后，又消失不见了...\n");
            destruct(shadow);
            return;
        }
    } else {
        if( !shadow->move(environment(me)) ) {
            write("你的影分身出来后，又消失不见了...\n");
            destruct(shadow);
            return;
        }
    }

    // 设定名称, ID
    shadow->set_name(me->query("name"), ({ me->query_id() + "'s shadow",  "shadow" }));
    shadow->set("nickname", me->query("nickname"));
    shadow->set("title", me->query("title"));

    // 相同等级
    shadow->set("level", me->query_level());

    // 相同属性
    shadow->set_attr("str", me->query_attr("str"));
    shadow->set_attr("con", me->query_attr("con"));
    shadow->set_attr("int", me->query_attr("int"));
    shadow->set_attr("dex", me->query_attr("dex"));

    // 相同状态
    shadow->set_stat_current("ap", me->query_stat_maximum("ap"));
    shadow->set_stat_maximum("ap", me->query_stat_maximum("ap"));
    shadow->set_stat_current("hp", me->query_stat_maximum("hp"));
    shadow->set_stat_maximum("hp", me->query_stat_maximum("hp"));
    shadow->set_stat_current("mp", me->query_stat_maximum("mp"));
    shadow->set_stat_maximum("mp", me->query_stat_maximum("mp"));

    shadow->guard_ob(me);       //保护leader
    shadow->set_leader(me);     //跟随leader
    shadow->set_owner(me);      //设定owner
    shadow->set_attack(1);      //可以攻击
    shadow->set_evade(1);       //可以回避
    shadow->set("skill", "shadow");  // shadow是由alter ego技能叫出

    me->set_temp("shadow/shadow"+count, shadow);

    
    // 增加技能经验
    me->improve_skill("shadow", 1);

    return;
}

// 删除影分身函式
void destruct_shadow(object me)
{
    object shadow, *obs;
    
    if( !me->query_temp("shadow") ) {
        write("你目前没有任何的影分身存在。\n");
        return;
    }
    
    if( sizeof(obs = values(me->query_temp("shadow"))) <= 0 ) {
        write("你目前没有任何的影分身存在。\n");
        return;
    }

    foreach(shadow in obs) if( objectp(shadow) ) shadow->do_destruct();
    
    me->delete_temp("shadow");
    
    write("你的所有影分身都消失了。\n");
    
    return;
}

// 使用绝招
int perform_action(object me, string act, object target)
{
    if( !objectp(me) ) return 0;
    if( me->is_busy() ) return notify_fail("你现在没有空使用影分身术。\n");

    switch( act ) {
        case   "shadow": construct_shadow(me); break;
        case "destruct": destruct_shadow(me);  break;
        default: return notify_fail("目前影分身有 shadow, destruct 两种功\能。\n"); break;
    }
    
    return 1;
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);    // 技能等级

    switch( lv ) {
        case 189: me->set_skill_require(skill, 1500000);  break; // 关卡3 : 189升190 需要一百五十万经验
        case 149: me->set_skill_require(skill, 1000000);  break; // 关卡2 : 149升150 需要一百万经验
        case  99: me->set_skill_require(skill, 500000);   break; // 关卡1 :  99升100 需要五十万经验
        default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}