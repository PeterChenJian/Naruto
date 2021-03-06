/* lotto.c
   乐透 三十八号选六号 摸彩程式
   write by -Acme-
*/

#include <localtime.h>
#include <npc.h>

inherit F_VILLAGER;

// 每张乐透单价(price), 卖出总量(amount), 是否正在开奖(lottery)
// 得奖号码(award), 暂存用变数(temp)
int price = 50, amount = 0, lottery = 0, *award, *temp;

// 总奖金是当期总投注金额乘以总奖金率得之, 总奖金率(total_ratio)
// 总奖金减掉普奖总额后，依下列比率分配奖金
// 头奖(first_ratio), 二奖(second_ratio), 三奖(third_ratio), 四奖(fourth_ratio)
int total_ratio = 58, first_ratio = 38, second_ratio = 12, third_ratio = 15, fourth_ratio = 35;

// 号码阵列(numbers)
int *numbers = ({  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                  31, 32, 33, 34, 35, 36, 37, 38 });

// 购买记录(record), 得奖记录(prize)
mapping record = ([]);
mapping prize = ([]);

void do_lottery();

void create()
{
    set_name("乐透美眉", ({ "lotto girl", "girl" }));
    set_race("human");
    set_level(1);

    set("age", 18);
    set("title", "爽歪歪");
    set("gender", "female");
    set("nickname", "包中");
    set("long", "一位穿著火辣的美眉令人垂涎欲滴。\n");

    set("chat_chance", 5);
    set("chat_msg", ({
        (: do_lottery :),
    }));

    setup();
}

void init()
{
    add_action("do_bet", "bet");
}

// 乱数取6个号码
int *do_random(int count)
{
    int t, *nums=({});
    temp = numbers;
    while(count--) {
        t = temp[random(sizeof(temp))];
        nums += ({ t });
        temp -= ({ t });
    }
    return nums;
}

// 捡查号码是否有重复
int check_number(int *nums)
{
    int i = sizeof(nums);
    temp = numbers;
    while(i--) {
        if( member_array(nums[i], temp) == -1 ) return 0;
        else temp -= ({ nums[i] });
    }
    return 1;
}

// 加入一笔号码
void add_bet(string id, int *nums)
{
    if( undefinedp(record[id]) ) record[id] = ({});
    record[id] += ({ nums });
    amount++;
}

int do_bet(string arg)
{
    int t, *nums = ({0,0,0,0,0,0});

    // 在区域中，同一格座标才会有作用
    if( environment(this_object())->is_area() )
        if( !area_environment(this_object(), this_player()) ) return 0;

    if( !arg ) return notify_fail("请输入 help lotto 了解如何玩乐透游戏。\n");

    if( arg == "random") {
        if( this_player()->query("bank") < price )
            return notify_fail("你银行不够钱，无法买彩券。\n");
        this_player()->add("bank", -price);
        nums = do_random(6);
        add_bet(this_player()->query_id(), nums);

    } else if( sscanf(arg, "random %d", t) == 1 ) {
        string id;
        if( t <= 0 || t >= 100 ) return notify_fail("请输入 help lotto 了解如何玩乐透游戏。\n");
        if( this_player()->query("bank") < price*t )
            return notify_fail("你银行不够钱，无法买彩券。\n");
        this_player()->add("bank", -(price*t));
        id = this_player()->query_id();
        while( t-- ) {
            nums = do_random(6);
            add_bet(id, nums);
        }

    } else if( sscanf(arg, "%d %d %d %d %d %d", nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]) == 6 ) {
        if( !check_number(nums) ) return notify_fail("请输入 help lotto 了解如何玩乐透游戏。\n"); 
        if( this_player()->query("bank") < price )
            return notify_fail("你银行不够钱，无法买彩券。\n");
        this_player()->add("bank", -price);
        add_bet(this_player()->query_id(), nums);

    } else return notify_fail("请输入 help lotto 了解如何玩乐透游戏。\n"); 

    return 1;
}

int check_hit(int *nums)
{
    int count, size;
    size = sizeof(nums);
    while( size-- ) {
        if( member_array(nums[size], award) != -1 )
            count++;
    }
    return count;
}

// 开奖
void do_lottery()
{
    int sp_number;

    if( !lottery ) return;
    
    // 乱数取七个不重复号码，一个为特别号
    award = do_random(7);

    // 特别号需要另外处理
    sp_number = award[6];
    award -= ({ award[6] });
}