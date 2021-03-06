/* animal_taming.c 忍术：驯兽之术 */
    
#include <ansi.h>

#define SKILL_NAME "animal taming"

inherit SKILL;

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}


// 应用于 忍术(ninja)
int valid_enable(string base_skill) { return base_skill == "ninja"; }

// 忍术
string query_class() { return "ninja"; }

// 初始化宠物
void setup_dog(object me, object pet)
{
    int lv;
   
    // 设定名字
    if( me->query("pet/name") ) pet->set("name", me->query("pet/name"));
    
    // 设定ID
    pet->set("id", me->query_id() + "'s dog");
    
    // 设定title
    if( me->query("pet/title") ) pet->set("title", me->query("pet/title"));
    
    // 设定nickname
    if( me->query("pet/nickname") ) pet->set("nickname", me->query("pet/nickname"));

    // 设定long
    if( me->query("pet/long") ) pet->set("long", me->query("pet/long"));
    
    // 设定food
    if( me->query("pet/food") ) pet->food_gain(me->query("pet/food"));
    
    // 设定Level
    if( me->query("pet/level") ) pet->set_level(me->query("pet/level"));
    
    // 设定exp
    if( me->query("pet/exp") )
        pet->exp_gain(me->query("pet/exp"));

    lv = pet->query_level();
    
    // 依等级提升能力
    pet->set_attr("str", lv*2);
    pet->set_attr("con", lv*2);
    pet->set_attr("dex", lv*2);
    pet->set_attr("int", lv*2);
    pet->set_stat_maximum("ap", lv*50);
    pet->set_stat_maximum("hp", lv*50);
    pet->set_stat_maximum("mp", lv*50);
    pet->set_stat_current("ap", lv*50);
    pet->set_stat_current("hp", lv*50);
    pet->set_stat_current("mp", lv*50);
    pet->set_skill("combat", lv*3);
    pet->set_skill("parry", lv*3);
    pet->set_skill("dodge", lv*3);
    pet->set_skill("heavy hit", lv*3);
    pet->set_skill("savage hit", lv*3);
    pet->set_skill("continual hit", lv*3);
    
    pet->do_heal();

    // 设定宠物目前状态
    if( me->query("pet/ap") ) pet->set_stat_current("ap", me->query("pet/ap"));
    if( me->query("pet/hp") ) pet->set_stat_current("hp", me->query("pet/hp"));
    if( me->query("pet/mp") ) pet->set_stat_current("mp", me->query("pet/mp"));

}

// 叫出忍犬函式
void call_dog(object me)
{
    object pet;

    if( objectp( pet = me->query_temp("pet") ) ) {
        write("你的忍犬已经被你叫出来了...\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 40 ) {
        write("你的驯兽术必需大于40级才能使用这个指令。\n");
        return;
    }

    // 拥有者消秏一些查克拉
    if( me->query_stat("mp") < 20 ) {
        write("你目前没有足够的查克拉来使用分身术。\n");
        return;
    }

    pet = new(__DIR__"obj/pet.c");
    if( !objectp(pet) ) {
        write("你似乎没办法呼叫你的忍犬出来...\n");
        destruct(pet);
        return;
    }

    message_vision(HIW"$N从裤裆里掏出一支哨子吹了几下，脸上露出满足的笑容...\n"NOR, me);

    // 初始化宠物状态
    setup_dog(me, pet);

    if( environment(me)->is_area() ) {
        if( !area_move(environment(me), pet, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) {
            write("你的忍犬出来后，又一溜烟地跑走了...\n");
            destruct(pet);
            return;
        }
    } else {
        if( !pet->move(environment(me)) ) {
            write("你的忍犬出来后，又一溜烟地跑走了...\n");
            destruct(pet);
            return;
        }
    }

    if( me->query_skill("animal taming", 1) > 50 ) pet->set_leader(me);     //跟随leader
    if( me->query_skill("animal taming", 1) > 60 ) pet->guard_ob(me);       //保护leader
    pet->set_owner(me);      //设定owner
    pet->set_attack(1);      //可以攻击
    pet->set_evade(1);       //可以回避

    me->set_temp("pet", pet);
    
    message_vision("$n一见到$N高兴得跳来跳去“汪！汪！”叫...\n", me, pet);

    me->damage_stat("mp", 20, me);
    me->start_busy(3);

    // 增加技能经验
    me->improve_skill("animal taming", 1);

    return;
}

// 删除宠物函式
void destruct_dog(object me)
{
    mapping petr=([]);
    object pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("你目前没有叫出你的忍犬出来...\n");
        return;
    }
    
    if( pet->is_fighting() || pet->is_busy() ) {
        write("你的忍犬似乎在忙些什么，一时脱不了身...\n");
        return;
    }
    
    // 宠物状态储存
    petr["name"] = pet->query("name");
    petr["title"] = pet->query("title");
    petr["nickname"] = pet->query("nickname");
    petr["long"] = pet->query("long");
    petr["exp"] = pet->query_exp();
    petr["food"] = pet->query_food();
    petr["level"] = pet->query_level();
    petr["ap"] = pet->query_stat("ap");
    petr["hp"] = pet->query_stat("hp");
    petr["mp"] = pet->query_stat("mp");    
    
    me->set("pet", petr);
    
    pet->do_destruct();
    me->delete_temp("pet");
    
    write("你的忍犬离开了。\n");
    
    return;
}

string status_color(int current, int max)
{
    int ratio = max ? (current * 100) / max : 100;

    switch( ratio ) {
        case  0..25: return HIR;
        case 26..50: return HIM;
        case 51..75: return HIY;
        case 76..100: return HIG;
        default: return HIC;
    }
}

void show_state(object me)
{
    object pet;
    string msg;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("请先叫出你的忍犬。\n");
        return;
    }

    if( me->query_skill("animal taming", 1) <= 20 ) {
        write("你的驯兽术必需大于20级才能使用这个指令。\n");
        return;
    }

    printf("[%s] 等级 %d  经验值 %2.2f%s  食物量 %s%d/%d\n"NOR,
        pet->query("name"),
        pet->query("level"),
        (pet->query_exp()*10.0 / (pet->query("level") * pet->query("level"))),
        "%",
        status_color(pet->query_food(), pet->query_food_maximum()),
        pet->query_food(),
        pet->query_food_maximum() );

    return;
}


