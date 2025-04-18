// yeqiuquan.c 野球拳
// by xiner 09/3/2009

#include <ansi.h>
inherit SKILL;

string *msg = ({
"只见$N一跃而起，大喝一声『石头』对准$n呼地砸了过去",
"$N右手食指中指一分，大喝道『剪刀』，两根指头便往$n刺去",
"$N拉开后弓步，手掌一张，大叫道『布』！一掌往$n罩去",
});

string *usage_skills = ({ "unarmed", "finger", "hand", "cuff" });

int valid_enable(string usage)	{return (member_array(usage, usage_skills) != -1);}

//int valid_combine(string combo) { return combo=="sanshou"; }

mapping query_action(object me, object weapon)
{
	int lvl = me->query_skill("yeqiu-quan", 1);

	if (lvl > 500)
	{
		return ([
			"action" : msg[random(sizeof(msg))],
			"attack" : 200+random(lvl),
			"force"  : 200+random(lvl),
			"dodge"  : 200+random(200),
			"parry"  : 200+random(200),
			"damage" : 200+random(lvl),
			"damage_type" : "瘀伤",
			]);
	}else if (lvl > 250)
	{
		return ([
			"action" : msg[random(sizeof(msg))],
			"attack" : 10+random(lvl),
			"dodge"  : 10+random(lvl),
			"parry"  : 0,
			"force"  : 10+random(lvl),
			"damage" : 10+random(lvl),
			"damage_type" : "瘀伤",
			]);
	}else
	{
		return ([
			"action" : msg[random(sizeof(msg))],
			"attack" : 10,
			"dodge"  : 10,
			"parry"  :-10,
			"force"  : 10,
			"damage" : 10,
			"damage_type" : "瘀伤",
			]);
	}
}

int valid_learn(object me)
{
	int layer;
	if( me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练野球拳必须空手。\n");

        layer = (int)me->query_skill("yeqiu-quan", 1) / 50;

        if (layer > 10) layer = 10;

	if (layer * 30 > me->query_skill("literate", 1))
		return notify_fail("你觉得野球拳第" + chinese_number(layer) +
			"层境界实在是深奥无比，看来自己读书写字能力还不够。\n");

        return 1;
}

int difficult_level(object me) {return 500;}

int practice_skill(object me)
{

        int i,level = me->query_skill("yeqiu-quan", 1);

	if(me->query_skill("yeqiu-quan", 1) < 100)
		return notify_fail("你现在的野球拳等级尚不足以自行练习。\n");
	
	if (level<500)
	{
	        for (i = 0; i < sizeof(usage_skills); i++)
        	        if (me->query_skill(usage_skills[i], 1) < level)
                	        return notify_fail("你对" + to_chinese(usage_skills[i]) + "的理解还"
                        	                   "不够，无法提高野球拳的等级。\n");
		if( (int)me->query("jing") < 300)
			return notify_fail("你的精神无法集中了，休息一下再练吧。\n");

		if( (int)me->query("qi") < 300 )
			return notify_fail("你现在太累了，休息一下再练吧。\n");

		if( (int)me->query("neili") < 10 )
			return notify_fail("你的内力不够了。\n");

		me->receive_damage("jing", 250);
		me->receive_damage("qi", 250);
		me->add("neili", -10);
		return 1;
	}else
	{
		if (me->query_skill("finger", 1) < level &&
			me->query_skill("hand", 1) < level &&
			me->query_skill("cuff", 1) < level &&
			me->query_skill("unarmed", 1) < level)
		return notify_fail("你的基本技能等级太低不能提高野球拳的等级。\n");

		if( (int)me->query("jing") < level*2)
			return notify_fail("你的精神无法集中了，休息一下再练吧。\n");

		if( (int)me->query("qi") <level*2)
			return notify_fail("你现在太累了，休息一下再练吧。\n");

		if( (int)me->query("neili") < level*2)
			return notify_fail("你的内力不够了。\n");

		me->receive_damage("jing", level/2);
		me->receive_damage("qi", level/2);
		me->add("neili", -level/2);
		return 1;
	}
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
        if (me->query_skill("yeqiu-quan", 1) < 500) return 0;

	switch(random(3))
	{
	case 1:
	        if (random(me->query_int()) > victim->query_int() / 3)
        	{
			me->start_busy(1);
			victim->start_busy(1 + random(10));
			return HIR "$n" HIR "一时拆解不及，被$N"HIR"击中，只得连连后退。\n" NOR;
	        }
		break;

	case 2:
	        if (random(me->query_int()) > victim->query_int() / 3)
        	{
                	victim->receive_wound("jing", damage_bonus * 2/3, me);
		                return HIR "$n" HIR "完全猜不透$N"HIR"的攻击，头昏眼花疲于应对。\n" NOR;
	        }
		break;

	default :
	        if (random(me->query_int()) > victim->query_int() / 3)
        	{
                	victim->receive_wound("qi", damage_bonus * 2, me);
		                return HIR "$n" HIR "面对$N"HIR"不停变换的招数，完全"
				"无法应对，气得鲜血狂喷。\n" NOR;
	        }
		break;
	}
}

string perform_action_file(string action)
{
	return __DIR__"yeqiu-quan/" + action;
}

void skill_improved(object me) 
{ 
	int lvl;
	int layer;
	lvl = me->query_skill("yeqiu-quan", 1);
        layer = (int)me->query_skill("yeqiu-quan", 1) / 50;
	if (layer > 10) layer = 10;

	if (! layer)
        {
                tell_object(me, HIM "你的野球拳等级提高了。\n" NOR);
	} else
	{
		if ((lvl % 50) == 0)
        	{
			tell_object(me, HIM "你的野球拳已经修炼成到了第" + chinese_number(layer) +
				"层的境界。\n" NOR);
			if (lvl > 250)
				me->set("title",HIG"野球拳大师"NOR);
        	} else
		tell_object(me, HIM "你对第" + chinese_number(layer) +
				"层的野球拳又加深了一层了解。\n" NOR);
	}
}