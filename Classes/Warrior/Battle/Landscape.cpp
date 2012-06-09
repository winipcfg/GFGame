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

#include "Landscape.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kParallexFront      = "Assets/Scene/graveyard_f.png";
const std::string   kParallexBack       = "Assets/Scene/graveyard_b.png";
const std::string   kParallexBackdrop   = "Assets/Scene/graveyard_bd.png";

Landscape::Landscape()
    : landscape_overlay_(NULL),
      landscape_front_(NULL),
      landscape_back_(NULL),
      landscape_backdrop_(NULL)
{  
}

Landscape::~Landscape()
{  
}

bool Landscape::init()
{
    cocos2d::CCSprite* sprite;
    
    // Landscape   
    landscape_front_ = cocos2d::CCNode::node();
    sprite = cocos2d::CCSprite::spriteWithFile(kParallexFront.c_str());
    sprite->setAnchorPoint(cocos2d::CCPointZero);    
    landscape_front_->addChild(sprite, 0, 0);    

    landscape_back_ = cocos2d::CCNode::node();
    sprite = cocos2d::CCSprite::spriteWithFile(kParallexBack.c_str());
    sprite->setAnchorPoint(cocos2d::CCPointZero);    
    landscape_back_->addChild(sprite, 0, 0);    

    landscape_backdrop_ = cocos2d::CCNode::node();
    sprite = cocos2d::CCSprite::spriteWithFile(kParallexBackdrop.c_str());
    sprite->setAnchorPoint(cocos2d::CCPointZero);
    landscape_backdrop_->addChild(sprite, 0, 0);
    
    this->addChild(landscape_front_, 0, ccp(1.0f, 1.0f), CCPointZero);
    this->addChild(landscape_back_, -1, ccp(0.75f, 1.0f), CCPointZero);
    this->addChild(landscape_backdrop_, -2, ccp(0.5f, 1.0f), CCPointZero);
    
    return true;
}

cocos2d::CCNode* Landscape::GetMainLayer()
{
    return landscape_back_;
}

} // namespace
