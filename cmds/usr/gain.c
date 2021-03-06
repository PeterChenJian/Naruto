/*  gain.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string term, txt;
    int g;
    mapping score_g, skill_g;

    score_g = me->query_score_gain();
    me->reset_score_gain();
    skill_g = me->query_skill_gain();
    me->reset_skill_gain();

    txt = "从你上次使用 gain 指令，到现在为止，你共获得：\n\n";
    
    txt += "经验点数：\n";
    if( !sizeof(score_g) )
	txt += "    无。\n";
    else {
	foreach(term, g in score_g)
	    txt += sprintf("    %-16s %+d 点\n",
//		to_chinese("score of " + term), score_g[term] );
		to_chinese(term), score_g[term] );
    }

    txt += "\n技能点数：\n";
    if( !sizeof(skill_g) )
	txt += "    无。\n";
    else {
	foreach(term, g in skill_g)
	    txt += sprintf("    %-16s %+d 点\n",
		to_chinese(term), skill_g[term] );
    }

    write(txt);
    return 1;    
}

int help()
{
    write(@TEXT
指令格式：gain

这个指令可以用来检验你的人物成长，每次你的人物获得任何经验点数或技能点数，系
统会记录所获得的值，当你使用 gain 指令，会显示这些值，然后清除这些纪录从头开
始，你可以在战斗开始前先用 gain 清除，然后在战斗结束后用 gain 检视你的人物从
刚刚的战斗中获的多少进步。
TEXT
    );
    return 1;
}
