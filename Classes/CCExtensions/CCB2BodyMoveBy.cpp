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

#include "CCB2BodyMoveBy.h"
#include "cocos2d.h"

namespace GFGame {

CCB2BodyMoveBy* CCB2BodyMoveBy::actionWithDuration(cocos2d::ccTime duration, b2Body* body, cocos2d::CCPoint position, const float& PTMRatio)
{
	CCB2BodyMoveBy *pMoveTo = new CCB2BodyMoveBy();
	pMoveTo->initWithDuration(duration, body, position, PTMRatio);
	pMoveTo->autorelease();

	return pMoveTo;
}

bool CCB2BodyMoveBy::initWithDuration(cocos2d::ccTime duration, b2Body* body, cocos2d::CCPoint position, const float& PTMRatio)
{
	if (CCActionInterval::initWithDuration(duration))
	{
        m_pBody = body;
        m_fPTMRatio = PTMRatio;
        m_fLastElapsed = 0;
        m_delta = position;
		return true;
	}

	return false;
}

void CCB2BodyMoveBy::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
}

void CCB2BodyMoveBy::update(cocos2d::ccTime time)
{
    cocos2d::ccTime tick = time - m_fLastElapsed;
	if (m_pBody)
	{   
        m_pBody->SetTransform(
            b2Vec2(m_pBody->GetPosition().x + (m_delta.x * tick / m_fPTMRatio), 
                   m_pBody->GetPosition().y + (m_delta.y * tick / m_fPTMRatio)), 
            m_pBody->GetAngle());
	}
    m_fLastElapsed = time;
}

}
