/* poker table

    朴克牌 - 大老二
	
    变数：
    clear(重设牌局)	0-未设牌局, 1-牌局重设
    start(开始游戏)	0-未开始, 1-开始
    focus(出牌权)	0-未知, 1-palyer1出牌, 2-player2出牌, 3-player3出牌，4-player4出牌
    play_kind(出牌方式) 0-随意, 1-练单, 2-对子, 3-葫芦, 4-顺子, 5-铁支, 6-同花顺, 7-须梅花三
    owner		牌面上最大者
    power		牌子的大小

						write by Acme@ED  2002.1.29 
*/

#include <ansi.h>

inherit ITEM;

mapping poker=([]);
mapping pok=([]);
mapping p1=([]);
mapping p2=([]);
mapping p3=([]);
mapping p4=([]);
mapping score=(["clear":0,"start":0,"focus":0,"play_kind":0,"owner":"","power":0,"person":0]);
mapping board=(["p1":0,"p2":0,"p3":0,"p4":0]);

/* 指令函式 */
int do_sit();
int do_reset();
int do_start();
int do_see(string arg);
int do_dump(string arg);
int do_pass();
int do_leave();
int do_rules();
int do_check();
int do_score();

string show_poker(mapping mm,int s);
string show_number(int i);
string show_flower(string str);
string show_score();
string focus_name();
int check_player(string name);	
int check_poker();
int check_over(string name);
void check_board(string winner);
string long();

void create()
{
        set_name("“大老二” 牌桌", ({ "poker table","table"}));
	set("long", long() );
        set("no_get", 1);
        setup();
}

void init()
{
	add_action("do_sit","sit");
	add_action("do_reset","reset");
	add_action("do_start","start");
	add_action("do_see",".");
	add_action("do_dump","du");
	add_action("do_pass","pa");
	add_action("do_check","ch");
	add_action("do_leave","leave");
	add_action("do_rules","rules");
	add_action("do_score",",");
}

