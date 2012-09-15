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

#ifndef GFGAME_WARRIOR_UNIT_VIEWER_H_
#define GFGAME_WARRIOR_UNIT_VIEWER_H_

#include "cocos2d.h"
#include "../Units/Unit.h"

namespace Warrior
{

/// Class for viewing unit information
class UnitViewer : public cocos2d::CCLayer
{    
public:
    /// Constructor.
    UnitViewer();

    /// Destructor.
    ~UnitViewer();

    /// Set the unit that it refers to.
    void SetUnit(Unit* unit)            { unit_ = unit; }
        
    CREATE_FUNC(UnitViewer)

protected:
    /// Update the node.
    /// @param dt
    virtual void UpdateNode(float dt);
   
protected:
    Unit*                               unit_;
    cocos2d::CCLabelTTF*                label_action_;

private:
    bool                                alive_;
    UnitActionType                      current_action_type_;
};

} // namespace

#endif // GFGAME_WARRIOR_UNIT_VIEWER_H_
