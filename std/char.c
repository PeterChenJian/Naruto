/*  char.c - standard character body

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <login.h>
#include <action.h>
#include <ansi.h>
#include <char.h>
#include <command.h>
#include <condition.h>
#include <dbase.h>
#include <move.h>
#include <name.h>
#include <skill.h>
#include <user.h>

inherit F_ACTION;
inherit F_ATTACK;
inherit F_ATTRIBUTE;
inherit F_COMBAT;
inherit F_COMMAND;
inherit F_CONDITION;
inherit F_DBASE;
inherit F_FINANCE;
inherit F_MESSAGE;
inherit F_MASTER;
inherit F_MOVE;
inherit F_NAME;
inherit F_SCORE;
inherit F_SKILL;
inherit F_STATISTIC;
inherit F_TEAM;

// variables

static int tick;

// implementations

int is_character() { return 1; }

void die();

/* setup()
 *
 * 这个函式用来完成一个人物的初始化，并启动所有人物物件可以接受的事件，包括
 * 心跳(heart beat)，指令(command)等。
 * setup 还会呼叫 CHAR_D 来完成 game-specific 的人物初始化。
 */

void setup()
{
    seteuid(getuid());

    // 只有复制的物件需要打开 heart beat 及以下的设定，用来节省 CPU time
    if( !clonep(this_object()) ) return;

    enable_player();
    init_command();
    CHAR_D->setup_char( this_object() );

    set_heart_beat(1);
    tick = 5 + random(10);
}

/* heart_beat()
 *
 * 人物的心跳函式，这个函式每两秒钟会被 driver 呼叫一次，让人物得以进行
 * 战斗等动作；使用者以及 NPC 特有的功能分别定义在 NPC 及 USER 的物件中
 * 。
 *
 * 由于这个函式被执行的次数是整个 mudlib 中最高的，因此若不作相当的最佳
 * 化，很容易导致效能的大幅降低，如果你对 LPC 还没有很熟，请不要随意更动
 * 这个函式的内容。
 */

static void heart_beat()
{
    mapping f;
    int f_altered = 0;

    if( mapp(f=query_destroyed()) && sizeof(f) ) {
		f_altered++;
		RACE_D(query_race())->statistic_destroyed(this_object(), f);
        if( !this_object() ) return;
    }

    if( mapp(f=query_exhausted()) && sizeof(f) ) {
		f_altered++;
		RACE_D(query_race())->statistic_exhausted(this_object(), f);
        if( !this_object() ) return;
    }

    if( mapp(f=query_notified()) && sizeof(f) ) {
		f_altered++;
		this_object()->notify_stat(f);
        if( !this_object() ) return;
    }

    if( f_altered ) clear_statistic_flags();

    /* 战斗回合的控制 */
    if( living(this_object()) )
    {
		if( is_fighting() ) clean_up_enemy();
		if( is_busy() ) continue_action();
		else if( is_fighting() ) attack();
    }

    /* 以下是 tick 时间到的时候才会执行的部分。 */
    if( tick-- ) return;

    /* 下一个 tick 的时间间隔取乱数，将所有生物的 heart beat 错开，以免造
     * 成系统的负担过于集中在特定的 tick。
     */
    tick = 20 + random(10);

    update_condition();
    regenerate();
}

void lose_fight(object opponent)
{
    if( opponent ) opponent->win_fight(this_object());
}

int notify_stat(mapping flag)
{
    // 如果有设定预设逃跑方向，则先尝试往这个方向逃跑，否则随机选一个方向跑。
    string dir;
    if( !living(this_object()) || !environment() ) return 0;
    if( stringp(dir = query("wimpy_dir")) ) return command("go "+ dir);
    else command("go escape");
}

varargs void revive(int quiet)
{
    object ob;
    string arg, arg2;

    remove_call_out("revive");

    // 如果这个人物被其他人物携带著，移动到携带者的外面
    while( environment()->is_character() )
        this_object()->move(environment(environment()));

    enable_player();
    init_command();		// Add by -Acme- 昏倒醒来时，没办法使用指令

    delete_temp("block_msg/all");

    if( !quiet ) {
        COMBAT_D->announce(this_object(), "revive");
        message("system", HIY "\n慢慢地你终于又有了知觉....\n\n" NOR, this_object());
    }
}

/*  人物的死亡
 */
void die()
{
    object corpse, ob;

    // 停止一切的战斗、保护、攻击选择的关系
    remove_all_killer();
    guard_ob(0);
    charge_ob(0);
    dismiss_team();

    // 停止任何正在进行的动作
    if( is_busy() ) interrupt(this_object(), INTR_DEATH);

    // 清除所有的 condition
    clear_condition();

    // 宣告这个人物的死亡
    COMBAT_D->announce(this_object(), "dead");

    // 如果这名人物在昏迷中，让他“安静地”醒过来，这样才看得到自己死亡的讯息
    if( !living(this_object()) ) revive(1);

    tell_object(this_object(), HIR "\n你死了！！\n\n" NOR);
    CHAR_D->make_corpse(this_object());

    if( userp(this_object()) ) {
    	int lv;
        mapping skills;
        string err, sk, *sname;
        object room;
        
        // 死亡惩罚，所有技能降低一级
        skills = this_object()->query_skills();
        if( !mapp(skills) ) skills = ([]);
        sname = keys(skills);
        
        foreach( sk in sname ) {
        	lv = this_object()->query_skill(sk, 1);
        	if( lv > 1 ) this_object()->set_skill(sk, lv-1);
        }

        // 人物死亡，所有的世仇都删除
        this_object()->delete("vendetta");
        
	    // 人物移到复生地
        err = catch(room = load_object(this_object()->query("recoverroom")));
        if( !room ) err = catch(room = load_object(START_ROOM));
        if( !room ) err = catch(room = load_object(VOID_OB));
        if( room && this_object()->move(room) )
            message_vision(HIW"一阵白光过去，$N“唉哟～”一声，突然从天空中掉了下了，跌了个狗吃屎。\n"NOR, this_object());
    }

    return;
}

