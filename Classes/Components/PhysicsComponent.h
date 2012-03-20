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

#ifndef GFGAME_COMPONENTS_PHYSICS_COMPONENT_H_
#define GFGAME_COMPONENTS_PHYSICS_COMPONENT_H_

#include <string>
#include <hash_map>
#include <Cistron/Cistron.h>
#include <Box2D/Box2D.h>
#include <Warrior/TypeDef.h>

namespace GFGame { namespace Components 
{

/// Stores missile information
class PhysicsComponent : public Cistron::Component
{
public:
    /// Constructor.
    PhysicsComponent();

    /// Destructor.
    ~PhysicsComponent();

    /// Gets the physics body.
    b2Body* Body()                                          { return bodies_["root"]; }

    /// Assign physics body to the unit.
    /// @param body
    void AddBody(std::string name, b2Body* body)            { bodies_[name] = body; }

    /// Gets the bounding region.
    BPolygon GetBoundingRegion() const;
                
private:    
    std::hash_map<std::string, b2Body*>                     bodies_;
};  

    
inline PhysicsComponent::PhysicsComponent()
    : Cistron::Component("PhysicsComponent")
{
}

inline PhysicsComponent::~PhysicsComponent()
{
}
    
} } // namespace

#endif // GFGAME_COMPONENTS_PHYSICS_COMPONENT_H_
