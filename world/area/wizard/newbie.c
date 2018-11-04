inherit ROOM;

void create()
{
	set("short", "新手学堂");
	set("long", @LONG
这里是新手教学学堂的大门口，你可以看到来来往往的新手，每
个人脸上都充满了期待。在新手学堂里，会教导一些游戏中的基本常
识和操作方法，并且指导如何在这个世界中闯荡。
LONG
	);
	set("exits", ([
        "west" : ([ "filename" : __DIR__"wizard.c",
                    "x_axis" : 15,
                    "y_axis" : 5,
                  ]),		
	]));

    set("objects", ([
    	__DIR__"npc/zhang.c" : 1,
    ]));

	setup();

	load_object("/daemon/board/newbie");

	replace_program(ROOM);
}
