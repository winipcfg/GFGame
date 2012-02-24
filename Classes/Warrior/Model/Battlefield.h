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

#ifndef WARRIOR_BATTLEFIELD_H_
#define WARRIOR_BATTLEFIELD_H_

#include <vector>
#include <GFort/Core/Physics/PhysicsController.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <Cistron/Cistron.h>
#include <Warrior/Model/Struct.h>
#include <Warrior/Units/Player.h>
#include <Warrior/Units/Enemy.h>
#include "Trail.h"
#include "GameStat.h"

namespace Warrior 
{

/// Map of battle. All attributes are in public to make sure it is fast access.
class Battlefield
{
public :
    typedef std::vector<b2Body* > BodyRefList;
public:
    /// Constructor.
    Battlefield();

    /// Destructor.
    ~Battlefield();

    /// Initialize a map.
    void Initialize();

    /// Gets the contents of the map.
    BodyRefList& Contents()                 { return contents_; }

    /// Sets the width of the map.
    /// @param value
    void SetWidth(const float& value)       { width_ = value; }

    /// Sets the height of the map.
    /// @param value
    void SetHeight(const float& value)      { height_ = value; }

    /// Sets the landscape of the map.
    /// @param value
    void SetLandscape(const float& value)   { landscape_ = value; }

    /// Sets the gravity of the map.
    /// @param value
    void SetGravity(const float& value)     { gravity_ = value; }
    

    /// Gets the width of the map.
    float Width() const                     { return width_; }

    /// Gets the height of the map.
    float Height() const                    { return height_; }

    /// Gets the landscape of the map.
    float Landscape() const                 { return landscape_; }

    /// Gets the gravity of the map.
    float Gravity() const                   { return gravity_; }
    
private:
    // Stores the reference of contents
    BodyRefList         contents_;

    float               width_;
    float               height_;
    float               landscape_;
    float               gravity_;
};

}

#endif // WARRIOR_BATTLEFIELD_H_
