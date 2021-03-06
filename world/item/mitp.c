/* testpaper.c 下忍考试卷 write by -Acme- */

inherit ITEM;

mapping test = ([
    1: ([ "msg": "在火影忍者世界中，最大的忍者村是？\n"
                 "           (1) 音隐村\n"
                 "           (2) 雾隐村\n"
                 "           (3) 木叶村\n"
                 "           (4) 砂隐村\n",
          "correct": 3,
          "answer": 0,
       ]),
    2: ([ "msg": "忍者村中通常会设立暗部组织，通常是用来？\n"
                 "           (1) 搞笑用\n"
                 "           (2) 从事间碟、暗杀、追杀叛忍\n"
                 "           (3) 忍术指导组织\n"
                 "           (4) 保护村民\n",
          "correct": 2,
          "answer": 0,
       ]),
    3: ([ "msg": "忍书“临兵斗者皆阵列在前” 的 \"在\" 该如何用双手来使用咒印？\n"
                 "           (1) 五指柔柔分开 双手拇指与食指围成圆形\n"
                 "           (2) 食指 中指 拇指\n"
                 "           (3) 食指 拇指 小拇指\n"
                 "           (4) 右手 食指 左手 握住食指\n",
          "correct": 1,
          "answer": 0,
       ]),
    4: ([ "msg": "什么是查克拉？\n"
                 "           (1) 是一种食物\n"
                 "           (2) 使用忍术、幻术的能量源\n"
                 "           (3) 钱币的单位\n"
                 "           (4) 是一种组织\n",
          "correct": 2,
          "answer": 0,
       ]),
    5: ([ "msg": "使用法术前必须透过什么动作来发动？\n"
                 "           (1) 做暖身操\n"
                 "           (2) 倒立\n"
                 "           (3) 挖鼻屎\n"
                 "           (4) 连续的手印\n",
          "correct": 4,
          "answer": 0,
       ]),
    6: ([ "msg": "木叶村第五代火影是？\n"
                 "           (1) 猿飞\n"
                 "           (2) 自来也\n"
                 "           (3) 钢手\n"
                 "           (4) 鸣人\n",
          "correct": 3,
          "answer": 0,
       ]),
    7: ([ "msg": "下列哪一位是木叶村的叛忍？\n"
                 "           (1) 狮子丸\n"
                 "           (2) 大蛇丸\n"
                 "           (3) 小蛇丸\n"
                 "           (4) 兵粮丸\n",
          "correct": 2,
          "answer": 0,
       ]),
    8: ([ "msg": "什么是体术？\n"
                 "           (1) 利用体内的查克拉在自已或对手身上使用的法术\n"
                 "           (2) 是一种体操，战斗前使用的暖身操\n"
                 "           (3) 以幻觉制造催眠状态\n"
                 "           (4) 利用自已的身体做出攻击\n",
          "correct": 4,
          "answer": 0,
       ]),
    9: ([ "msg": "什么是忍术？\n"
                 "           (1) 利用体内的查克拉在自已或对手身上使用的法术\n"
                 "           (2) 是一种体操，战斗前使用的暖身操\n"
                 "           (3) 以幻觉制造催眠状态\n"
                 "           (4) 利用自已的身体做出攻击\n",
          "correct": 1,
          "answer": 0,
       ]),
   10: ([ "msg": "什么是幻术？\n"
                 "           (1) 利用体内的查克拉在自已或对手身上使用的法术\n"
                 "           (2) 是一种体操，战斗前使用的暖身操\n"
                 "           (3) 以幻觉制造催眠状态\n"
                 "           (4) 利用自已的身体做出攻击\n",
          "correct": 3,
          "answer": 0,
       ]),
]);

string do_look_long()
{
    int index, size;
    string msg;
    
    size = sizeof(test);

    msg  = this_object()->query("name") + ", 请使用 answer 指令作答。\n";
    msg += "────────────────────────────────────\n";

    for(index=1;index<=size;index++) {
        if( undefinedp(test[index]) ) continue;
        msg += sprintf("（%|3s）%2d. %s\n",
               test[index]["answer"] ? test[index]["answer"] + "" : "",
               index,
               test[index]["msg"] );
    }

    msg += "────────────────────────────────────";
    return msg;
}

int do_check()
{
    int index, score = 0 , size;
    size = sizeof(test);

    for(index=1;index<=size;index++) {
        if( undefinedp(test[index]) ) continue;
        if( test[index]["correct"] == test[index]["answer"] ) score += 10;   //答对一题得十分
    }

    return score;
}

void create()
{
    set_name("木叶村下忍考试卷", ({ "muye inferior ninja test paper", "paper"}));
    set_weight(500);
    if( !clonep() ) {
        set("unit", "张");
        set("value", 0);
        set("long", (: do_look_long :) );
    }
    setup();
}

void init()
{
    add_action("do_answer", "answer");
}

int do_answer(string arg)
{
    int no, ans;

    // paper不是在user身上不会有answer动作
    if( !userp(environment(this_object())) ) return 0;

    if( !arg ) return notify_fail("指令格式： answer <题号> <答案>\n");

    if( sscanf(arg, "%d %d", no, ans) != 2 ) return notify_fail("指令格式： answer <题号> <答案>\n");

    if( undefinedp(test[no]) ) return notify_fail("没有这一个题目。\n");
    
    tell_object(this_player(), "你在第 " + no + " 题的答案栏写上 " + ans + " 这个答案。\n");

    test[no]["answer"] = ans;

    this_object()->set("long", do_look_long());

    return 1;
}

string is_test_paper() { return "muye"; }