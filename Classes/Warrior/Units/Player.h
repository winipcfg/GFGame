//Copyright (C) 2011 by Gavin Fong
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

#ifndef WARRIOR_PLAYER_H_
#define WARRIOR_PLAYER_H_

#include "Unit.h"
#include <Warrior/Components/Weapon.h>

namespace Warrior 
{
    
const short kDefaultLives       = 10;
//const short kMaxLives           = 10;

/// Stores player information
class Player : public Unit, public GFort::Core::ISubject<Player> 
{
public:
    /// Constructor.
    Player();

    /// Destructor.
    ~Player();

    /// Reset all parameters.
    virtual void Reset();
        
    /// Gets whether the unit can perform attack.
    virtual const bool CanPerformAttack() const;

    /// Gets whether the unit is under control and able to do action assigned.
    virtual const bool IsUnderControl() const;
            
private:    
    // Stores the weapon information
    Weapon          weapon_;
};  

    
inline Player::Player()
{
}

inline Player::~Player()
{
}

inline void Player::Reset()
{
    lives_ = kDefaultLives;
}

inline const bool Player::CanPerformAttack() const
{
    if (action_.ActionType == kUnitActionTypeIdle ||
        action_.ActionType == kUnitActionTypeWalk ||
        action_.ActionType == kUnitActionTypeRun)
        return true;
    return false;
}    

inline const bool Player::IsUnderControl() const
{
    if (Alive())
        return true;
    return false;
}    
            
} // namespace

#endif // WARRIOR_PLAYER_H_
