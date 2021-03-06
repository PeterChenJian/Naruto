/* advance */

#include <ansi.h>
#include <attribute.h>

inherit F_CLEAN_UP;

int add_attribute(object me, string attr, int amount)
{
    if( me->query_point("attribute") < amount ) {
        write("你的属性点数不足，无法提升人物属性。\n");
        return 1;
    }
    
    if( me->query_attr(attr, 1) + amount > ATTRVAL_MAX ) {
        write("你的属性 " + attr + " 无法提升这么多，上限为"+ chinese_number(ATTRVAL_MAX) +"。\n");
        return 1;
    }
    
    me->add_point("attribute", -amount);
    me->set_attr(attr, me->query_attr(attr, 1) + amount);
    
    write(HIY"你的属性 " + attr + " 提升" + chinese_number(amount) + "点了！\n"NOR);
    
    return 1;
}

int add_state(object me, string state, int amount)
{
    if( me->query_point("score") < amount ) {
        write("你的潜能点数不足，无法提升人物状态。\n");
        return 1;
    }
    
    if( me->query_stat_maximum(state) + amount > 9999 ) {
        write("你的状态 " + state + " 无法提升这么多，上限为九千九百九十九。\n");
        return 1;
    }
    
    me->add_point("score", -amount);
    me->advance_stat(state, amount);
    me->heal_stat(state, amount);

    write(HIY"你的状态 " + state + " 提升"+ chinese_number(amount) +"点了！\n"NOR);

    return 1;
}

int add_skill(object me, string skill, int amount)
{
    int i;
    if( me->query_point("skill") < amount ) {
        write("你的技能点数不足，无法提升人物技能上限。\n");
        return 1;
    }

    if( me->query_skill_ability(skill) + amount > 10 ) {
        write("你的技能 " + skill + " 无法提升这么多，技能投点最高为十点。\n");
        return 1;
    }
    
    me->add_point("skill", -amount);

    for(i=1;i<=amount;i++) me->add_skill_ability(skill);
    
    write(HIY"你的技能 " + skill + " 上限提升" + chinese_number(amount*20) + "级了！\n"NOR);
    
    return 1;
}

int main(object me, string arg)
{
    int amount;

    if( !arg ) return notify_fail("你想提升人物的什么能力？\n");
    
    if( sscanf(arg, "%s for %d", arg, amount) != 2 ) amount = 1;

    if( amount < 0 ) return notify_fail("你想降低人物的能力？\n");

    // 提升人物属性
    if( arg == "str" || arg == "con" || arg == "dex" || arg == "int" ) {
        add_attribute(me, arg, amount);
        return 1;
    }
    
    // 提升人物状态
    if( arg == "ap" || arg == "hp" || arg == "mp" ) {
        add_state(me, arg, amount);
        return 1;
    }
    
    // 提升人物技能
    if( me->query_skill(arg) ) {
        add_skill(me, arg, amount);
        return 1;
    }
    
    write("没有 " + arg + " 这种属性、状态或技能，你无法提升任何能力。\n");

    return 1;
}

int help()
{
    write(@TEXT
指令格式： advance <属性、状态或技能> [for <点数>]

这个指令可以提升人物的能力。

例：
advance str          提升人物的力量属性1点，使用后扣除一点属性点数
advance con for 3    提升人物的体格属性3点，使用后扣除三点属性点数
advance dex          提升人物的敏捷属性1点，使用后扣除一点属性点数
advance int for 2    提升人物的智慧属性2点，使用后扣除二点属性点数

advance ap           提升人物精神值状态1点，使用后扣除一点潜能点数
advance hp for 50    提升人物体力值状态50点，使用后扣除五十点潜能点数
advance mp           提升人物查克拉状态1点，使用后扣除一点潜能点数

advance combat      技能combat投一点，技能上限提升20级
advance dodge for 3 技能dodge投三点，技能上限提升60级
TEXT);
    return 1;
}