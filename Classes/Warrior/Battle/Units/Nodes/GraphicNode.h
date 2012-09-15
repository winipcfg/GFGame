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

#ifndef WARRIOR_GRAPHIC_NODE_H_
#define WARRIOR_GRAPHIC_NODE_H_

#include "cocos2d.h"
#include <Cistron/Cistron.h>
#include <CCExtensions/AdvanceSprite.h>
#include "../Unit.h"

namespace Warrior 
{

class GraphicNode : public cocos2d::CCNode, public Cistron::Component
{    
public:
    /// Constructor.
    GraphicNode(Unit* unit);

    /// Destructor.
    ~GraphicNode();

    /// Gets the size of bounding box.
    cocos2d::CCSize Size();

    /// Initialize.
    virtual bool init();

    /// Gets position and rotation of the component.
    /// @param[out] position
    /// @param[out] rotation
    void GetPositionAndRotation(b2Vec2& position, float& rotation);

    /// Update position and rotation.
    /// @param position
    /// @param rotation
    void UpdatePositionAndRotation(const b2Vec2& position, const float& rotation);

    /// Sets the offset of graphic.
    /// @param value
    void SetOffset(cocos2d::CCPoint value)  { offset_ = value; }

    /// Gets the offset of graphic.
    cocos2d::CCPoint Offset() const         { return offset_; }
        
protected:
    /// Show animation.
    void ShowAnimation();

    /// Stop current animation.
    void StopAnimation();

public:
    /// Update the node.
    /// @param dt
    void UpdateNode(float dt);

    /// Destroy itself.
    void Destroy();

    /// Resolve attack.
    void ResolveAttack();
    
private:
    // Attributes
    Unit*                               state_;

    AdvanceSprite*                      sprite_;
    cocos2d::CCPoint                    offset_;
    FacingDirection                     facing_;
    UnitActionType                      action_type_;
    bool                                needs_update_;    
};

} // namespace

#endif // WARRIOR_GRAPHIC_NODE_H_
