/*  info.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

int help(object me);

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj;
    string str;
    mixed file_stat;

    if (!arg) return help(me);

    if( arg=="me" ) obj = me;
    else if( arg=="my_link" ) obj = me->link();
    else {
        obj = present(arg, me);
        if (!obj) obj = present(arg, environment(me));
        if (!obj) obj = find_object( resolve_path(me->query("cwd"), arg) );
        if (!obj) return notify_fail("没有这样物件....。\n");
    }

    printf("物件 [%O]\n-----------------------------------------------------\n", obj);
    file_stat = stat(base_name(obj) + ".c");
    write("档案﹕\t\t" + base_name(obj) + ".c\n");
    if( file_stat[2] )
        write("载入时间﹕\t" + CHINESE_D->chinese_period(time()-file_stat[2]) + "\n");
    write("权限﹕\t\tuid = " + getuid(obj) + ", euid = " + geteuid(obj) + "\n");
    write("等级﹕\t\t" + wizhood(obj) + "\n");
    write("使用记忆体﹕\t" + memory_info(obj) + "\n");
    str = "";
    if( living(obj) )		str += "生物 ";
    if( userp(obj) )		str += "使用者 ";
    if( interactive(obj) )	str += "交谈式 ";
    if( wizardp(obj) )		str += "巫师 ";
    if( clonep(obj) )		str += "复制 ";
    if( virtualp(obj) )		str += "虚拟 ";
    if( query_heart_beat(obj) )	str += "心跳:" + query_heart_beat(obj) + " ";
    write("属性﹕\t\t" + str + "\n");
    write("复制个数﹕\t" + sizeof(children(base_name(obj)+".c")) + "\n");
    write("参考连结﹕\t" + refs(obj) + "\n");
    return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : info <物件之名称或档名>

利用此一指令可得知一些有关该物件的资讯。
HELP
    );
    return 1;
}
