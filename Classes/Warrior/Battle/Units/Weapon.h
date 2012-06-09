#ifndef WARRIOR_WEAPON_H_
#define WARRIOR_WEAPON_H_

#include <GFort/Core/Game/IWeapon.h>
#include <Cistron/Cistron.h>

namespace Warrior 
{

/// Base Weapon class
class Weapon 
    : public Cistron::Component, 
      public GFort::Core::Game::IWeapon
{
public:
    /// Constructor.
    Weapon() : Cistron::Component("Weapon") {}

    /// Destructor.
    ~Weapon() {}

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

} // namespace

#endif // WARRIOR_WEAPON_H_
