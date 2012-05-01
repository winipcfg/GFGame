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

#ifndef WARRIOR_BOT_H_
#define WARRIOR_BOT_H_

#include <Cistron/Cistron.h>

namespace Warrior 
{

class UnitNode;
class BattleLayer;

/// Base class of a Bot
class Bot : public Cistron::Component
{
public:
    /// Constructor.
    Bot(UnitNode* node);

    /// Destructor.
    ~Bot();

    ///// Attach to node.
    ///// @param node
    //void Attach(UnitNode* node);

    /// Think and do some actions for a single turn.
    /// This method will not be called if the unit is not ready to think.
    /// @param gameState 
    virtual void Update(BattleLayer& gameState) = 0;
    
protected:
    // Stores the pointer of unit 
    UnitNode*             node_;
};


} // namepsace

#endif // WARRIOR_BOT_H_
