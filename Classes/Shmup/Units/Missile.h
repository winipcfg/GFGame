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

#pragma once
#ifndef SHMUP_MISSILE_H_
#define SHMUP_MISSILE_H_

#include <Cistron/Cistron.h>
//#include "../Constants.h"

namespace GFort { namespace Games { namespace Shmup 
{

/// Stores missile information
class Missile : public Cistron::Component
{
public:
    /// Constructor.
    Missile();

    /// Destructor.
    ~Missile();

    /// Sets the damage of the missile.
    /// @param damage
    void SetDamage(const short& damage) { damage_ = damage; }

    /// Gets the damage of the missile.
    short GetDamage()                   { return damage_; }
                
private:    
    short       damage_;

    // If true, then the node follows physics motion. Else, it follows sprite motion
    bool        use_physics_;  

    short       life_time_;
};  

    
inline Missile::Missile()
    : Cistron::Component("Missile"),
      damage_(0),
      use_physics_(false)
{
}

inline Missile::~Missile()
{
}
    
} } } // namespace

#endif // SHMUP_MISSILE_H_
