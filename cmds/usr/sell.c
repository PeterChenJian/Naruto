/* sell.c by -Acme-
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int sell_item(object item)
{
	int value;
    string name;

    if( item->query("money_id") )
        return notify_fail("你要卖“钱”﹖\n");

    if( item->query("no_sell") )
        return notify_fail("这样东西不能卖。\n");

    if( !(value = item->query("value")) )
        return notify_fail("这样东西不值钱。\n");

    if( item->query("keep_this") )
    	return notify_fail("这样东西你已经 keep 无法贩卖。\n");
    
    if( item->query("equipped") )
        return notify_fail("这样东西你已经装备著，无法贩卖。\n");

    value /= 2;

    name = ( item->query_amount() ? chinese_number(item->query_amount()) : "一" );
    name += ( item->query("base_unit") ? item->query("base_unit") : 
            ( item->query("unit") ? item->query("unit") : "个" ) );
    name += item->query("name");

    if( !environment(this_player())->to_stock(item) )
    	return notify_fail("当铺似乎不愿收下这样物品。\n");

    message_vision("$N以" + chinese_number(value) + "两银子的价钱将" + name + "卖出去。\n", this_player() );
    
    // 直接将钱加到银行里
    this_player()->add("bank", value);

    return 1;
}

int main(object me, string arg)
{
	int amount;
	object item;

	if( !environment(me) )
	    return notify_fail("你现在身处在哪你都不知道了！\n");
	if( !environment(me)->is_hockshop() )
	    return notify_fail("你只能在当铺卖东西。\n");
    if( me->is_busy() )
        return notify_fail("你现在没有空！\n");
    if( !arg )
    	return notify_fail("你想卖什么东西？\n");
    
    if( arg == "all" ) {
        object *inv;
        
        inv = all_inventory(this_player());
        foreach(item in inv) {
            if( !item ) continue;
            sell_item(item);
        }
        write("Ok。\n");
        return 1;
    }

    if( sscanf(arg, "%d %s", amount, arg) == 2 ) {
    	object new_item;
    	if( !(item = present(arg, me)) )
    		return notify_fail("你想要卖断什么物品？\n");
    	if( amount <= 0 ) 
    		return notify_fail("你想要卖几个物品？\n");
    	if( item->query_amount() < amount )
    		return notify_fail("你身上没有这么多的" + item->query("name") + "。\n");
    	
    	// 相同数量
    	if( amount == item->query_amount() )
    		return sell_item(item);
    	
    	if( !objectp(new_item = new(base_name(item))) )
    		return notify_fail("你似乎无法贩卖这样物品。\n");
    	
    	new_item->set_amount(amount);
    	item->add_amount(-amount);
    	
    	// 无法卖出，则dest new_item, 并将原本的item amount恢复
    	if( !sell_item(new_item) ) {
    		destruct(new_item);
    		item->add_amount(amount);
    	}
    	
    	return 1;
    }

    if( !(item = present(arg, me)) )
        return notify_fail("你想要卖断什么物品？\n");

    return sell_item(item);
}

int help(object me)
{
  write(@HELP
指令格式 : sell [数量] 物品

这可指令可以将你身上的物品卖出去，通常只有在当铺的地方才能够
贩卖物品。例：

sell blade     将身上的第一把刀卖出去
sell blade 2   将身上的第二把刀卖出去
sell 3 pill    将身上的pill卖出三个，通常用于“复合物品”上
sell 4 pill 2  将身上第二堆的pill卖出四个

相关指令： list, buy
HELP);
    return 1;
}