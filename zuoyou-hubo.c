// zuoyou-hubo.c 左右互博
// by Doing Lu

inherit SKILL;

#include <ansi.h>

#define LEVEL_PER_LAYER         30
#define MAX_LAYER               10


string type() { return "knowledge"; }


int valid_learn(object me)
{
        int layer;
        int lvl;
	//2转放宽到悟性24以下 by mige 2025.2.26
        lvl = me->query_skill("zuoyou-hubo", 1);
        if (me->query("gender") == "无性" && lvl >= 50)
                return notify_fail("你默默凝神，试图分心二"
                                   "用，可是却觉得阴阳失调，心浮气躁。\n");
		if (me->query("int") > 24)
				return notify_fail("你觉得内心无比烦躁，难以静"
                                   "下心来尝试更高深的分心之术。\n");
		if (me->query("int") > 14 && me->query("reborn/times")<2)
                return notify_fail("你觉得心烦意乱，难以尝"
                                   "试更高深的分心之术。\n");
       	if ((int)me->query_skill("force") < lvl * 3 / 2 && ! me->query("reborn"))
               	return notify_fail("你的内功火候不够，难以轻松自如的分运两股内力。\n");
	//转世ID学习不受等级限制by xiner 2009/04/09

        return 1;
}

int practice_skill(object me)
{
        return notify_fail("左右互博只能通过学习不断提高。\n");
}

void skill_improved(object me)
{
        int lvl;
        int layer;

        lvl = me->query_skill("zuoyou-hubo", 1);
        layer = lvl / LEVEL_PER_LAYER;
        if (layer > MAX_LAYER) layer = MAX_LAYER;

        if (! layer)
        {
                tell_object(me, HIM "你潜心领悟左右互"
                                "博，若有所悟。\n" NOR);
        } else
        if ((lvl % LEVEL_PER_LAYER) == 0)
        {
                tell_object(me, HIM "你领悟了第" + chinese_number(layer) +
                            "层境界的左右互博的奥妙。\n" NOR);
        } else
                tell_object(me, HIM "你对第" + chinese_number(layer) +
                            "层境界的左右互博又加深了一点了解。\n" NOR);
}
