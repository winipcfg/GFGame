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

#pragma once
#ifndef CCB2BODYMOVEBY_H_
#define CCB2BODYMOVEBY_H_

#include <Box2D/Box2D.h>
#include "cocos2d.h"

namespace GFGame 
{
    
using namespace cocos2d;

/** @brief Moves a B2Body object by x,y. x and y are absolute coordinates by modifying it's position attribute.
*/
class CCB2BodyMoveBy : public CCActionInterval
{
public:
	/** initializes the action */
	bool initWithDuration(ccTime duration, b2Body* body, CCPoint position, const float& PTMRatio);
    	
    virtual void startWithTarget(CCNode *pTarget);
	virtual void update(ccTime time);

public:
	/** creates the action */
	static CCB2BodyMoveBy* actionWithDuration(ccTime duration, b2Body* body, CCPoint position, const float& PTMRatio);

protected:
    b2Body* m_pBody;
    float   m_fPTMRatio;
	CCPoint m_delta;
    CGFloat m_fLastElapsed;
};


} // namepace

#endif //CCB2BODYMOVEBY_H_
