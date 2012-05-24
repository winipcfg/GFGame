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

#ifndef WARRIOR_UNIT_PROPERTIES_H_
#define WARRIOR_UNIT_PROPERTIES_H_

#include <Cistron/Cistron.h>
//#include <Warrior/Model/Struct.h>
#include <Warrior/TypeDef.h>

namespace Warrior 
{
    
/// Class of basic unit
class UnitProperties : public Cistron::Component
{
public:
    /// Constructor.
    UnitProperties() 
        : Cistron::Component("Properties")
        , max_lives_(0)
        , max_stamina_(0)
        , walk_speed_(200)
        , run_speed_(400)
    {}
   
    /// Gets maximum health point of the unit.
    const short MaxLives() const                            { return max_lives_; }

    /// Gets maximum stamina of the unit.
    const short MaxStamina() const                          { return max_stamina_; }

    /// Gets the walk speed of the unit.
    const float WalkSpeed() const                           { return walk_speed_; }

    /// Gets the run speed of the unit.
    const float RunSpeed() const                            { return run_speed_; }

    /// Sets maximum lives of the unit.
    /// @param value
    virtual void SetMaxLives(const short& value)            { max_lives_ = value; }

    /// Sets walk speed of the unit.
    /// @param value
    virtual void SetWalkSpeed(const float& value)           { walk_speed_ = value; }

    /// Sets run speed of the unit.
    /// @param value
    virtual void SetRunSpeed(const float& value)            { run_speed_ = value; }
    
protected:
    // Attributes
    short                       max_lives_;
    short                       max_stamina_;
    float                       walk_speed_;
    float                       run_speed_;
};    
    
} // namespace

#endif // WARRIOR_UNIT_PROPERTIES_H_
