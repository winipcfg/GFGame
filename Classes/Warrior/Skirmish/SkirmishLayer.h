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
#ifndef GFGAME_WARRIOR_SKIRMISHLAYER_H_
#define GFGAME_WARRIOR_SKIRMISHLAYER_H_

#include <vector>
#include <set>
#include <hash_map>
#include <Cistron/Cistron.h>
#include "cocos2d.h"
#include <CCExtensions/AdvanceSprite.h>
#include <CCExtensions/CCLayerPanZoom.h>
#include "ControlPoint.h"

namespace Warrior 
{

class SkirmishLayer : public cocos2d::CCLayer, public Cistron::Component
{   
public:
    typedef cocos2d::CCPoint                            Point;

public:
    /// Constructor.
    /// @param game
    SkirmishLayer();

    /// Destructor.
    ~SkirmishLayer();
            
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    LAYER_CREATE_FUNC(SkirmishLayer);
        
private:
    void SetupViewer();
    void RefreshViewer();

    void ControlPointCallback(CCObject* pSender);

    cocos2d::CCPoint GetBoundLayerPosition(const cocos2d::CCPoint& newPos);

    /// Set the current point of player.
    /// @param index
    /// @param duration
    void SetCurrentPoint(const short& index, const float& duration);
    
    /// Returns true if the control point is accessible.
    /// @param index
    const bool IsControlPointAccessible(const short& index) const;

    virtual void draw(void);
    
private:
    // Stores time information
    double                              duration_;
    Point                               touch_start_position_;
    Point                               touch_end_position_;

    cocos2d::CCMenu*                    stage_menu_;
    cocos2d::AdvanceSprite*             player_node_;
    short                               current_point_;
    std::vector<ControlPoint>           control_points_;
    std::vector<ControlPointLink>       control_point_links_;

    cocos2d::CCSprite*                  background_;
    std::vector<cocos2d::CCSprite*>     accessible_indicators_;
    cocos2d::CCTexture2D*               texture_ok_;
    cocos2d::CCTexture2D*               texture_not_ok_;
    cocos2d::CCParticleSystemQuad*      effect_indicator_;

    // The set of control points that allow player to access
    std::set<short>                     accessible_control_points_;
    
    friend class SkirmishScene;
};

} // namespace

#endif // GFGAME_WARRIOR_SKIRMISHLAYER_H_
