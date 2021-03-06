/*  passwd.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object ob;

#ifndef SAVE_USER
    return notify_fail("系统目前不储存使用者资料。\n");
#endif

    if( me != this_player(1)
    ||    previous_object() != this_player(1) ) return 0;

    ob = me->link();
    if( !ob ) return 0;
    while( ob && ob->is_character() ) ob = ob->link();

    if( stringp(ob->query("password")) ) {
        write("为了安全起见﹐请先输入您原来的密码﹕");
        input_to("get_old_pass", 1, ob);
    } else {
        write("请输入新密码：");
        input_to("get_new_pass", 1, ob);
    }

    return 1;
}

private void get_old_pass(string pass, object ob)
{
    string old_pass;

    write("\n");
    old_pass = ob->query("password");
    if( crypt(pass, old_pass)!=old_pass ) {
        write("密码错误﹗\n");
        return;
    }
    write("请输入新密码﹕");
    input_to("get_new_pass", 1, ob );
}

private void get_new_pass(string pass, object ob)
{
    write("\n请再输入一次新密码﹕");
#ifdef	ENABLE_MD5_PASSWORD
    input_to("confirm_new_pass", 1, ob, crypt(pass,sprintf("$1$%d", random(99999999))));
#else
    input_to("confirm_new_pass", 1, ob, crypt(pass,0));
#endif
}

private void confirm_new_pass(string pass, object ob, string new_pass)
{
    write("\n");
    if( crypt(pass, new_pass)!=new_pass ) {
        write("对不起﹐您输入的密码并不相同﹐继续使用原来的密码。\n");
        return;
    }
    seteuid(getuid());
    if( !ob->set("password", new_pass) ) {
        write("密码变更失败﹗\n");
        return;
    }

#ifdef SAVE_USER
    ob->save();
    write("密码变更成功\。\n");
    log_file("PASSWD", sprintf("[%s] %s changed password from %s\n",
        ctime(time()), getuid(this_player(1)), query_ip_name(this_player(1)) ));
#endif
}

int help(object me)
{
    write(@HELP
指令格式 : passwd
 
这个指令可以修改你的人物密码。
 
HELP
    );
    return 1;
}
 
