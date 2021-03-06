/*  identify.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <combat.h>

inherit F_CLEAN_UP;

mapping named_apply = ([
    "str" : "力量",
    "con" : "体格",
    "int" : "智慧",
    "dex" : "敏捷",
    "attack" : "攻击力量",
    "armor" : "防御力量",
    "exact"  : "命中能力",
    "evade"  : "回避能力",
    "intimidate" : "攻击技巧",
    "wittiness" : "防御技巧",
    "move" : "移动能力",
    "carriage" : "负重能力",
    "ap" : "精神值",
    "hp" : "体力值",
    "mp" : "查克拉",
    "head_eq" : "头部",
    "face_eq" : "脸部",
    "neck_eq" : "颈部",
    "wrist_eq" : "手腕",
    "hand_eq" : "手部",
    "finger_eq" : "手指",
    "cloth" : "衣服",
    "armor" : "铠甲",
    "waist_eq" : "腰部",
    "pants" : "裤子",
    "leg_eq" : "腿部",
    "feet_eq" : "足部",
]);

int main(object me, string arg)
{
    object ob;
    string type, sub_type;
    mapping dp, apply;

    if( me->is_busy() ) return 0;

    if( !arg ) return notify_fail("你要鉴定什么东西?\n");
    ob = present(arg, me);
    if( !ob ) return notify_fail("你要鉴定什么东西?\n");
    if( !(type = ob->query("equipped")) )
        return notify_fail("你必须先装备" + ob->name() + "才能鉴定.\n");
    if( sscanf(type, "%s/%s", type, sub_type) != 2 )
        return notify_fail("这类物品无法鉴定.\n");

    write(ob->long() + "\n");

    if( type=="weapon" ) {
        string s;
        mixed sk = ob->query("wield_as");
        
        write("可使用方式：\n");
        if( stringp(sk) ) sk = ({ sk });
        foreach(s in sk)
            printf("%s%s (%s)\n", ((s==sub_type) ? HIY"＊"NOR : "  "), to_chinese(s), s);

        dp = ob->query("damage/" + sub_type);
        printf("\n伤害力： %d - %d (伤害力修正 %d%%，武器等级 %d)\n", 
            dp["lower"], dp["upper"], dp["bonus"], dp["level"] );

        apply = ob->query("apply_" + type + "/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("\n武器特性﹕\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                else
                    prop = to_chinese(prop);
                printf("  %-30s %O\n", prop, value);
            }
        }
    }
    if( type=="armor" ) {
        apply = ob->query("apply_" + type + "/" + sub_type);
        printf("护具部位：%s\n", !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type );
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("护具特性：\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                printf("  %-30s %O\n", to_chinese(prop), value);
            }
        }
    }

    return 1;        
}

int help(object ob)
{
    write(@TEXT
指令格式: identify <物件>

这个指令可以让你鉴定一些武器装备等物品。
TEXT
    );
    return 1;
}
