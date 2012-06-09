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

#ifndef WARRIOR_BATTLE_LAYER_H_
#define WARRIOR_BATTLE_LAYER_H_

#include <vector>
#include <GFort/Core/IObserver.h>
#include "GameStat.h"
#include "Battle.h"
#include "ViewController\/BattleHUD.h"
#include <Cistron/Cistron.h>

#include "cocos2d.h"
#include "../TypeDef.h"
#include "SliceLayer.h"
#include "BattleAudioHandler.h"
#include "../EmitterHandler.h"
#include "Units/Nodes/PlayerNode.h"
#include "Units/Nodes/EnemyNode.h"
#include "Units/Nodes/MissileNode.h"
#include "Landscape.h"

namespace Warrior 
{

class BattleLayer : public cocos2d::CCLayer, public Cistron::Component
{    
public:
    /// Constructor.
    BattleLayer();

    /// Destructor.
    ~BattleLayer();

    /// Setup the HUD viewer of this layer.
    /// @param viewer
    void SetViewer(BattleHUD* viewer);

    /// TEMP
    Battle* GetBattle()                     { return &battle_; }
    
    /// Gets player node.
    PlayerNode* GetPlayerNode()             { return player_node_; }

    void DoSlice();

    /// Resolve attack.
    /// @param attacker
    /// @param target
    void ResolveAttack(UnitNode& attacker, UnitNode& target);
            
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        
    LAYER_NODE_FUNC(BattleLayer)
    
protected:
private:
    void SetupGame();

    void SpawnPlayer(const Point& position);
    void SpawnEnemy();
    void SpawnEnemy(const Point& position);
    
    void UpdateNode(cocos2d::ccTime dt);

    void UpdateCamera(const Point& position);
    
    virtual void draw(void);

private:
    // Stores time information
    double                                      duration_;

    // Stores touch points
    Point                                       touch_start_position_;
    Point                                       touch_end_position_;

    // Stores game information (will be moved later)
    Unit*                                       player_;
    PlayerNode*                                 player_node_;
    std::vector<UnitNode*>                      enemies_nodes_;
    int                                         next_enemy_;
    double                                      next_enemy_spawn_;
    
    Battle                                      battle_;
    Landscape*                                  landscape_;
        
    // Stores handlers (Views)
    BattleAudioHandler                          audio_handler_;
    EmitterHandler                              emitter_handler_;

    // Stores related layer
    BattleHUD*                                  hud_layer_;
    SliceLayer*                                 slice_layer_;
};

} // namespace

#endif // WARRIOR_BATTLE_LAYER_H_
