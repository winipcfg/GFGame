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

#ifndef WARRIOR_SLICE_LAYER_H_
#define WARRIOR_SLICE_LAYER_H_

#include <list>

#include "Trail.h"
#include "cocos2d.h"
#include "../TypeDef.h"
#include "CCExtensions/CCBlade.h"

namespace Warrior 
{

class SliceLayer : public cocos2d::CCLayer
{    
public:
    /// Constructor.
    SliceLayer();

    /// Destructor.
    ~SliceLayer();
    
    /// Set the slice to begin to dimish.
    void DimSlice();

    /// Clear the slice.
    void ClearSlice();

    /// Gets the length of slice.
    const float SliceLength()                       { return slice_.Length(); }

    /// Gets the start position of slice.
    const Point& GetStartPosition() const;

    /// Gets the end position of slice.
    const Point& GetEndPosition() const;
    
    /// Gets the slice
    Trail Slice() const                             { return slice_; }
        
    /// Returns true if it is inside specified region.
    /// @param region
    bool Within(const BPolygon& region)             { return slice_.Within(region); }

    /// Returns true if it collides with region.
    /// @param region
    /// @param[out] turns intersection points
    bool Collide(const BPolygon& region, std::vector<BTurnInfo>& turns)    { return slice_.Collide(region, turns); }
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        
    virtual void draw(void);

    CREATE_FUNC(SliceLayer)
    
protected:
private:
    
private:
    // Stores the start/end position of slice.
    //Point                   touch_start_position_;
    Point                   touch_end_position_;
    
    // Stores the path and length of slice.
    CCBlade*                blade_;
    Trail                   slice_;    
};

} // namespace

#endif // WARRIOR_SLICE_LAYER_H_
