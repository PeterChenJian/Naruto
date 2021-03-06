/* area.c write by -Acme-
*/

#pragma optimize
#pragma save_binary

// 比较二个物件是否处在相同的区域座标中
int area_environment(object ob1, object ob2)
{
    if( environment(ob1) != environment(ob2) ) return 0;
    else {
        mapping info1, info2;
        if( !(info1 = ob1->query("area_info")) ) return 0;
        if( !(info2 = ob2->query("area_info")) ) return 0;
        if( info1["x_axis"] != info2["x_axis"] || info1["y_axis"] != info2["y_axis"] ) return 0;
    }
    return 1;
}

int area_move(object area, object who, int x, int y)
{
    mapping info;
    int env_area = 0;

    if( !objectp(who) || !objectp(area) ) return 0;

    if( environment(who) && environment(who)->is_area() ) {
        info = who->query("area_info");
        env_area = 1;
    }

    // 如果who move_in到新的area中失败
    if( !area->move_in(x, y, who) ) return 0;

    who->set("area_info/x_axis", x);
    who->set("area_info/y_axis", y);
    // 成功移入area, move() 会自动从area里移出
    if( who->move(area) ) {
        who->set("area_info/x_axis_old", x);
        who->set("area_info/y_axis_old", y);
        return 1;
    }
    // 移入area失败，如果原本是在area环境中，必须再移入一次
    else {
        // 将先前移到新的area再做移出
        area->move_out(x, y, who);
        if( env_area ) {
            // 设回先前的 x, y 
            who->set("area_info/x_axis", info["x_axis_old"]);
            who->set("area_info/y_axis", info["y_axis_old"]);
            if( !environment(who)->move_in(info["x_axis_old"], info["y_axis_old"]) ) {
                tell_object(who, "因为某种原因，你的角色在区域移动时产生了错误...\n");
                destruct(who);
                return 0;
            }
        }
    }
    return 0;
}

// 覆写mudos present函式
object present(mixed arg, object ob)
{
    if( ob->is_area() ) {
        int p=0, index;
        object *obs, t;
        mapping area_info;

        area_info = this_player()->query("area_info");
    
        if( sscanf(arg, "%s %d", arg, index) != 2 ) index = 1;

        obs = ob->query_inventory(area_info["x_axis"], area_info["y_axis"]);

        foreach(t in obs) {
            if( !objectp(t) ) continue;
            if( t->id(arg) ) {
                p += 1;
                if( p == index ) return t;
            }
        }
        return 0;
    } else return efun::present(arg, ob);
}
