inherit ROOM;

void create()
{
	set("short", "黑魔法阵");
	set("long", @LONG
这个房间四周布满了强大的黑魔法攻击术，似乎是为了斩杀到处
乱窜的臭蟑螂，在你眼前有著堆积如山的蟑螂残骸，可见这黑魔法攻
击术相当有用。
LONG
	);
	set("exits", ([
		"east" : __DIR__"guildhall",
	]));

	setup();
	load_object("/daemon/board/bug");
	replace_program(ROOM);
}
