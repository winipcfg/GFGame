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

#include "Box2dDebugViewer.h"
#include <string>
#include <GFort/Core/StringHelper.h>

using namespace cocos2d;

namespace GFGame { namespace Viewer 
{
    
Box2dDebugViewer::Box2dDebugViewer()
    : world_(NULL),
      debug_draw_(NULL),      
      settings_(NULL)
{
    debug_draw_ = new GLESDebugDraw(PTM_RATIO);
}

Box2dDebugViewer::~Box2dDebugViewer()
{
    if (debug_draw_ != NULL) 
    {
        delete debug_draw_;
        debug_draw_ = NULL;
    }
}

void Box2dDebugViewer::SetWorld(b2World* world, GFort::Core::Physics::Box2dSettings* settings)
{
    if (world && settings)
    {
        world_ = world;
        settings_ = settings;
        debug_draw_->SetFlags(settings_->GetDebugFlag());
        world_->SetDebugDraw(debug_draw_);
    }
}

void Box2dDebugViewer::draw(void)
{
    //if (world_)
    //{
    //    debug_draw_->SetFlags(settings_->GetDebugFlag());

    //    glDisable(GL_TEXTURE_2D);
	   // glDisableClientState(GL_COLOR_ARRAY);
	   // glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    //    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
 
	    world_->DrawDebugData();

    //    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
 
	   // // restore default GL states
	   // glEnable(GL_TEXTURE_2D);
	   // glEnableClientState(GL_COLOR_ARRAY);
	   // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //}
}

} } // namespace
