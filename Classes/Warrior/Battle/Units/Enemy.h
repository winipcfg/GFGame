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

#ifndef WARRIOR_ENEMY_H_
#define WARRIOR_ENEMY_H_

#include "Unit.h"

namespace Warrior 
{

/// Stores enemy information
class Enemy : public Unit
{
public:
    /// Constructor.
    Enemy();

    /// Destructor.
    ~Enemy();

    /// Reset all parameters.
    virtual void Reset();
    
    /// Gets whether the unit can perform attack.
    virtual const bool CanPerformAttack() const;
                
private:    
};  

    
inline Enemy::Enemy()
{
}

inline Enemy::~Enemy()
{
}

inline void Enemy::Reset()
{
    lives_ = 1;
}

inline const bool Enemy::CanPerformAttack() const
{
    if (1)
        return true;
    return false;
}   
    
} // namespace

#endif // WARRIOR_ENEMY_H_