int do_sit()
{
	object me=this_player();
	if( score["start"] ) return notify_fail("牌局已经开始了。\n");
	if( !check_player(me->query_id()) )
	{
		if( undefinedp(score["p1"]) ) {
			score["p1"]=me->query_id();
			message_vision("$N坐上了牌桌的北边位子。\n",me);
			board["p1"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p2"]) ) {
			score["p2"]=me->query_id();
			message_vision("$N坐上了牌桌的东边位子。\n",me);
			board["p2"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p3"]) ) {
			score["p3"]=me->query_id();
			message_vision("$N坐上了牌桌的南边位子。\n",me);
			board["p3"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p4"]) ) {
			score["p4"]=me->query_id();
			message_vision("$N坐上了牌桌的西边位子。\n",me);
			board["p4"]=100;
			score["person"]+=1;
		}
		else return notify_fail("牌桌已经坐满了。\n");
	}
	else return notify_fail("你已经坐在椅子上了。\n");
	return 1;
}

int do_reset()
{
	int i;

	if( !check_poker() )
	{
		message_vision("旧局无法继续，$N将牌局重新设定...",this_player());
		score=([]);
	}
	else if( !check_player(this_player()->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	/* 重设变数 */
	score["clear"] = 1;
	score["start"] = 0;
	score["focus"] = 0;
	score["power"] = 0;
	score["owner"] = "";
	score["play_kind"] = 0;

	p1=([]);
	p2=([]);
	p3=([]);
	p4=([]);
	poker["m"]=([]);
	poker["f"]=([]);
	poker["h"]=([]);
	poker["t"]=([]);

	/* 设 52 张牌，并设定大小 */
	for(i=1;i<=13;i++)
	{
		if( i == 2 ) {		/* 老 2 最大 */
			poker["m"][i]=49;
			poker["f"][i]=50;
			poker["h"][i]=51;
			poker["t"][i]=52;
		}
		else if( i == 1 ) {	/* A 第二大 */
			poker["m"][i]=45;
			poker["f"][i]=46;
			poker["h"][i]=47;
			poker["t"][i]=48;
		} else {		/* 其它牌按公式给大小 */
			poker["m"][i]=i-2+(i-3)*3;
			poker["f"][i]=i-1+(i-3)*3;
			poker["h"][i]=i+(i-3)*3;
			poker["t"][i]=i+1+(i-3)*3;
		}
	}

	message_vision("$N将牌重新洗过一次。\n",this_player());
	return 1;
}

int do_start()
{
	int i, point,value;
	string kind;

	if( !check_player(this_player()->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( !check_poker() )
		return notify_fail("人数不足、或者有牌友在发呆、断线，无法开始。\n");

	if( !score["clear"] ) return notify_fail("还没有洗牌。\n");
	if( score["start"] ) return notify_fail("已经开始了。\n");

	/* 轮流发牌 */
	point=1;
	for(i=1;i<=52;i++)
	{
		/* 花色无牌时，删除花色 */
		if( sizeof(poker["m"]) < 1 ) map_delete(poker,"m");
		if( sizeof(poker["f"]) < 1 ) map_delete(poker,"f");
		if( sizeof(poker["h"]) < 1 ) map_delete(poker,"h");
		if( sizeof(poker["t"]) < 1 ) map_delete(poker,"t");
		
		/* 乱数取牌 */
		kind=keys(poker)[random(sizeof(keys(poker)))];
		value=keys(poker[kind])[random(sizeof(values(poker[kind])))];
		
		switch( point )
		{
			case 1:
				if( undefinedp(p1[kind]) ) p1[kind]=([]);
				p1[kind][value]=poker[kind][value];
				break;
			case 2:
				if( undefinedp(p2[kind]) ) p2[kind]=([]);
				p2[kind][value]=poker[kind][value];
				break;
			case 3:
				if( undefinedp(p3[kind]) ) p3[kind]=([]);
				p3[kind][value]=poker[kind][value];
				break;
			case 4:
				if( undefinedp(p4[kind]) ) p4[kind]=([]);
				p4[kind][value]=poker[kind][value];
				break;
		}

		map_delete(poker[kind],value);

		if( kind == "m" && value==3 ) {
			score["focus"]=point;
			/* 三个人玩手上有梅花三者，多一张牌 */
			if( score["person"]==3 ) point--;
		}
		point++;
		if( score["person"]==3 && point > 3 ) point =1;
		else if( point>4 ) point=1;
	}
	kind=focus_name();
	message_vision(HIW"$N将牌洗了洗，发了"+chinese_number(score["person"])+"份... 梅花三在"+kind+"手上，请"+kind+"先行出牌。\n"NOR,this_player());
	score["start"] = 1;
	score["play_kind"] = 7;
	return 1;
}

int do_see(string arg)
{
	if( !check_player(this_player()->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( !score["start"] ) 
		return notify_fail("牌局还没开始！\n");

	/* 巫师作弊专用，唉～总要留个后门走走吧！...:P
	   不然每次跟玩家玩都玩轮，岂不丢脸？....Hehe....	
	 */
	if( arg && !wizardp(this_player()) )
		return notify_fail("你不能偷看别人的牌！\n");
	if( !arg ) arg=this_player()->query_id();

	switch( check_player(arg) )
	{
		case 1:	write( "【"+score["p1"]+"】"+show_poker(p1,1) ); break;
		case 2: write( "【"+score["p2"]+"】"+show_poker(p2,1) ); break;
		case 3:	write( "【"+score["p3"]+"】"+show_poker(p3,1) ); break;
		case 4:	write( "【"+score["p4"]+"】"+show_poker(p4,1) ); break;
		default: return notify_fail(arg+"没有加入这牌局。\n"); break;
	}
	return 1;
}

int do_dump(string arg)
{
	int i, len, p, num, d=0;
	string flo;
	mapping p_num=([]), p_flo=([]);

	if( !arg ) return notify_fail("你要打哪张牌？\n");

	if( !check_player(this_player()->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( !score["start"] ) 
		return notify_fail("牌局还没开始！\n");

	if( (flo=focus_name())=="??" )
		return notify_fail("这牌局还没重设、或还未发牌。\n");

	if( flo != this_player()->query_id() )
		return notify_fail("现在是轮到"+flo+"打牌，还没换你！\n");

	len=strlen(arg);
	if( len > 19 ) return notify_fail("你丢太多牌了，最多只能丢五张牌！\n");

	/* 抓取丢的牌 */
	for(i=0;i<len;i++)
	if( arg[i..i] == " " || i==len-1 ) {

		if( i!=len-1 ) {
			if( sscanf(arg[p..i-1], "%d%s", num, flo) != 2 )
				return notify_fail("你丢的牌有问题哦！\n");
		} else {
			if( sscanf(arg[p..len], "%d%s", num, flo) != 2 )
				return notify_fail("你丢的牌有问题哦！\n");
		}

		if( num < 1 || num > 13 )
			return notify_fail("你丢的牌大小错了！\n");
		if( flo != "t" && flo != "h" && flo != "f" && flo != "m" )
			return notify_fail("你丢的牌花色有问题哦！\n");

		/* 检查丢的牌有无重覆 */
		if( !undefinedp(p_flo[flo]) && !undefinedp(p_flo[flo][num]) )
			return notify_fail("你丢的牌重覆了哦！\n");

		/* 判断手中有无此牌..&& 将牌丢到mapping里，方便以后判断
		   && 设定大小
		*/
		switch( score["focus"] ) {
			case 1:
				if( undefinedp(p1[flo]) || undefinedp(p1[flo][num]) )
					return notify_fail("你手上没有"+show_flower(flo)+show_number(num)+"这张牌。\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p1[flo][num];
				p_flo[flo][num]=p1[flo][num];
				break;
			case 2:
				if( undefinedp(p2[flo]) || undefinedp(p2[flo][num]) )
					return notify_fail("你手上没有"+show_flower(flo)+show_number(num)+"这张牌。\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p2[flo][num];
				p_flo[flo][num]=p2[flo][num];
				break;
			case 3:
				if( undefinedp(p3[flo]) || undefinedp(p3[flo][num]) )
					return notify_fail("你手上没有"+show_flower(flo)+show_number(num)+"这张牌。\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p3[flo][num];
				p_flo[flo][num]=p3[flo][num];
				break;
			case 4:
				if( undefinedp(p4[flo]) || undefinedp(p4[flo][num]) )
					return notify_fail("你手上没有"+show_flower(flo)+show_number(num)+"这张牌。\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p4[flo][num];
				p_flo[flo][num]=p4[flo][num];
				break;
		}	
		d++;
		p=i+1;
	}
		
	/* 牌局一开始必须丢出梅花三 */
	if( score["play_kind"]==7 && ( undefinedp(p_flo["m"]) || undefinedp(p_flo["m"][3]) ) )
		return notify_fail("牌局一开始必须丢出梅花三。\n");

	if( score["owner"] == this_player()->query_id() ) {
		score["power"] = 0;
		score["play_kind"] = 0;
	}

	switch( d ) {
		case 1: /* 练单 */
			if( score["play_kind"] != 1 && score["play_kind"] != 0 && score["play_kind"] != 7 )
				return notify_fail("现在不是练单的时候。\n");

			/* check power */
			p=0;
			foreach( flo in keys(p_flo) )
			foreach( i in keys(p_flo[flo]) )
				p=p_flo[flo][i];
			if( p < score["power"] ) 
				return notify_fail("你丢的牌不够大哦！\n");

			score["play_kind"] = 1;
			score["power"]=p;
			message_vision("$N说道："HIW"“ "NOR+YEL"练单！"HIW"”"NOR"\n",this_player());
			break;

		case 2: /* 对子 */
			if( score["play_kind"] != 2 && score["play_kind"] != 0 && score["play_kind"] != 7 )
				return notify_fail("现在不是丢对子的时候。\n");

			/* check是否为对子 */
			if( sizeof( keys(p_num) ) > 1 )
				return notify_fail("丢二张牌时，必需丢对子才行。\n");

			/* check power */
			p=0;
			foreach( flo in keys(p_flo) )
			foreach( i in keys(p_flo[flo]) )
				if( p < p_flo[flo][i] ) p=p_flo[flo][i];
			if( p < score["power"] ) 
				return notify_fail("你丢的对子不够大哦！\n");
			score["play_kind"] = 2;
			score["power"]=p;
			message_vision("$N说道："HIW"“ "NOR+MAG"对子！"HIW"”"NOR"\n",this_player());
			break;

		case 3: return notify_fail("没有这种组合的牌哦。\n"); break;
		case 4:	return notify_fail("没有这种组合的牌哦。\n"); break;

		case 5: 
			num=sizeof(keys(p_num));
			/* 顺子、同花顺 */
			if( num == 5) {
				int *check, x, y, swap;
				check=keys(p_num);

				/* 排序 */
			        for(x=4;x>0;x--)
		                for(y=1;y<=x;y++)
				if( check[y-1] < check[y] )
				{
					swap=check[y-1];
					check[y-1]=check[y];
					check[y]=swap;
                		}
				
				/* check是否为顺子,1 10,11,12,13 也是顺子 */
				if( check[0]!=13 || check[1]!=12 || check[2]!=11 || check[3]!=10 || check[4]!=1 ) {
					y=check[0];
					for(x=1;x<5;x++) {
						if( y-check[x] != 1 )
							return notify_fail("没有这种组合的牌吧？(5)\n");
						y=check[x];
					}
				}

				/* check power 1~5最小 */
				if( check[0]==5 && check[1]==4 && check[2]==3 && check[3]==2 && check[4]==1 ) {
					foreach( flo in keys(p_num[5]) )
						p=p_num[5][flo];
				} else {
					p=0;
					foreach( flo in keys(p_flo) )
					foreach( i in keys(p_flo[flo]) )
						if( p < p_flo[flo][i] ) p=p_flo[flo][i];
				}

				/* check 是否同花 */
				num=0;
				foreach( flo in keys(p_flo) )
				if( num < sizeof(keys(p_flo[flo])) ) num = sizeof(keys(p_flo[flo]));

				if( num==5 ) {
					if( score["play_kind"] == 6 && p < score["power"] )
						return notify_fail("你丢的同花顺不够大哦！\n");
					score["play_kind"]=6;
					message_vision("$N狂笑道："HIW"“ "HIY"哇！哈！哈！同花顺！"HIW"”"NOR"\n",this_player());
				} else {
					if( score["play_kind"] != 4 && score["play_kind"] != 0 && score["play_kind"] != 7 )
						return notify_fail("现在不是出顺子的时候。\n");
					if( p < score["power"] ) 
						return notify_fail("你丢的顺子不够大哦！\n");
					score["play_kind"]=4;
					message_vision("$N叫道："HIW"“ "HIM"顺子！"HIW"”"NOR"\n",this_player());
				}
				score["power"]=p;

			/* 铁支、葫芦 */
			} else if( num == 2 ) {
				i=0; len=0; p=0;
				foreach( num in keys(p_num) )
				if( i < sizeof(keys(p_num[num])) ) {
					i=sizeof(keys(p_num[num]));
					len=num;
				}

				/* 葫芦 */
				if( i == 3 ) {
					if( score["play_kind"] != 3 && score["play_kind"] != 0 && score["play_kind"] != 7 )
						return notify_fail("现在不是出葫芦的时候。\n");
					foreach( flo in keys(p_num[len]) )
						if( p < p_num[len][flo] ) p = p_num[len][flo];
					if( score["power"] > p )
						return notify_fail("你丢的葫芦不够大哦！\n");
					score["play_kind"]=3;
					message_vision("$N喊道："HIW"“ "HIG"葫芦！"HIW"”"NOR"\n",this_player());
				/* 铁支 */
				} else if( i==4 ) {
					if( score["play_kind"] == 6 )
						return notify_fail("现在不是出铁支的时候。\n");
					foreach( flo in keys(p_num[len]) )
						if( p < p_num[len][flo] ) p = p_num[len][flo];
					if( score["play_kind"] == 5 && score["power"] > p )
						return notify_fail("你丢的铁支不够大哦！\n");
					score["play_kind"]=5;
					message_vision("$N大声笑道："HIW"“ "HIC"哈！哈！铁支！"HIW"”"NOR"\n",this_player());

				} else return notify_fail("你出的牌组合有问题哦。\n");

				score["power"]=p;

			} else return notify_fail("没有这种组合的牌哦。\n"); break;

			break;
		default: return notify_fail("你丢的牌太多张了，最多五张。\n"); break;
	}

	pok=p_flo;
	/* 丢出的牌从手中清除 */
	foreach( flo in keys(p_flo) )
	foreach( i in keys(p_flo[flo]) )
	switch( score["focus"] )
	{
		case 1: map_delete(p1[flo],i); break;
		case 2: map_delete(p2[flo],i); break;
		case 3: map_delete(p3[flo],i); break;
		case 4: map_delete(p4[flo],i); break;
	}
	score["owner"]=this_player()->query_id();

	message_vision(show_poker(p_flo,0),this_player());

	if( !check_over(this_player()->query_id()) ) {
		score["focus"] += 1;
		if( score["person"]==3 && score["focus"] > 3 ) score["focus"]=1;
		else if( score["focus"] > 4 ) score["focus"]= 1;
		message_vision("... 轮到"+focus_name()+"打牌。\n",this_player());
	} else {
		message_vision("\n$N发了狂般笑道："HIW"“ "HIY"口圭！口合！口合！口合！ 我  赢  了！！！"HIW"”\n\n"NOR,this_player());

		check_board(this_player()->query_id());
		do_reset();
	}

	return 1;
}

int do_pass()
{
	string who;
	object me=this_player();

	if( !check_player(me->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( !score["start"] ) 
		return notify_fail("牌局还没开始！\n");

	if( (who=focus_name())=="??" )
		return notify_fail("这牌局还没重设、或还未发牌。\n");

	if( who != me->query_id() )
		return notify_fail("现在是轮到"+who+"打牌，还没换你！\n");

	if( score["play_kind"]==7 )
		return notify_fail("现在起码要丢一张梅花三，不行Pass哦。\n");

	if( score["owner"] == me->query_id() )
		return notify_fail("你不能够Pass，你就随便丢几张牌吧。\n");

	score["focus"] += 1;
	if( score["person"] == 3 && score["focus"] > 3 )  score["focus"]=1;
	else if( score["focus"] > 4 ) score["focus"]= 1;

	message_vision("$N大声说道："HIW"‘ "HIY"Ｐａｓｓ！"HIW"’"NOR"... 轮到"+focus_name()+"打牌。\n",me);
	return 1;
}

int do_check()
{
	string msg;
	
	if( !check_player(this_player()->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( !score["start"] ) 
		return notify_fail("牌局还没开始！\n");

	if( focus_name()=="??" )
		return notify_fail("这牌局还没重设、或还未发牌。\n");

	if( sizeof(pok) < 1 )
		return notify_fail("目前桌面上没有牌！\n");

	msg = "由"+score["owner"]+"所出的牌：";
	msg += show_poker(pok,0);
	msg += "接下来换"+focus_name()+"出牌。\n";
	write(msg);
	return 1;
}

int do_leave()
{
	object me=this_player();

	if( !check_player(me->query_id()) )
		return notify_fail("你并没有参与这次的牌局。\n");

	if( score["p1"] == me->query_id() ) 
		map_delete(score,"p1");
	else if( score["p2"] == me->query_id() )
		map_delete(score,"p2");
	else if( score["p3"] == me->query_id() )
		map_delete(score,"p3");
	else {
		if( undefinedp(score["p4"]) ) return 0;
		if( score["p4"] == me->query_id() )
		map_delete(score,"p4");
	}

	message_vision("$N离开了牌桌不玩了。\n",me);
	score["person"]-=1;
	if( score["start"] ) do_reset();
	return 1;
}

int do_rules()
{
	write(@rules
	╔═══════════════════════════╗
	║               朴克牌-大老二 游戏规则说明	        ║
	╠═══════════════════════════╣
	║  大家都知道了... 懒得打了 ccc..                      ║
	║                                                      ║
	║  需要注意的是：                                      ║
	║       一、没有“吐呸”                               ║
	║       二、没有“三条”                               ║
	║       三、顺子1~5是最小的                            ║
	║       四、只要其中一个玩者手中没牌即赢了             ║
	║       五、三个人也可以玩，手中有梅花三的人多一张牌   ║
	║                                                      ║
	║  计分方式：                                          ║
	║       当牌局结束后，手中剩几张牌就扣几点分数         ║
	║       如果超过十张则乘二，如果有大老二也乘二         ║
	║       其它人扣的点数则加给赢的人。                   ║
	║                                                      ║
	║                             write by -Acme@ED-       ║
	╚═══════════════════════════╝
rules);

	return 1;
}

int do_score()
{
	if( !score["start"] ) 
		return notify_fail("牌局还没开始！\n");

	if( focus_name()=="??" )
		return notify_fail("这牌局还没重设、或还未发牌。\n");

	write(show_score()+"\n");

	return 1;
}

/* 显示目前出牌人之名字 */
string focus_name()
{
	if( undefinedp(score["focus"]) ) return "??";
	switch( score["focus"] )
	{
		case 1: if( !undefinedp(score["p1"]) ) return score["p1"]; break;
		case 2: if( !undefinedp(score["p2"]) ) return score["p2"]; break;
		case 3: if( !undefinedp(score["p3"]) ) return score["p3"]; break;
		case 4: if( !undefinedp(score["p4"]) ) return score["p4"]; break;
	}
	return "??";
}

/* 显示手上的牌 */
string show_poker(mapping mm,int s)
{
	int i, t;
	string msg="", msg2="",bar_up,bar_down, num;

	t=sizeof(mm["t"])+sizeof(mm["h"])+sizeof(mm["f"])+sizeof(mm["m"])-1;
	bar_up = "\n╔";
	bar_down = "╚";
	while( t-- ) {
		bar_up += "═╦";
		bar_down += "═╩";
	}
	bar_up += "═╗\n";
	bar_down += "═╝\n";

	/* 按照大小排列 */
	for(i=13;i>0;i--)
	{
		num=show_number(i);
		if( member_array("t",keys(mm))!=-1 && !undefinedp(mm["t"][i]) ) {
			msg+=sprintf("║%s",num);
			msg2+="║"+HIK"桃"NOR;
		}
		if( member_array("h",keys(mm))!=-1 && !undefinedp(mm["h"][i]) ) {
			msg+=sprintf("║%s",num);
			msg2+="║"+HIR"心"NOR;
		}
		if( member_array("f",keys(mm))!=-1 && !undefinedp(mm["f"][i]) ) {
			msg+=sprintf("║%s",num);
			msg2+="║"+HIR"方"NOR;
		}
		if( member_array("m",keys(mm))!=-1 && !undefinedp(mm["m"][i]) ) {
			msg+=sprintf("║%s",num);
			msg2+="║"+HIK"梅"NOR;
		}
	}
	if( s ) {
		msg+="║\tt->"HIK"黑桃"NOR", h->"HIR"红心"NOR"\n";
		msg2+="║\tf->"HIR"方块"NOR", m->"HIK"梅花"NOR"\n";
	}
	else {
		msg+="║\n";
		msg2+="║\n";
	}
	return bar_up+msg+msg2+bar_down;
}

/* 秀数字 */
string show_number(int i)
{
	switch( i ) {
		case 1: return "Ａ"; break;
		case 2: return HIW"２"NOR; break;
		case 3: return "３"; break;
		case 4: return "４"; break;
		case 5: return "５"; break;
		case 6: return "６"; break;
		case 7: return "７"; break;
		case 8: return "８"; break;
		case 9: return "９"; break;
		case 10: return "10"; break;
		case 11: return "Ｊ"; break;
		case 12: return "Ｑ"; break;
		case 13: return "Ｋ"; break;
	}
	return "??";
}

/* 秀花色 */
string show_flower(string str)
{
	switch( str ) {
		case "t": return "黑桃"; break;
		case "h": return "红心"; break;
		case "f": return "方块"; break;
		case "m": return "梅花"; break;
	}
	return "??";
}

string show_score()
{
	string msg="";

	if( !undefinedp(board["p1"]) && !undefinedp(score["p1"]) )
		msg+=score["p1"]+"目前为"+board["p1"]+"分  ";

	if( !undefinedp(board["p2"]) && !undefinedp(score["p2"]) )
		msg+=score["p2"]+"目前为"+board["p2"]+"分  ";

	if( !undefinedp(board["p3"]) && !undefinedp(score["p3"]) )
		msg+=score["p3"]+"目前为"+board["p3"]+"分  ";

	if( !undefinedp(board["p4"]) && !undefinedp(score["p4"]) )
		msg+=score["p4"]+"目前为"+board["p4"]+"分  ";

	return msg;
}

/* 检查是否坐在牌桌前 */
int check_player(string name)
{
	if( name == score["p1"] ) return 1;
	else if( name == score["p2"] ) return 2;
	else if( name == score["p3"] ) return 3;
	else {
		if( undefinedp(score["p4"]) ) return 0;
		if( name == score["p4"] ) return 4;
	}
	return 0; 
}

/* 检查牌局是否可以继续执行 */
int check_poker()
{
	object user;
	if( !undefinedp(score["p1"]) && 
	    /* 玩家离开、断线或发呆，无法进行 */
	    user = present(score["p1"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	if( !undefinedp(score["p2"]) && 
	    user = present(score["p2"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	if( !undefinedp(score["p3"]) && 
	    user = present(score["p3"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	/* 三人也可玩 */
	if( score["person"] == 3 ) return 1;
	if( !undefinedp(score["p4"]) && 
	    user = present(score["p4"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;
	return 1;
}

/* 检查牌局是否结束 */
int check_over(string name)
{
	int i;
	i=check_player(name);
	switch( i )
	{
		case 1: if( sizeof(p1["m"]) < 1 && sizeof(p1["f"]) < 1 
			&& sizeof(p1["h"]) < 1 && sizeof(p1["t"]) < 1 ) return 1; break;
		case 2: if( sizeof(p2["m"]) < 1 && sizeof(p2["f"]) < 1 
			&& sizeof(p2["h"]) < 1 && sizeof(p2["t"]) < 1 ) return 1; break;
		case 3: if( sizeof(p3["m"]) < 1 && sizeof(p3["f"]) < 1 
			&& sizeof(p3["h"]) < 1 && sizeof(p3["t"]) < 1 ) return 1; break;
		case 4: 
			if( undefinedp(score["p4"]) ) return 0;
			if( sizeof(p4["m"]) < 1 && sizeof(p4["f"]) < 1 
			&& sizeof(p4["h"]) < 1 && sizeof(p4["t"]) < 1 ) return 1; break;
	}
	return 0;
}

void check_board(string winner)
{
	int num=0, x, i;
	string flo;
	object me=this_player();

	message_vision("═══════════════════════════════════\n",me);
	message_vision("                              结      算                              \n",me);
	message_vision("═══════════════════════════════════\n",me);
	i=x=0;
	if( winner != score["p1"] ) {
		message_vision(score["p1"]+"手中所剩下的牌："+show_poker(p1,0),me);
		foreach( flo in keys(p1) ) {
			i+=sizeof(keys(p1[flo]));
			if( !undefinedp(p1[flo][2]) ) {
				message_vision(show_flower(flo)+"２未丢(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("剩下的牌超过十张(x2)。",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p1"]+"总共扣了"+i+"*"+x+"="+i*x+"点分数。\n",me);
		i=i*x;
		board["p1"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( winner != score["p2"] ) {
		message_vision(score["p2"]+"手中所剩下的牌："+show_poker(p2,0),me);
		foreach( flo in keys(p2) ) {
			i+=sizeof(keys(p2[flo]));
			if( !undefinedp(p2[flo][2]) ) {
				message_vision(show_flower(flo)+"２未丢(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("剩下的牌超过十张(x2)。",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p2"]+"总共扣了"+i+"*"+x+"="+i*x+"点分数。\n",me);
		i=i*x;
		board["p2"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( winner != score["p3"] ) {
		message_vision(score["p3"]+"手中所剩下的牌："+show_poker(p3,0),me);
		foreach( flo in keys(p3) ) {
			i+=sizeof(keys(p3[flo]));
			if( !undefinedp(p3[flo][2]) ) {
				message_vision(show_flower(flo)+"２未丢(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("剩下的牌超过十张(x2)。",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p3"]+"总共扣了"+i+"*"+x+"="+i*x+"点分数。\n",me);
		i=i*x;
		board["p3"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( !undefinedp(score["p4"]) && winner != score["p4"] ) {
		message_vision(score["p4"]+"手中所剩下的牌："+show_poker(p4,0),me);
		foreach( flo in keys(p4) ) {
			i+=sizeof(keys(p4[flo]));
			if( !undefinedp(p4[flo][2]) ) {
				message_vision(show_flower(flo)+"２未丢(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("剩下的牌超过十张(x2)。",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p4"]+"总共扣了"+i+"*"+x+"="+i*x+"点分数。\n",me);
		i=i*x;
		board["p4"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	if( score["p1"] == winner ) {
		message_vision(HIY+score["p1"]+"赢了这场，共得"+num+"分。\n"NOR,me);
		board["p1"]+=num;
	} else if( score["p2"] == winner ) {
		message_vision(HIY+score["p2"]+"赢了这场，共得"+num+"分。\n"NOR,me);
		board["p2"]+=num;
	} else if( score["p3"] == winner ) {
		message_vision(HIY+score["p3"]+"赢了这场，共得"+num+"分。\n"NOR,me);
		board["p3"]+=num;
	} else if( !undefinedp(score["p4"]) && score["p4"] == winner ) {
		message_vision(HIY+score["p4"]+"赢了这场，共得"+num+"分。\n"NOR,me);
		board["p4"]+=num;
	}
	message_vision("═══════════════════════════════════\n",me);
	message_vision(show_score()+"\n",me);
	message_vision("═══════════════════════════════════\n",me);

	return;
}

string long()
{
	string msg="";
		
	msg += "╔══════╗\n";
	msg += "║ 大老二牌桌 ║\n";
	msg += "╠══════╩═════════╦═══════════════╗\n";
	msg += "║                                ║                              ║\n";
	msg += "║     rules     观看规则         ║                              ║\n";
	msg += "║     leave     离开牌桌         ║                              ║\n";
	msg += sprintf("║     sit	 找座位坐         ║%13s%-17s║\n","",
			!undefinedp(score["p1"]) ? score["p1"] : "" );

	msg += "║     reset	 设置新局、洗牌   ║            ╔═╗            ║\n";

	msg += sprintf("║     start	 开始牌局         ║%12s║桌║%-12s║\n",
			!undefinedp(score["p4"]) ? score["p4"] : "",
			!undefinedp(score["p2"]) ? score["p2"] : "" );

	msg += "║     .         观看手中的牌     ║            ╚═╝            ║\n";

	msg += sprintf("║     du	 丢牌             ║%13s%-17s║\n","",
			!undefinedp(score["p3"]) ? score["p3"] : "" );

	msg += "║     pa        出牌权让给下家   ║                              ║\n";
	msg += "║     ch        检视目前是什么牌 ║                              ║\n";
	msg += "║     ,         检视目前分数	  ║                              ║\n";
	msg += "║                                ║                              ║\n";
	msg += "╠════════════════╩═══════════════╣\n";
	msg += "║     黑桃是t, 红心是h, 方块是f, 梅花是m                         ║\n";
	msg += "║     老Ｋ是13, Ｑ是12, Ｊ是11, Ａ是1                            ║\n";
	msg += "║     du时为du 10t 3h, 丢多张牌时以空白隔开                      ║\n";
	msg += "║     不可以K,Q,J,A方式丢，必须是数字才行                        ║\n";
	msg += "╚════════════════════════════════╝\n";

	return msg;
}