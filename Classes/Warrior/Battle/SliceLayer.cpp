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
    cocos2d::CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(
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
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    //touch_start_position_ = touchLocation;
    touch_end_position_ = touchLocation;
    
    ClearSlice();
    blade_ = CCBlade::node();
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
    Point oldTouchLocation = pTouch->previousLocationInView(pTouch->view());
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

//void DrawLightning(
//    const float& x1, 
//    const float& y1, 
//    const float& x2, 
//    const float& y2, 
//    const float& displace,
//    const float& curDetail)
//{
//    if (displace < curDetail) 
//    {
//        Point segStart = Point(x1, y1);
//        Point segEnd = Point(x2, y2);
//        ccDrawLine(segStart, segEnd);
//    }
//    else 
//    {        
//        float mid_x = (x2 + x1) / 2;
//        float mid_y = (y2 + y1) / 2;
//        mid_x += (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * displace;
//        mid_y += (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * displace;
//        DrawLightning(x1,y1,mid_x,mid_y,displace/2,curDetail);
//        DrawLightning(x2,y2,mid_x,mid_y,displace/2,curDetail);
//    }
//}

//void DrawLightning(
//    const Point& start, 
//    const Point& end)
//{
//    int numBolt = 5;
//    float displacement = ccpLength(ccpSub(start, end)) / 5.0f;// 20.0f;
//    float boltWidth = 5;
//    const float curDetail = 4.0f;
//    const float startVar = 12.0f;
//    float x1, y1, x2, y2;
//    
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);    
//    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    for (unsigned int i = 0; i < numBolt; ++i)
//    {
//        float red = 1;//GFort::Core::MathHelper::RandomDouble<float>();
//        float green = GFort::Core::MathHelper::RandomDouble<float>();
//        float blue = 0;//GFort::Core::MathHelper::RandomDouble<float>();
//        glColor4f(red, green, blue, 0.2f);
//
//        glLineWidth(GFort::Core::MathHelper::RandomDouble<float>() * boltWidth);    
//        x1 = start.x + (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * startVar;
//        y1 = start.y + (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * startVar;
//        x2 = end.x + (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * startVar;
//        y2 = end.y + (GFort::Core::MathHelper::RandomDouble<float>() - 0.5f) * startVar;
//        DrawLightning(x1, y1, x2, y2, displacement, curDetail);
//    }
//    glLineWidth(1);
//
//    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}



void SliceLayer::draw(void)
{
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);

    cocos2d::ccDrawLine(GetStartPosition(), GetEndPosition());
    //Point start = GetStartPosition();
    //Point end = GetEndPosition();
    //DrawLightning(start, end);

    Point segStart;
    const std::list<BPoint > path = slice_.Path();
    if (path.size() > 0)
    {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

        list<BPoint >::const_iterator it = path.begin();
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

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

} // namespace
