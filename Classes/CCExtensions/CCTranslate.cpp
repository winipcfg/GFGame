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

#include "CCTranslate.h"
#include "cocos2d.h"

namespace GFGame {

//
// MoveTo
//
CCTranslate* CCTranslate::create(float duration, cocos2d::CCPoint position)
{
	CCTranslate *pMoveTo = new CCTranslate();
	pMoveTo->initWithDuration(duration, position);
	pMoveTo->autorelease();

	return pMoveTo;
}

bool CCTranslate::initWithDuration(float duration, cocos2d::CCPoint position)
{
	if (CCActionInterval::initWithDuration(duration))
	{
        m_fLastElapsed = 0;
        m_delta = position;
		return true;
	}

	return false;
}

void CCTranslate::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
}

void CCTranslate::update(float time)
{
    float tick = time - m_fLastElapsed;
	if (m_pTarget)
	{        
        cocos2d::CCPoint position = m_pTarget->getPosition();
        position.x = position.x + m_delta.x * tick;
        position.y = position.y + m_delta.y * tick;
		m_pTarget->setPosition(position);
	}
    m_fLastElapsed = time;
}

}
