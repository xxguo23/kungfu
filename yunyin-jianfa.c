#include <ansi.h>

inherit SKILL;

string *action_msg = ({
    "$N����΢�Σ�����$wһ����һ�С��ƾ�ǧ�塹��������������������������$n��$l",
    "$N$wбָ��񷣬ʹ��һʽ��������¥����������������������ʵ�ѱ棬��Ȼ����$n��$l",
    "$N����$w�����һ�С��������¡��������������֣��������棬ٿ�ش���$n��$l",
    "$N���潣�ߣ�һʽ�����֪������$w����һ���׺磬Ѹ�����׵ش���$n��$l",
    "$N����һת��ʹ������ɢ���ࡹ������$w���ƿ���ɢ������������ֱȡ$n��$l",
    "$N��Хһ����һ�С�������ã����$w�趯�佣���ݺᣬ�����ƺ����ڣ�����$n����",
    "$N�Ų����ƣ�һʽ����Ӱ���١���$w����Ӱ�ӵأ�Ʈ�������ش���$n��$l",
    "$N$w�߾٣�һ�С�������ơ������������Ʒ��٣����϶�������$n��$l",
});

string *parry_msg = ({
    "$n����һ�࣬����$w����һ����������������ת����$N�Ĺ����������⡣\n",
    "$n$w΢̧��һ�����赭д���������������ڣ�����$N�Ĺ��ơ�\n",
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
        return notify_fail("���������Ϊ���㣬�޷�ѧϰ����������\n");
    if ((int)me->query_skill("force", 1) < 60)
        return notify_fail("��Ļ����ڹ����̫ǳ���޷���������������\n");
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
            "action": HIW "$N����һ�񣬽������������ڣ�һʽ��������⡹��\n"
                      "���������ӵ��ң�����������ƥ�����ƴ���$n��$l��" NOR,
            "dodge": 120,
            "parry": 100,
            "attack": 220,
            "damage": 250,
            "damage_type": "����"
        ]);
    }

    return ([
        "action": action_msg[random(sizeof(action_msg))],
        "damage": 140 + random(60),
        "attack": 80 + random(20),
        "dodge": 90 + random(20),
        "parry": 80 + random(10),
        "damage_type": random(2) ? "����" : "����",
    ]);
}

int practice_skill(object me)
{
    object weapon;

    if (!objectp(weapon = me->query_temp("weapon")))
        return notify_fail("�����޷���ϰ����������\n");
    if ((string)weapon->query("skill_type") != "sword")
        return notify_fail("��ʹ�õ��������ԣ��޷���ϰ����������\n");
    if ((int)me->query("qi") < 90)
        return notify_fail("����������㣬�޷���ϰ����������\n");
    if ((int)me->query("neili") < 90)
        return notify_fail("����������㣬�޷���ϰ����������\n");

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
        result += ([ "msg": HIW "$n��$N�����е�����֮����������һ�ͣ�¶��������\n" NOR ]);
    else
        result += ([ "msg": CYN "$nֻ��$N����������Ѫ��ӿ�������мܣ�\n" NOR ]);

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