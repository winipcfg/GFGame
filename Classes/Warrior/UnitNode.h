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

#ifndef WARRIOR_UNIT_NODE_H_
#define WARRIOR_UNIT_NODE_H_

#include <string>
#include <deque>
#include <hash_map>
#include <cocos2d.h>
#include <cocos2dx/include/CCProtocols.h>
//#include <cocos2dx/extensions/AdvanceSpriteAndParticle/AdvanceSprite.h>
#include <Cistron/Cistron.h>
#include <GFort/Core/IObserver.h>
#include <Warrior/Model/Struct.h>
#include <Warrior/Units/Player.h>
#include <Warrior/Model/UnitAction.h>
#include "TypeDef.h"

namespace Warrior 
{
    
class UnitNode 
    : public cocos2d::CCNode, 
      public GFort::Core::IObserver<Unit>, 
      public Cistron::Component
{    
public:
    /// Constructor.
    UnitNode(Unit* unit);

    /// Destructor.
    ~UnitNode();

    /// Initialize.
    virtual bool init() = 0;
    
    /// Gets the bounding region.
    virtual BPolygon GetBoundingRegion() = 0;

    /// Reset all data in node.
    virtual void Reset();
    
    /// Kill the unit.
    void Die();

    /// Change facing direction.
    /// @param value
    void ChangeFacingDirection(const FacingDirection& value);
        
    /// Gets the state.
    Unit* GetState()                    { return state_; }
    
    /// Reset all parameters of the state.
    virtual void ResetState();

    /// Update observer.
    /// @param subject The subject that notify the observer
    void Update(Unit* subject);
         
protected:
    virtual void UpdateAction() = 0;
    virtual void FinishAction();

protected:
    /// Update the node.
    /// @param dt
    virtual void UpdateNode(cocos2d::ccTime dt) {}

protected:
    /// Resolve attack.
    void ResolveAttack();
        
    /// Set the node to be invisible.
    void SetInvisible();
        
protected:
    // Attributes
    Unit*                               state_;
        
    // If true, then the node follows physics motion. Else, it follows sprite motion
    bool                                use_physics_motion_;  
    
    // Instance Data
    UnitAnimationType                   animation_type_;
    bool                                needs_update_;
};

} // namespace

#endif // WARRIOR_UNIT_NODE_H_
