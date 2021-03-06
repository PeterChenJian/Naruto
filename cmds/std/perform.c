// cast.c

#include <skill.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object weapon;
    string martial_skill, act, trg;
    object target;

    if( me->is_busy() ) return notify_fail("你现在没有空﹗\n");

    if( !arg ) return notify_fail("指令格式﹕perform <技能.绝招> on <目标>\n");
    
    if( sscanf( arg, "%s.%s on %s", martial_skill, act, trg) != 3 &&
        sscanf( arg, "%s.%s", martial_skill, act) != 2 )
        return notify_fail("指令格式﹕perform <技能.绝招> [on <目标>]\n");

    notify_fail("你请先用 enable 指令选择你要使用的技能。\n");

    if( (martial_skill != me->skill_mapped( martial_skill ))
    &&	(me->skill_mapped(martial_skill) != 0))
        return SKILL_D(me->skill_mapped(martial_skill))->perform_action(me, act, target);

    return 0;

}

int help (object me)
{
    write(@HELP
指令格式﹕perform <技能.绝招> [on <施招对象>]

在战斗中使用所学技能的绝招来和施招对象对战，绝招可以参考
个别的技能所提供的HELP。
HELP);
    return 1;
}