void feed_dog(object me, string item)
{
    int stuff, heal;
    object food, pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("请先叫出你的忍犬。\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 30 ) {
        write("你的驯兽术必需大于20级才能使用这个指令。\n");
        return;
    }

    food = present(item, me);
    if( !food ) food = present(item, environment(me));
    if( !food ) {
        write("这里没有这样东西 ...\n");
        return;
    }

    stuff = food->query("food_stuff");
    if( !stuff || !food->stuff_ob(pet)) {
        write("这样东西似乎不能吃...\n");
        return;
    }
    // 增加食物量
    pet->food_gain(stuff);

    message_vision("$N吃下了一" + ( food->query("base_unit") ? food->query("base_unit") : food->query("unit") ) + "$n。\n", pet, food);
    
    food->add_amount(-1);
    if( food->query_amount() <= 0 ) destruct(food);

    // 吃东西会增加经验值
    pet->exp_gain(stuff);
    
    return;
}

void set_data(object me, string key, string value)
{
    object pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("请先叫出你的忍犬。\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 100 ) {
        write("你的驯兽术必需大于100级才能使用这个指令。\n");
        return;
    }

    if( key == "name" ) {
        if( strlen(value) > 10 ) {
            write("你只能为你的忍犬取五个中文字大小以内的名字。\n");
            return;
        }
        write("你帮你的忍犬取名为 " +value + "。\n");
        pet->set("name", value);
        return;
    }
    
    if( key == "title" ) {
        if( strlen(value) > 14 ) {
            write("你只能为你的忍犬取七个中文字大小以内的头衔。\n");
            return;
        }
        write("你帮你的忍犬的头衔取为 " +value + "。\n");
        pet->set("title", value);
        return;
    }

    if( key == "nickname") {
        if( strlen(value) > 14 ) {
            write("你只能为你的忍犬取七个中文字大小以内的外号。\n");
            return;
        }
        write("你帮你的忍犬的外号取为 " +value + "。\n");
        pet->set("nickname", value);
        return;
    }
    
    if( key == "long") {
        if( strlen(value) > 200 ) {
            write("你只能为你的忍犬设定一百个中文字大小以内的描述。\n");
            return;
        }
        write("你帮你的忍犬的描述设为 " +value + "。\n");
        pet->set("long", value);
        return;
    }

    write("你目前能改更忍犬的资讯有 name, title, nickname, long ...\n");
    return;
}

// 使用绝招
int perform_action(object me, string act, object target)
{
    string k, v;
    if( !objectp(me) ) return 0;
    if( me->is_busy() ) return notify_fail("你现在没有空使用驯兽之术。\n");

    switch( act ) {
        case     "call": call_dog(me); break;
        case    "leave": destruct_dog(me); break;
        case    "state": show_state(me); break;
        default:
            if( sscanf(act, "feed %s", k) == 1 ) {
                feed_dog(me, k);
            } else if( sscanf(act, "set %s %s", k, v) == 2 ) {
                set_data(me, k, v);
            } else return notify_fail("目前驯兽之术有 feed, call, leave, state, set 五种功\能。\n"); break;
        break;
    }
    
    return 1;
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);    // 技能等级

    switch( lv ) {
        case 189: me->set_skill_require(skill, 1500000);  break; // 关卡3 : 189升190 需要一百五十万经验
        case 149: me->set_skill_require(skill, 1000000);  break; // 关卡2 : 149升150 需要一百万经验
        case  99: me->set_skill_require(skill, 500000);   break; // 关卡1 :  99升100 需要五十万经验
        default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}