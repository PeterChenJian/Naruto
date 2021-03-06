#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("武大师", ({ "weapon master", "master" }));
    set_race("human");
    set_level(50);
    set("age", random(50)+18);

    set("long", "无奸不成商，十足的奸商样的老奸商人。\n");

    // 设定商人所贩卖的物品
    // set_goods(代码, 种类, 编号, 数量)
    // 商人能贩卖的东西只能由 query 指令里查 eq, wp, food 能查出来的物品
    // 如 set_goods(1, "food", 1, 10) 表示贩卖 query 指令查出来的food编号为1的食品十个
    // 若数量设定 -1 表示无限量供应
    // 代码是用于 buy 指令上，如 buy 1 则会向商人买下代码为1的商品

    set_goods(1, "wp", 1, 10);
    set_goods(2, "wp", 2, 10);
    set_goods(3, "wp", 3, 10);
    set_goods(4, "wp", 4, 10);
    set_goods(5, "wp", 5, 10);
    set_goods(6, "wp", 6, 10);
    set_goods(7, "wp", 7, 10);
    set_goods(8, "wp", 8, 10);
    set_goods(9, "wp", 9, 10);
    set_goods(10, "wp", 10, 10);
    set_goods(11, "wp", 11, 10);
    set_goods(12, "wp", 12, 10);

    setup();
}
