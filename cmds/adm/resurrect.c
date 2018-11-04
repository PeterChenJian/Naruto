// resurrect command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(ROOT_UID);
        if(!str) return notify_fail("指令格式: resurrect <使用者>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("线上的使用者中没有这个人。\n");
        write(file_name(environment(ob))+"\n");
	CHAR_D->make_living(ob);
	ob->setup();

        // ok ..
        write("ok.\n");
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : resurrect <某人>

此指令可让你(你)将某人复活。
HELP
    );
    return 1;
}
