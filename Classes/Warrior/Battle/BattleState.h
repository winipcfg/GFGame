//Copyright (C) 2012 by Gavin Fong
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#ifndef GFGAME_WARRIOR_BATTLESTATE_H_
#define GFGAME_WARRIOR_BATTLESTATE_H_

#include <Cistron/Cistron.h>

namespace Warrior 
{

struct BattleState 
{
    /// Constructor.
    BattleState();

    /// Whether it is Survival Mode.
    bool SurvivalMode;

    /// Remain HP of Player (Survival Mode).
    int HP;

    /// Total number of battles.
    int TotalBattles;

    /// Total number of kills in all battles.
    long TotalKills;

    /// Total number of headshots.
    long Headshots;

    /// Total number of death in all battles.
    int TotalDeaths;

    /// Total number of slash in all battles.
    int TotalSlashCount;

    /// Maximum combos in all battle.
    int MaximumCombos;

    /// Total number of item used.
    int NumItemsUsed;
};

inline BattleState::BattleState()
    : SurvivalMode(false)
    , HP(0)
    , TotalBattles(0)
    , TotalKills(0)
    , Headshots(0)
    , TotalDeaths(0)
    , TotalSlashCount(0)
    , MaximumCombos(0)
    , NumItemsUsed(0)
{
}

} // namespace

#endif // GFGAME_WARRIOR_BATTLESTATE_H_
