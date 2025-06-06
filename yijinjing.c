inherit FORCE;

int valid_enable(string usage)
{
        return usage == "force";
}

int valid_force(string force)
{
	object me = this_player();
	//2转后不再内功冲突 mige 2025.03.08
    if (me && me->query("reborn/times") > 1) 
        return 1;
        return force == "hunyuan-yiqi" ||
               force == "zhanshen-xinjing" ||
               force == "taiji-shengong" ||
               force == "taixuan-gong" ||
               force == "wudang-xinfa" ||
			   force == "honghua-xinfa" ||
			   force == "tianchi-neigong" ||
               force == "shaolin-xinfa";
}

int query_neili_improve(object me)
{
        int lvl;
        lvl = (int)me->query_skill("yijinjing", 1);

        if (me->query_skill("lunhui-jian", 1) > 200)
                lvl += me->query_skill("lunhui-jian", 1) / 2;

        return lvl * lvl * 15 * 21 / 100 / 200;
}

int query_jingli_improve(object me)
{
        int lvl;
        lvl = (int)me->query_skill("buddhism", 1);
        lvl += (int)me->query_skill("lunhui-jian", 1);
        if (me->query_skill("lunhui-jian", 1) > 200)
                return lvl * 9 / 5;
        else
                return 0;
}

int valid_learn(object me)
{
        if (me->query_skill("force", 1) < 100)
                return notify_fail("你的基本内功火候不够，无法学习易筋经神功！\n");

        //if (me->query_skill("freezing-force", 1))
        //        return notify_fail("你已经将易筋经神功和冰蚕寒毒合炼成了一门新内功了。\n");

        if (me->query_skill("force", 1) < me->query_skill("yijinjing", 1))
                return notify_fail("你的基本内功水平不够，难以领悟更高深的易筋经神功！\n");

        if (me->query("gender") != "男性")
                return notify_fail("你不是男性，无法练习易筋经神功。\n");

        return ::valid_learn(me);
}


int practice_skill(object me)
{
        return notify_fail("易筋经神功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
        return __DIR__"yijinjing/" + func;
}
