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

#ifndef GFORT_GAMES_SHMUP_WEAPON_H_
#define GFORT_GAMES_SHMUP_WEAPON_H_

#include <Cistron/Cistron.h>
#include <GFort/Core/Game/IWeapon.h>

namespace GFort { namespace Games { namespace Shmup 
{

/// Base Weapon class
class Weapon 
    : public Cistron::Component
    , public GFort::Core::Game::IWeapon
{
public:
    Weapon() : Cistron::Component("Weapon") {}

    virtual void StartAttack(const double& currentTime, bool instantAttack);
    virtual void StopAttack(const double& currentTime);
    virtual void Update(const double& currentTime);

    /// Retruns true if it is attacking
    virtual bool IsAttacking() const { return is_attacking_; }

    virtual void DoAttack()   {};
    virtual void Reset()      {};

protected:
    // Stores whether it is attacking and last attack time
    bool        is_attacking_;
    double      last_attack_time_;
    
    // Stores attack rate
    float       num_shoots_per_second_;
};

inline void Weapon::StartAttack(const double& currentTime, bool instantAttack)
{
    if (!is_attacking_)
    {
        is_attacking_ = true;        
        if (instantAttack && 
            (currentTime - last_attack_time_) >= num_shoots_per_second_)
        {
            DoAttack();
        }
        last_attack_time_ = currentTime;
    }
}

inline void Weapon::StopAttack(const double& currentTime)
{
    is_attacking_ = false;
    last_attack_time_ = currentTime;
}

inline void Weapon::Update(const double& currentTime)
{
    if (is_attacking_ && 
        (currentTime - last_attack_time_) >= num_shoots_per_second_)
    {
        DoAttack();
        last_attack_time_ = currentTime;
    }
}

} } } // namespace

#endif // GFORT_GAMES_SHMUP_WEAPON_H_
