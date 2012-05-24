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

#pragma once
#ifndef WARRIOR_ENTITYB2CATEGORY_H_
#define WARRIOR_ENTITYB2CATEGORY_H_

#include <Box2D/Box2D.h>

namespace Warrior
{

/// Mask Bits and Category Bits.
enum EntityB2Category 
{
    BOUNDARY            = 0x0001,
    GROUND              = 0x0002,
    OBSTACLE            = 0x0004,

    UNIT                = 0x0010,
    MISSILE             = 0x0020,    
    
    FRIENDLY            = 0x0100,
    ENEMY               = 0x0200,
};

static const uint16&       kCategoryBitsBoundary       = BOUNDARY;
static const uint16&       kMaskBitsBoundary           = 0xFFFF;

static const uint16&       kCategoryBitsGround         = GROUND;
static const uint16&       kMaskBitsGround             = 0xFFFF;

static const uint16&       kCategoryBitsPlayer         = FRIENDLY | UNIT;
static const uint16&       kMaskBitsPlayer             = BOUNDARY | GROUND | OBSTACLE | ENEMY;

static const uint16&       kCategoryBitsEnemy          = ENEMY | UNIT;
//static const uint16&       kMaskBitsEnemy              = BOUNDARY | GROUND | OBSTACLE;
static const uint16&       kMaskBitsEnemy              = BOUNDARY | GROUND | FRIENDLY | OBSTACLE;

static const uint16&       kCategoryBitsMissile        = ENEMY | MISSILE;
static const uint16&       kMaskBitsMissile            = GROUND | OBSTACLE;
//static const uint16&       kMaskBitsMissile            = GROUND | FRIENDLY | OBSTACLE;

} // namespace

#endif // WARRIOR_ENTITYB2CATEGORY_H_
