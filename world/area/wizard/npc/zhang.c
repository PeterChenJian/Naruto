// write by -Acme-

#include <npc.h>

inherit F_FIGHTER;
inherit F_TRAINER;    // 训练忍者

/* guild: muye   木叶忍者村
          yinyin 音隐忍者村
          wuyin  雾隐忍者村
          shayin 砂隐忍者村
          yuyin  雨隐忍者村
          yunyin 云隐忍者村
          caoyin 草隐忍者村
          puyin  瀑隐忍者村
*/

void create()
{
    set_name("辛心长", ({ "sin sin zhang master", "master" }) );
    set_race("human");
    set_level(40);
    set_class("superior");    //上忍
    set("age", 35);
    set("title", "新手导师");
    set("long", "他是一位新手学院里专聘的教师，教导新手一些基本技能，你可以利\n"
                "用 train 指令来学习一些基本的技能。\n"
                "用 list 指令查询可以学习的技能列表。\n");

    // 设定忍者村，相同的忍者村才会教, 设定 all 不分忍者村
    set("guild", "all");

    // 设定能训练的技能
    //
    // set_train(技能名, 是否可直接习得, 学习该技能所需要的角色等级, 此技能可学习到的上限等级)
    //
    // 当玩家没有该技能时，若"是否可直接习得"这个设定值为非零值时，玩家向训练者学习时则可记住
    // 这个技能到lv1，而有lv的技能才可以投点继续锻练。故可用于特殊技，要解谜得到lv后方可训练。
    //
    set_train("combat", 1, 1, 100);               // 格斗技巧             F级
    set_train("dodge", 1, 1, 100);                // 闪躲技巧             F级
    set_train("parry", 1, 1, 100);                // 挡格技巧             F级
    set_train("heavy hit", 1, 5, 100);            // 重击之术             F级
    set_train("savage hit", 1, 5, 100);           // 暴击之术             F级
    set_train("continual hit", 1, 5, 100);        // 连击之术             F级
    set_train("force", 1, 10, 100);               // 体术                 F级
    set_train("ninja", 1, 10, 100);               // 忍术                 F级
    set_train("magic", 1, 10, 100);               // 幻术                 F级
/*
    set_train("alter ego", 0, 20, 100);           // 忍术：分身术         
    set_train("substitute", 0, 15, 100);          // 忍术：替身术         
    set_train("shadow", 0, 50, 100);              // 忍术：影分身术       
    set_train("thunder cut", 0, 60, 100);         // 忍术：雷切           S级
    set_train("reincarnation", 0, 80, 100);       // 忍术：秽土转生       S级
    set_train("moon dance", 0, 55, 100);          // 忍术：弯月舞         A级
    set_train("wind blade", 0, 60, 100);          // 忍术：风刃           A级
    set_train("snake kill", 0, 65, 100);          // 忍术：双蛇相杀术     A级
    set_train("water-water fall", 0, 50, 100);    // 忍术：水遁·大瀑布术 A级
    set_train("water-water dragon", 0, 40, 100);  // 忍术：水遁·水龙弹术 B级
    set_train("plover", 0, 55, 100);              // 忍术：千鸟           A级
    set_train("soul dies", 0, 45, 100);           // 忍术：死魂术         B级
    set_train("puppet", 0, 35, 100);              // 忍术：傀儡术         C级
    set_train("whirlwind", 0, 30, 100);           // 忍术：旋风术         C级
    set_train("fire-dragon fire", 0, 30, 100);    // 忍术：火遁·龙火术   C级
    set_train("fire-bold fireball", 0, 25, 100);  // 忍术：火遁·豪火球术 C级
    set_train("fire-phoenix fire", 0, 20, 100);   // 忍术：火遁·凤仙火术 C级
    set_train("animal taming", 0, 10, 100);       // 忍术：驯兽术         E级
    set_train("bad worm", 0, 40, 100);            // 忍术：寄坏虫术       -级
    set_train("sand burial", 0, 40, 100);         // 忍术：砂瀑送葬       -级
    set_train("sand tie", 0, 30, 100);            // 忍术：砂缚柩         -级
    set_train("sand armor", 0, 50, 100);          // 忍术：砂盔甲         -级
    set_train("sand shield", 0, 60, 100);         // 忍术：砂盾           -级

    set_train("inside lotus", 0, 60, 100);        // 体术：里莲华         A级
    set_train("outside lotus", 0, 50, 100);       // 体术：表莲华         B级
    set_train("shadow leaf", 0, 30, 100);         // 体术：影舞叶         C级
    set_train("lion attack", 0, 30, 100);         // 体术：狮子连弹       C级
    set_train("muscle tank", 0, 25, 100);         // 体术：肉弹战车       C级
    set_train("thousand year kill", 0, 20, 100);  // 体术：千年杀         E级
    set_train("double physique", 0, 30, 100);     // 体术：倍化术         -
    set_train("sixty palms", 0, 50, 100);         // 体术：八卦六十四掌   -
    set_train("sky palm", 0, 40, 100);            // 体术：八卦掌回天     -

    set_train("five elements seal", 0, 70, 100);  // 封印：五行封印术     A级
    set_train("five elements unseal", 0, 70, 100);// 封印：五行解印术     A级
*/
    setup();

    // 人物本身的技能设定
    set_skill("dodge", 200);               // 基本： 回避技巧
    set_skill("parry", 200);               // 基本： 防御技巧
    set_skill("combat", 200);              // 基本： 格斗技巧
    set_skill("savage hit", 200);          // 基本： 暴击
    set_skill("heavy hit", 200);           // 基本： 重击
    set_skill("continual hit", 200);       // 基本： 连击
    set_skill("force", 180);               // 基本： 体术
    set_skill("ninja", 200);               // 基本： 忍术
    set_skill("magic", 160);               // 基本： 幻术
    set_skill("substitute", 180);          // 忍术： 替身术

    map_skill("dodge", "substitute");      // 使用技能“替身术”于回避技巧上
}