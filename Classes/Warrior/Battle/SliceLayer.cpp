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

#include "SliceLayer.h"
#include <string>
#include <GFort/Core/MathHelper.h>

const int           kMinSliceLength   = 20.0f;
const float         kMaxSliceLength   = 300.0f;
const short         kSimplifyDistance = 10;
const std::string   kBlade            = "streak1.png";

using namespace cocos2d;

namespace Warrior 
{

SliceLayer::SliceLayer()
    : blade_(NULL)
{    
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, 
        0, 
        false);
}

SliceLayer::~SliceLayer()
{    
}

void SliceLayer::DimSlice()
{
    if (blade_)
        blade_->Reset();
}

void SliceLayer::ClearSlice()
{
    if (blade_)
    {
        CCLOG("[%s][%d] - Clear Slice", __FUNCTION__, __LINE__);
        blade_->Clear();
        slice_.Clear();
        
        //removeChild(blade_, false);
        //blade_ = NULL;
    }
}

const Point& SliceLayer::GetStartPosition() const
{
    //return slice_.GetStartPosition();
    BPoint point = slice_.GetStartPosition();
    return Point(point.x(), point.y());
}

const Point& SliceLayer::GetEndPosition() const
{
    //return slice_.GetEndPosition();
    BPoint point = slice_.GetEndPosition();
    return Point(point.x(), point.y());
}

bool SliceLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{    
    CCLOG("[%s][%d] - Touch Begin", __FUNCTION__, __LINE__);

    Point touchLocation = convertTouchToNodeSpace(pTouch);
    //touch_start_position_ = touchLocation;
    touch_end_position_ = touchLocation;
    
    ClearSlice();
    blade_ = CCBlade::create();
    blade_->texture_ = cocos2d::CCTextureCache::sharedTextureCache()->addImage(kBlade.c_str());
    
    addChild(blade_,-1,-1);
    blade_->Push(touchLocation);

    BPoint point(touchLocation.x, touchLocation.y);
    slice_.Push(point);
    return true;
}

void SliceLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    Point oldTouchLocation = pTouch->previousLocationInView();
    oldTouchLocation = director->convertToGL(oldTouchLocation);
    oldTouchLocation = convertToNodeSpace(oldTouchLocation);
    
    if (slice_.Length() < kMaxSliceLength)
    {
        float dx, dy;
        dx = touchLocation.x - touch_end_position_.x;
        dy = touchLocation.y - touch_end_position_.y;
        float lengthSquared = (float)(dx * dx + dy * dy);        
        float length = sqrt(lengthSquared);
        
        if ((slice_.Length() + length) <= kMaxSliceLength)
        {
            blade_->Push(touchLocation);

            BPoint point(touchLocation.x, touchLocation.y);
            slice_.Push(point);
        }
        else if (length > 0)
        {
            CCLOG("Slice length is larger than limit");
            float newLength = kMaxSliceLength - slice_.Length();
            
            Point newPoint;
            float distance = newLength / length;
            newPoint.x = touch_end_position_.x + (float)dx * distance;
            newPoint.y = touch_end_position_.y + (float)dy * distance;
            
            CCLOG("Push new point at (%0.2f, %0.2f)", newPoint.x, newPoint.y);
            blade_->Push(newPoint);

            BPoint point(newPoint.x, newPoint.y);
            slice_.Push(point);
        }
        
        touch_end_position_ = touchLocation;
    }
}

void SliceLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //touch_start_position_ = touch_end_position_ = cocos2d::CCPointZero;
    touch_end_position_ = cocos2d::CCPointZero;
    DimSlice();

#if (COCOS2D_DEBUG)
    unsigned int originalSize = slice_.Path().size();
#endif
    slice_.Simplify(kSimplifyDistance);

#if (COCOS2D_DEBUG)
    CCLOG("Slice nodes are reduced from %d nodes to %d nodes", originalSize, slice_.Path().size());
#endif

    //CCLOG("Total Child of SliceLayer: %d", this->getChildren()->count());
}

void SliceLayer::draw(void)
{
    //glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);

    cocos2d::ccDrawLine(GetStartPosition(), GetEndPosition());
    Point segStart;
    const std::list<BPoint > path = slice_.Path();
    if (path.size() > 0)
    {
        ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

        std::list<BPoint >::const_iterator it = path.begin();
        BPoint start = path.front();
        segStart = Point(start.x(), start.y());
        ++it;
        for (;it != path.end(); ++it)
        {            
            BPoint end = *it;
            Point segEnd = Point(end.x(), end.y());
            
            //CCLOG("Draw Line: (%0.2f, %0.2f), (%0.2f, %0.2f)", segStart.x, segStart.y, segEnd.x, segEnd.y);
            ccDrawLine(segStart, segEnd);   

            start = *it;
            segStart = Point(start.x(), start.y());
        }  

        ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

} // namespace
