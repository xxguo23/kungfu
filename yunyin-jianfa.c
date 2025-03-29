#include <ansi.h>

inherit SKILL;

string *action_msg = ({
    "$N身形微晃，手中$w一抖，一招「云卷千峰」，剑光如云雾翻滚，层层叠叠刺向$n的$l",
    "$N$w斜指苍穹，使出一式「雾锁重楼」，剑势如雾气弥漫，虚实难辨，悄然点向$n的$l",
    "$N手中$w轻颤，一招「隐风逐月」，剑光若隐若现，似清风拂面，倏地刺向$n的$l",
    "$N身随剑走，一式「云深不知处」，$w化作一道白虹，迅捷无伦地刺向$n的$l",
    "$N剑势一转，使出「雾散天青」，手中$w如云开雾散，剑光清冽，直取$n的$l",
    "$N长啸一声，一招「剑隐苍茫」，$w舞动间剑气纵横，宛如云海奔腾，笼罩$n周身",
    "$N脚步轻移，一式「云影无踪」，$w似云影掠地，飘忽不定地刺向$n的$l",
    "$N$w高举，一招「天外飞云」，剑光如流云飞瀑，自上而下劈向$n的$l",
});

string *parry_msg = ({
    "$n身形一侧，手中$w轻轻一拨，剑势如云雾流转，将$N的攻击无声化解。\n",
    "$n$w微抬，一招轻描淡写，剑光如云遮雾掩，挡下$N的攻势。\n",
});

string query_parry_msg(object weapon)
{
    if (!objectp(weapon))
        return 0;
    return parry_msg[random(sizeof(parry_msg))];
}

int valid_enable(string usage) { return usage == "sword" || usage == "parry"; }

int valid_learn(object me)
{
    if ((int)me->query("max_neili") < 150)
        return notify_fail("你的内力修为不足，无法学习云隐剑法。\n");
    if ((int)me->query_skill("force", 1) < 60)
        return notify_fail("你的基本内功火候太浅，无法领悟云隐剑法。\n");
    return 1;
}

mapping query_action(object me, object weapon)
{
    int level = me->query_skill("yunyin-jianfa", 1);

    if (random(level) > 180 &&
        me->query_skill("force", 1) > 120 &&
        me->query("neili") > 300)
    {
        return ([
            "action": HIW "$N长剑一振，剑气如云雾升腾，一式「云隐天光」，\n"
                      "剑光如银河倒挂，带着凌厉无匹的气势刺向$n的$l！" NOR,
            "dodge": 120,
            "parry": 100,
            "attack": 220,
            "damage": 250,
            "damage_type": "刺伤"
        ]);
    }

    return ([
        "action": action_msg[random(sizeof(action_msg))],
        "damage": 140 + random(60),
        "attack": 80 + random(20),
        "dodge": 90 + random(20),
        "parry": 80 + random(10),
        "damage_type": random(2) ? "刺伤" : "割伤",
    ]);
}

int practice_skill(object me)
{
    object weapon;

    if (!objectp(weapon = me->query_temp("weapon")))
        return notify_fail("空手无法练习云隐剑法。\n");
    if ((string)weapon->query("skill_type") != "sword")
        return notify_fail("你使用的武器不对，无法练习云隐剑法。\n");
    if ((int)me->query("qi") < 90)
        return notify_fail("你的体力不足，无法练习云隐剑法。\n");
    if ((int)me->query("neili") < 90)
        return notify_fail("你的内力不足，无法练习云隐剑法。\n");

    me->receive_damage("qi", 80);
    me->add("neili", -80);
    return 1;
}

string perform_action_file(string action)
{
    return __DIR__ "yunyin-jianfa/" + action;
}

mixed hit_ob(object me, object victim, int damage)
{
    mixed result;

    if (damage < 60 || !objectp(me->query_temp("weapon")))
        return 0;

    result = ([ "damage": damage ]);

    if (random(2) == 1)
        result += ([ "msg": HIW "$n被$N剑势中的云雾之意所惑，身形一滞，露出破绽！\n" NOR ]);
    else
        result += ([ "msg": CYN "$n只觉$N剑风如雾，气血翻涌，难以招架！\n" NOR ]);

    return result;
}

int query_effect_parry(object attacker, object me)
{
    int lvl;

    if (!objectp(me->query_temp("weapon")))
        return 0;

    lvl = me->query_skill("yunyin-jianfa", 1);
    if (lvl < 80)  return 0;
    if (lvl < 200) return 60;
    if (lvl < 280) return 90;
    if (lvl < 350) return 110;
    return 130;
}