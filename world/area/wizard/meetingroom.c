#include <ansi.h>
#include <command.h>

inherit ROOM;

string filename;

void create()
{
	set("short", "异言堂");
	set("long", @LONG
这里是众神们讨论重大事情的场所。圆形的房间，白色的墙，铺
著红色的地毯，房间的顶端镶崁著一颗神秘的晶石，光线就从晶石散
发出来，照亮了整个房间，明亮却不刺眼。靠门的墙边浮著一块紫色
的玉板，板子上刻了些字。
LONG
	);

	set("exits", ([
		"down" : __DIR__"guildhall"
	]));

	set("detail", ([
		"玉板" : "一块飘浮在半空中的紫翠玉板, 上面刻了些字:

会议记录的方法:
start <filename> :	开始记录会议过程, filename 是会议记录的档名。
over :			停止记录会议过程。

",
	]));
    set("no_fight", 1);
	setup();
	load_object("/daemon/board/wizard_talk");
}

void init()
{
	add_action("say", ({"say", "'"}) );
	add_action("start", "start");
	add_action("over", "over");
}

int start(string str)
{
	if( !wizardp(this_player()) )
	return notify_fail("你没有权力开会议。\n");

	if( !str )
	return notify_fail("你要将会议记录写在那个档??\n");

	if ( (int)query("meeting_trigger") != 0 )
	return notify_fail("目前正在开会中。\n");

	set("meeting_trigger", 1);
	message_vision(HIW"\n$N宣布现在会议正式开始。\n\n"NOR, this_player());
	filename = str;
	log_file(filename, sprintf("[%s] %s (%s) 开启会议。\n",
		ctime(time()), this_player()->name(1), geteuid(this_player())));
	return 1;
}

int say(string msg)
{
	if( !msg ) return 0;
	else
	SAY_CMD->main(this_player(), msg);
	if ((int)query("meeting_trigger") == 1)
	log_file(filename,
	sprintf("%s说道﹕" + msg + "\n",this_player()->name(1)) );
	return 1;
}

int over()
{
	if( !wizardp(this_player()) )
        return notify_fail("你没有权力结束会议。\n");

	delete("meeting_trigger");
	log_file(filename, sprintf("[%s] %s (%s) 结束会议。\n",
		ctime(time()), this_player()->name(1), geteuid(this_player())));
	message_vision(HIW"\n$N宣布会议结束。\n\n"NOR, this_player());
	return 1;
}
