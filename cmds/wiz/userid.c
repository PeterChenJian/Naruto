// tail.c

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
        object *ob;
        int i;
        ob=users();
        i = sizeof(ob);
        while (i--) {
        write(sprintf("%15s : %s\n",ob[i]->query("id"),
        ob[i]->query("userid")));
        }
        return 1;
}           

int help(object me)
{
write(@HELP
ָ���ʽ : usrid <��� id>

��ָ�������(��)�鿴ĳ��һ�������ҵ� userid.
HELP
    );
    return 1;
}