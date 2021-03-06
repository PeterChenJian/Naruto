/*  attribute.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef	ATTRIBUTE_H
#define	ATTRIBUTE_H

/* 定义一个人物“属性”值的个数 */
#define	NUM_ATTRIBUTES	4

/* 属性值的最大及最小值 */
#define	ATTRVAL_MIN	10
#define ATTRVAL_MAX	120

varargs void init_attribute(mapping base);
int set_attr(string what, int value);
varargs int query_attr(string what, int raw);


#endif	/* ATTRIBUTE_H */
