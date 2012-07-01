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

#ifndef WARRIOR_LANDSCAPE_H_
#define WARRIOR_LANDSCAPE_H_

#include <vector>
#include <GFort/Core/IObserver.h>
#include "GameStat.h"
#include "Battle.h"

#include "cocos2d.h"

namespace Warrior 
{

/// Base landscape class for battle. Any graphic effect related to landscape should be done in the class.
class Landscape : public cocos2d::CCParallaxNode
{    
public:
    /// Constructor.
    Landscape();

    /// Destructor.
    ~Landscape();

    /// Initialize.
    virtual bool init();

    /// Gets the main layer, which involves game objects such as characters.
    cocos2d::CCNode* GetMainLayer();

    static Landscape* create()
	{
		Landscape *pRet = new Landscape();
		if (pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

protected:
private:
    
private:
    cocos2d::CCNode*                          landscape_overlay_;
    cocos2d::CCNode*                          landscape_front_;
    cocos2d::CCNode*                          landscape_back_;
    cocos2d::CCNode*                          landscape_backdrop_; 
};

} // namespace

#endif // WARRIOR_LANDSCAPE_H_
