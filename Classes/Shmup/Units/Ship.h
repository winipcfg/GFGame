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

#ifndef GFORT_GAMES_SHMUP_SHIP_H_
#define GFORT_GAMES_SHMUP_SHIP_H_

#include <vector>
#include <Cistron/Cistron.h>
#include <GFort/Core/Game/IEntity.h>
#include <GFort/Core/Game/IWeapon.h>

namespace GFort { namespace Games { namespace Shmup 
{

/// The entities that are controlled by Players
/// The term "Ship" is a generic term
class Ship 
    : public Cistron::Component
    , public GFort::Core::Game::IEntity
{
public:
    typedef GFort::Core::Game::IWeapon& WeaponReference;
    typedef GFort::Core::Game::IWeapon* WeaponPtr;
    typedef std::vector<WeaponPtr >     WeaponList;

public:
    /// Constructor
    Ship();

    /// Destructor
    ~Ship()                             {}

    /// Add new weapon to the object.
    /// @param weapon
    void AddWeapon(WeaponReference weapon);

    /// Remove a weapon from the object
    /// @param index
    void RemoveWeapon(const short& index);

    /// Set weapon.
    /// @param index The index of the weapon
    void SetWeapon(const short& index);

    /// Remove all weapons.
    void RemoveAllWeapons();

    /// Do a single attack.
    void DoAttack();

    /// Gets current weapon.
    WeaponPtr CurrentWeapon() const     { return current_weapon_; }

    /// Kill the object.
    void Die();

private:
    // Stores weapons and the weapon currently used
    WeaponList          weapons_;
    short               current_weapon_index_; 
    WeaponPtr           current_weapon_;
};

inline Ship::Ship()
    : Cistron::Component("Ship")
{
    SetWeapon(-1);
}

inline void Ship::AddWeapon(WeaponReference weapon)
{
    weapons_.push_back(&weapon);
    if (current_weapon_index_ == -1)
        current_weapon_index_ = 0;
}

inline void Ship::RemoveWeapon(const short& index)
{
    if (!weapons_.empty() && 
        index >= 0 &&
        static_cast<WeaponList::size_type>(index) < weapons_.size())
    {
        weapons_.erase(weapons_.begin() + index);
        if (current_weapon_index_ >= index)
            SetWeapon(current_weapon_index_ - 1);
    }
}

inline void Ship::SetWeapon(const short& index)
{
    if (!weapons_.empty() && 
        index >= 0 &&
        static_cast<WeaponList::size_type>(index) < weapons_.size())
    {
        current_weapon_index_ = index;
        current_weapon_ = weapons_[current_weapon_index_];
    }
    else
    {
        current_weapon_index_ = -1;
        current_weapon_ = NULL;
    }
}

inline void Ship::RemoveAllWeapons()
{
    weapons_.clear();
    SetWeapon(-1);
}

inline void Ship::DoAttack()
{
    if (current_weapon_)
        current_weapon_->DoAttack();
}

inline void Ship::Die()
{
    RemoveAllWeapons();
}

} } } // namespace

#endif // GFORT_GAMES_SHMUP_SHIP_H_
