
inherit BULLETIN_BOARD;

void create()
{
	set_name("新手教学看板", ({ "newbie board", "board" }) );
	set("location", "/world/area/wizard/newbie");
	set("board_id", "newbie");
	setup();
	set("capacity", 200);
	replace_program(BULLETIN_BOARD);
}
