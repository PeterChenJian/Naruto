/* write by -Acme */
#include <ansi.h>

inherit ITEM;

void create()
{
    set_name("任务编辑器", ({ "quest maker", "maker" }));
    set_weight(1);
    if( !clonep() ) {
        set("unit", "根");
        set("value", 0);
        set("no_drop", 1);
        set("long", "相关指令：\n"
        	        HIW"1. sq\n\n"NOR
        	        "查询目前的设定状态\n\n"
                    HIW"2. sq <类别> <内容>\n\n"NOR
                    "设定务任相关资料\n"
                    "类别设定：\n"
                    "name: 设定任务名称\n"
                    "detail: 设定任务细节，若细节较长，可以配合 to 指令，如 to sq detail\n"
                    "trigger: 设定触发语句，找到任务NPC时，对他说这句话便开始任务\n"
                    "end: 任务在第几步骤结束，在撰写任务时，判断该任务是否结束\n\n"
                    HIW"3. create <编号> <忍者村选项>\n\n"NOR
                    "<忍者村选项> 请输入 help maker\n"
                    "<编号> 请配合指令 quest 查询编号，请依序递增\n"
                    "将你先前设定的任务资讯建立起来，以提供任务系统使用。\n\n"
                    HIW"4. help maker\n\n"NOR
                    "任务设计的相关说明" );
    }
    setup();
}

void init()
{
    if( !environment()->is_area() ) {
	    add_action("do_set", "sq");
	    add_action("do_help", "help");
	    add_action("do_create", "create");
	}
}

int show_help()
{
    string msg;

    msg = @LONG
忍者村选项
  -mu 木叶村(muye)             -yi 音隐村(yinyin)
  -wn 雾隐村(wuyin)            -sn 砂隐村(shayin)
  -yn 雨隐村(yuyin)            -yy 云隐村(yunyin)
  -cn 草隐村(caoyin)           -pn 瀑隐村(puyin)
  -or 其它(other)

任务设计举列

任务设定

sq name 木叶村溜狗任务
sq detail 木叶村的吉田小姐，因为有事要外出，需要找人帮忙溜狗。
sq trigger 吉田小姐我帮你溜狗
sq end 3

假设任务的设定如上的木叶村溜狗任务，任务id 为1, 任务是木叶村(mu)的。
也许这个任务是由任务分派所提供(或是某NPC提供)。

1. 当玩家去任务分派所接木叶村溜狗任务时，便将玩家 palyer->add_quest("mu", 1, 1);
   这是设定玩家取得 木叶村(mu) 的任务 ID为1 的任务，且目前的步骤加 1 
   目前任务步骤为 1

2. 找到吉田小姐后用 say 吉田小姐我帮你溜狗 时，先在吉田小姐身上判断player用指令say
   的内容是否与触发语句相同 QUEST_D->check_trigger("mu", 1, "玩家所说的话") 若为真值
   则表示是触发语句，则在吉田小姐身上判断 player->query_quest("mu", 1)
   木叶村(mu) 的 任务 ID为1 的任务是否为 1, 也就是有经过任务步骤1, 若是，则将狗给player溜
   且 player->add_quest("mu", 1, 1); 再将步骤加 1
   目前任务步骤为 2

3. 当你溜完狗后 再 player->add_quest("mu", 1, 1); 再将步骤加 1
   目前任务步骤为 3, (符合任务设计中的 end 设定)

4. 再去找吉田小姐，在吉田小姐身上判断 QUEST_D->check_end(player, "mu", 1) 是否为真值，若为
   真表示任务已经结束，便设计结束后的动作。
LONG;

   this_player()->start_more(msg);
   return 1;
}

int do_help(string arg)
{
	if( arg == "maker" ) return show_help();
	return 0;
}

int do_set_quest(string kind, mixed value)
{
	mapping m;
	if( !mapp(m=this_object()->query("sq")) ) {
	   m = ([]);
	   this_object()->set("sq", m);
	}
    m[kind] = value;
    this_object()->set("sq", m);
    write("Ok.\n");
    return 1;
}

int do_create(string arg)
{
	string kind;
	int iv;
    if( sscanf(arg, "%d -%s", iv, kind) == 2 ) {
    	mapping sq;
        if( !mapp(sq = this_object()->query("sq")) ) return notify_fail("目前没有任何的设定。\n");
        QUEST_D->set_quest(kind, iv, sq);
        this_object()->delete("sq");
        write("Ok.\n");
        return 1;
    }
    return notify_fail("指令格式错误。\n");
    return 1;
}

int do_set(string arg)
{
	int iv;
	string kind, sv;

    if( !arg ) {
    	mapping sq;
        if( !mapp(sq = this_object()->query("sq")) ) return notify_fail("目前没有任何的设定。\n");
        write("name:" + sq["name"] + "\n");
        write("detail:" + sq["detail"] + "\n");
        write("trigger:" + sq["trigger"] + "\n");
        write("end:" + sq["end"] + "\n");
        return 1;
    }

    if( sscanf(arg, "%s %d", kind, iv) == 2 ) return do_set_quest(kind, iv);
    if( sscanf(arg, "%s %s", kind, sv) == 2 ) return do_set_quest(kind, sv);

    return notify_fail("指令格式不正确。\n");

	return 1;
}