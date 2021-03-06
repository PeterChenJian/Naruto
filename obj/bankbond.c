// bankbond.c

inherit ITEM;

int balance = 0;

void create()
{
	set_name("钱庄金契", ({ "bankbond" }));
	set_weight(1);
	if( !clonep() ) {
		set("unit", "张");
		set("long",
			"这是一张钱庄用来记载跟客户金钱往来的金契﹐上面有双方的花押跟\n"
			"结余。\n");
        set("no_sell", 1);
	        set("value", 1);
	}
	set("keep_this", 1);
	setup();
}

string query_autoload() { return balance + ""; }

void autoload(string param)
{
	if( sscanf(param, "%d", balance) )
		set("owner_id", geteuid(previous_object()));
}

void transact(int amount)
{
	if( environment()->query("bank_account") != balance ) return;

	balance = environment()->query("bank_account") + amount;
	environment()->set("bank_account", balance);
}

void set_balance(int b) { balance = b; environment()->set("bank_account", b); }
int query_balance() { return balance; }

string long(int raw)
{
	string str;
	int amount;

	str = ::long(raw);
	if( balance > 0 ) {
		str += "你现总共结余﹕";
		amount = balance;
	} else if( balance < 0 ) {
		str += "你现在还赊欠钱庄﹕";
		amount = - balance;
	}
	if( balance != 0 ) {
		if( amount >= 100 ) {
			str += chinese_number(amount/100) + "两";
			if( amount%100 ) str += "又";
		}
		if( amount%100!=0 )
			str += chinese_number(amount%100) + "文钱。";
		str += "\n";
	}

	return str;	
}
