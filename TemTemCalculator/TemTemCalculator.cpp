#include <iostream>
#include <vector>
#include <string>

struct Stats
{
    Stats() 
        : hitpoints(0)
        , stamina(0)
        , speed(0)
        , attack(0)
        , defence(0)
        , specialAttack(0)
        , specialDefence(0)
    {}

    Stats(int inValueForAll)
        : hitpoints(inValueForAll)
        , stamina(inValueForAll)
        , speed(inValueForAll)
        , attack(inValueForAll)
        , defence(inValueForAll)
        , specialAttack(inValueForAll)
        , specialDefence(inValueForAll)
    {}

    Stats(int inHitpoints, int inStamina, int inSpeed, int inAttack, int inDefence, int inSpecialAttack, int inSpecialDefence)
        : hitpoints(inHitpoints)
        , stamina(inStamina)
        , speed(inSpeed)
        , attack(inAttack)
        , defence(inDefence)
        , specialAttack(inSpecialAttack)
        , specialDefence(inSpecialDefence)
    {}

    std::string createString()
    {
        std::string retString("");

        retString += "HitPoints:" + std::to_string(hitpoints);
        retString += " Stamina:" + std::to_string(stamina);
        retString += " Speed:" + std::to_string(speed);
        retString += " Attack:" + std::to_string(attack);
        retString += " Defence:" + std::to_string(defence);
        retString += " SpAttack:" + std::to_string(specialAttack);
        retString += " SpDef:" + std::to_string(specialDefence);

        return retString;
    }

    int hitpoints;
    int stamina;
    int speed;
    int attack;
    int defence;
    int specialAttack;
    int specialDefence;
};

int calculateHitpoints(int baseStat, int tv, int sv = 50, int level = 100)
{
    const float part1 = (((1.5f * (float)baseStat) + (float)sv / 1.f + (float)tv / 5.f) * (float)level) / 80.f;
    const float part2 = ((float)sv * (float)baseStat * (float)level) / 20000.f;
    const float part3 = (float)level;
    const float part4 = 15.f;

    return (int)std::floorf(part1 + part2 + part3 + part4);
}

int calculateStamina(int baseStat, int tv, int sv = 50, int level = 100)
{
    const float part1 = (float)baseStat / 4;
    const float part2 = std::powf((float)level, 0.35f) * 6.f;
    const float part3 = ((float)sv * (float)level * (float)baseStat) / 20000.f;
    const float part4 = ((float)tv * (float)level * (float)baseStat) / 30000.f;

    return (int)std::floorf(part1 + part2 + part3 + part4);
}

int calculateNormalStat(int baseStat, int tv, int sv = 50, int level = 100)
{
    const float part1 = (((1.5f * (float)baseStat) + (float)sv + (0.2f *(float)tv)) * (float)level) / 100.f;
    const float part2 = (((float)sv * (float)baseStat * (float)level) / 25000.f);
    const float part3 = 10.f;

    return (int)std::floorf(part1 + part2 + part3);
}

Stats calculateStats(Stats baseStats, Stats tvs, Stats svs = Stats(50))
{
    return Stats(
        calculateHitpoints(baseStats.hitpoints, tvs.hitpoints, svs.hitpoints)
        , calculateStamina(baseStats.stamina, tvs.stamina, svs.stamina)
        , calculateNormalStat(baseStats.speed, tvs.speed, svs.speed)
        , calculateNormalStat(baseStats.attack, tvs.attack, svs.attack)
        , calculateNormalStat(baseStats.defence, tvs.defence, svs.defence)
        , calculateNormalStat(baseStats.specialAttack, tvs.specialAttack, svs.specialAttack)
        , calculateNormalStat(baseStats.specialDefence, tvs.specialDefence, svs.specialDefence)
    );
}

float calculateTankinessMetric(Stats temStats)
{
    return (1.f / temStats.defence + 1.f / temStats.specialDefence) / (float)temStats.hitpoints;
}

int main()
{
    // Input values
    Stats baseStats(77, 52, 66, 84, 77, 36, 51);
    Stats svs(50, 50, 50, 50, 50, 50, 50);
    int tvsAvailable(901);

    int step(1);

    float bestTankinessMetric(9999999.f);
    Stats statsAtBestTankiness = Stats();
    Stats tvsAtBestTankiness = Stats();

    for (int hp = 0; hp < std::min(tvsAvailable, 500); hp += step)
    {
        int tvsAvailableAfterHp = tvsAvailable - hp;
        for (int defence = 0; defence < tvsAvailableAfterHp; defence += step)
        {
            Stats tvs(hp, 0, 0, 0, defence, 0, tvsAvailableAfterHp - defence);
            Stats temStats(calculateStats(baseStats, tvs, svs));

            float tankinessMetric = calculateTankinessMetric(temStats);
            if (tankinessMetric < bestTankinessMetric)
            {
                bestTankinessMetric = tankinessMetric;
                statsAtBestTankiness = temStats;
                tvsAtBestTankiness = tvs;
            }
        }
    }

    std::cout << "Best tankiness metric: " << bestTankinessMetric << "\nstats: " << statsAtBestTankiness.createString() << "\nTVs: " << tvsAtBestTankiness.createString() << "\n";
}