varargs void unconcious(string reason)
{
    object ob, me=this_object();

    // 已经昏迷 ?
    if( !living(me) ) return;

    // 巫师不能昏迷
    if ( wizardp(this_object()) ) {
        message("system", HIW "\n你的巫师能力治愈了身上全部的伤害 ....\n\n" NOR, me);
        me->set_stat_current("ap", me->query_stat_maximum("ap"));
        me->set_stat_current("hp", me->query_stat_maximum("hp"));
        me->set_stat_current("mp", me->query_stat_maximum("mp"));
        return;
    }

    // 停止所有 fight 中的敌人
    remove_all_enemy();

    // 中断正在进行的动作
    if( is_busy() ) interrupt(me, INTR_UNCONCIOUS);

    message("system", HIR "\n你的眼前一黑，接著什么也不知道了....\n\n" NOR, this_object());

    disable_player(stringp(reason) ? reason : " <昏迷不醒>");
    set_temp("block_msg/all", 1);

    COMBAT_D->announce(this_object(), "unconcious");

    // 设定一分钟后苏醒
    remove_call_out("revive");
    call_out("revive", 60);
}

// overrides
int receive_object(object ob, int from_inventory)
{
    if( from_inventory ) return 1;

    if( query_encumbrance() + ob->weight() > query_ability("carriage") )
        return notify_fail(ob->name() + "现在对你来说太重了。\n");

    if( sizeof(all_inventory(this_object())) >= 30 )
        return notify_fail("你身上已经没有空位再携带其他东西了。\n");

    return 1;
}

varargs int move(mixed dest, int silent)
{
    if( interactive(this_object()) && is_busy() )
	return notify_fail("你现在无法移动﹗\n");

    if( ::move(dest, silent) ) {
		if( interactive(this_object()) && !silent )
	    	environment()->do_look(this_object());
		return 1;
    }

    return 0;
}

string query_id()
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id")) && sizeof(applied) && arrayp(applied[<1]) && stringp(applied[<1][0]) )
        return applied[<1][0];

    return query("id");
}

int id(string str)
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id")) && sizeof(applied) && arrayp(applied[<1]) )
		return member_array(str, applied[<1]) >= 0;
    return ::id(str);
}

varargs string name(int raw)
{
    string *applied_name;

    if( !raw && arrayp(applied_name = query_temp("apply/name")) && sizeof(applied_name) && stringp(applied_name[<1]) )
		return applied_name[<1];
    return ::name(raw);
}

/*  short()
 *
 *  这个函数传回人物的简短叙述，使用者在 look 一个房间中，若有其他人物，则
 *  显示的就是这个字串。
 */
varargs string short(int raw)
{
    string str, title, nick, *applied;

    str = ((title = query("title")) ? title : "")
        + ((nick = query("nickname")) ? ("“"+nick+"”") : (title?" ":""))
        + ::short(1);

    if( raw ) return str;

    // 附加的短叙述
    if(	arrayp(applied = query_temp("apply/short")) && sizeof(applied) && stringp(applied[<1]) ) {
		str = applied[<1];
		if( !this_player()->query("option/BRIEF_ID") )
	    	str += "(" + capitalize(query_id()) + ")";
    }

    if( !living(this_object()) ) str += HIR + query("disable_type") + NOR;

    if( query_temp("pending/hidden") )
		str = GRN + " <潜藏> " + str + NOR;

    if( this_object()->is_fighting() ) str += HIR" <战斗>"NOR;

    if( !userp(this_object()) ) return str;

    if( !interactive(this_object()) ) str += HIG " <断线>" NOR;
    else if( query_idle( this_object() ) > 120 ) str += HIM " <发呆>" NOR;

    if( in_input() ) str += HIC " <输入>" NOR;
    if( in_edit() ) str += HIY " <编辑>" NOR;

    return str;
}

varargs string long(int raw)
{
    string str, *applied;

    if( !raw && arrayp(applied = query_temp("apply/long"))  && sizeof(applied) && stringp(applied[<1]) )
		str = applied[<1];
    else str = ::long(raw);

    if( !raw && arrayp(applied = query_temp("apply/description")) && sizeof(applied) && stringp(applied[<1]) ) {
		str += implode(applied, "\n");
		str = replace_string(str, "$N", this_object()->name(raw));
    }

    return str;
}

varargs string rank(string politeness, int raw)
{
    string c, r;

    if( c = this_object()->query_class() )
        r = CLASS_D(c)->query_rank(this_object(), politeness);
    else
        r = ::rank(politeness, raw);

    if( !politeness && !raw ) {
        switch(wizhood(this_object())) {
        	case "(admin)":      return "天尊";
        	case "(arch)":       return "尊者";
        	case "(wizard)":     return "贤人";
        	case "(apprentice)": return "高人";
        	case "(immortal)":   return "野人";
        }
    }
    return r;
}

nomask int visible(object ob)
{
    int a, b;

    // 任何人物永远都可以看到自己
    if( ob==this_object() ) return 1;

    // 巫师永远可以看到巫师等级比自己低的物件
    a = wiz_level(ob);
    b = wiz_level(this_object());
    if( wizardp(ob) && a>b ) return 1;
    if( query("invis") && a<=b ) return 0;

    if( !::visible(ob) ) return 0;

    // 隐藏的物件需要足够的能力才能发现
    if( query_temp("pending/hidden") > ob->query_ability("awareness") )
		return 0;

    return 1;
}