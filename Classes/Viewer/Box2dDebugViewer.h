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

#ifndef GFGAME_VIEWER_BOX2D_DEBUG_VIEWER_H_
#define GFGAME_VIEWER_BOX2D_DEBUG_VIEWER_H_

#include <GFort/Core/Physics/PhysicsController.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "cocos2d.h"
#include "GLES-Render.h"

namespace GFGame { namespace Viewer 
{

/// Class for viewing Box2d world
class Box2dDebugViewer : public cocos2d::CCLayer
{    
public:
    /// Constructor.
    Box2dDebugViewer();

    /// Destructor.
    ~Box2dDebugViewer();

    /// Sets the world for display.
    /// @param world
    /// @param settings
    void SetWorld(b2World* world, GFort::Core::Physics::Box2dSettings* settings);

    virtual void draw(void);
    
    CREATE_FUNC(Box2dDebugViewer)
   
protected:
    b2World*                                    world_;
    GLESDebugDraw*                              debug_draw_;
    GFort::Core::Physics::Box2dSettings*        settings_;
};

} } // namespace

#endif // GFGAME_VIEWER_BOX2D_DEBUG_VIEWER_H_
