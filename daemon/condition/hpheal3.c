/* hpheal3.c 
   状态 hp 之恢复效能，恢复量是固定值

   资料结构
   ([ "duration": 时效 ])
*/
#include <ansi.h>

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("hpheal3");
}

void condition_update(object me, string cnd, mixed cnd_data)
{
    if( !mapp(cnd_data) || (cnd_data["duration"]--) < 1 ) {
        me->delete_condition(cnd);
        return;
    }

    // 恢复HP值
    me->heal_stat("hp", 30);
}


void food_condition_apply(object me, mapping cnd_data)
{
    mapping hpheal;
    
    if( !mapp(hpheal = me->query_condition("hpheal3")) )
        hpheal = ([]);
    
    hpheal["duration"] += cnd_data["duration"];
    hpheal["name"] = BRED"Ａ级还阳术"NOR;
    
    if( hpheal["duration"] <= 0 ) hpheal["duration"] = 1;
    else if( hpheal["duration"] > 160 ) hpheal["duration"] = 160;
    
    me->set_condition("hpheal3", hpheal);
}

/*
void condition_apply(object me, string cnd, mapping cnd_data) { }
void condition_unapply(object me, string cnd, mapping cnd_data) { }
*/