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

#ifndef WARRIOR_PLAYER_NODE_H_
#define WARRIOR_PLAYER_NODE_H_

#include "cocos2d.h"
#include "../Player.h"
#include "UnitNode.h"

namespace Warrior 
{

class PlayerNode : public UnitNode, public GFort::Core::ISubject<PlayerNode>
{    
public:
    /// Constructor.
    PlayerNode(Unit* unit);

    /// Destructor.
    ~PlayerNode();

    /// Initialize.
    virtual bool init();

    /// Reset all parameters of the state.
    virtual void ResetState();
            
protected:
    void UpdateNode(cocos2d::ccTime dt);        

    /// Update action based on current command.
    virtual void UpdateAction();

    /// Walk to specified location.
    /// @param position
    virtual void Walk(const cocos2d::CCPoint& position);

    /// Run to specified location.
    /// @param position
    virtual void Run(const cocos2d::CCPoint& position);

    /// Perform attack.
    /// @param action
    virtual void Attack(UnitAction& action);
};

} // namespace

#endif // WARRIOR_PLAYER_NODE_H_
