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

#ifndef GFGAME_COMPONENTS_RENDER_COMPONENT_H_
#define GFGAME_COMPONENTS_RENDER_COMPONENT_H_

#include <Cistron/Cistron.h>
#include <cocos2d.h>

namespace GFGame { namespace Components 
{

using namespace cocos2d;

class RenderComponent : public Cistron::Component
{
public:
    /// Constructor.
    RenderComponent(cocos2d::CCNode* node);

    /// Destructor.
    ~RenderComponent();

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

    /// Return the node.
    cocos2d::CCNode* Node() { return node_; }
                    
private:    
    cocos2d::CCNode*        node_;
    cocos2d::CCPoint        offset_;
};  

    
inline RenderComponent::RenderComponent(cocos2d::CCNode* node)
    : Cistron::Component("RenderComponent"),
      node_(node)
{
}

inline RenderComponent::~RenderComponent()
{
    node_->removeAllChildrenWithCleanup(true);
    node_->removeFromParentAndCleanup(true);
    node_ = NULL;
}

inline void RenderComponent::GetPositionAndRotation(b2Vec2& position, float& rotation)
{    
    position.Set(node_->getPosition().x * GFort::Core::Physics::kINV_PTM_RATIO,
        node_->getPosition().y * GFort::Core::Physics::kINV_PTM_RATIO);
    rotation = -1 * CC_DEGREES_TO_RADIANS(node_->getRotation());
}

inline void RenderComponent::UpdatePositionAndRotation(const b2Vec2& position, const float& rotation)
{
    float rad = CC_RADIANS_TO_DEGREES(-1 * rotation);
    node_->setRotation(rad);        
    node_->setPosition(ccp(position.x + offset_.x, position.y + offset_.y));
}
    
} } // namespace

#endif // GFGAME_COMPONENTS_RENDER_COMPONENT_H_
