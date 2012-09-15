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
#ifndef GFGAME_SHMUP_SHMUPLAYER_H_
#define GFGAME_SHMUP_SHMUPLAYER_H_

#include <vector>
#include <Cistron/Cistron.h>
#include "cocos2d.h"
#include <CCExtensions/HSJoystick.h>
#include <CCExtensions/AdvanceSprite.h>
#include "../Game.h"
#include "../Units/ShipNode.h"

namespace GFort { namespace Games { namespace Shmup 
{

class ShmupLayer 
    : public cocos2d::CCLayer
    , public Cistron::Component
{   
public:
    typedef cocos2d::CCPoint                            Point;

public:
    /// Constructor.
    /// @param game
    ShmupLayer(GFort::Games::Shmup::Game* game);

    /// Destructor.
    ~ShmupLayer();
            
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
        
    //CREATE_FUNC(ShmupLayer)
    
protected:
    void DoFire();
    void SpawnAsteroid();

private:
    void SetupGame();

    void UpdateNode(float dt);
    
    /// Attach render components of an object.
    /// @param objId
    void AttachRenderComponents(const Cistron::ObjectId& objId);

    virtual void draw(void);
    
private:
    // Stores time information
    double                      duration_;
    Point                       touch_start_position_;
    Point                       touch_end_position_;

    HSJoystick*                 joystick_;

    // Reference
    GFort::Games::Shmup::Game*                  game_;

    // TEMP
    bool                                        is_shooting_;
    double                                      last_shoot_time_;

    Ship*                                   ship_;

public:
    ShipNode*                               ship_node_;
    
    double                                   next_asteroid_;
};

} } } // namespace

#endif // GFGAME_SHMUP_SHMUPLAYER_H_
