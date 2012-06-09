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

#include "Trail.h"
#include <vector>
#include <boost/assign.hpp>
#include <boost/geometry/geometry.hpp> 
#include <GFort/Core/StringHelper.h>

using namespace boost::assign;

namespace Warrior 
{

Trail::Trail()
    : length_(0),
      max_points_(-1),
      needs_update_(false)
{
}

Trail::~Trail()
{
    point_list_.clear();
}

void Trail::Push(const BPoint& point)
{   
    if (point_list_.size() > 0)
        length_ += boost::geometry::distance(point, point_list_.back());   
    
    point_list_.push_back(point);
    if (max_points_ > 0 &&
        point_list_.size() - max_points_ > 0)
    {
        point_list_.pop_front();
        needs_update_ = true;
    }
}

void Trail::Pop()
{
    Pop(1);
}

void Trail::Pop(const short& numPoints)
{
    if (point_list_.size() > 0 && numPoints > 0) 
    {
        short n = (point_list_.size() - numPoints > 0) ? numPoints : point_list_.size();
        while (n > 0)
        {
            point_list_.pop_front();
            n--;
        }
        needs_update_ = true;
	}
}

void Trail::Simplify(const short& distance)
{
    BLine line;        
    for (std::list<BPoint >::iterator it = point_list_.begin(); 
        it != point_list_.end(); 
        ++it)
    {
        line.push_back(BPoint(it->x(), it->y()));
    }

    BLine simplified;
    boost::geometry::simplify(line, simplified, distance);

    point_list_.clear();
    for (boost::geometry::model::linestring<BPoint >::iterator it = simplified.begin(); 
        it != simplified.end(); 
        ++it)
    {
        point_list_.push_back(BPoint(it->x(), it->y()));
    }
}

void Trail::Clear()
{
    point_list_.clear();
    needs_update_ = true;
}

const BPoint& Trail::GetStartPosition() const
{
    if (point_list_.size() > 0)
        return point_list_.front();
    else
        return BPoint(0, 0);
}

const BPoint& Trail::GetEndPosition() const
{
    if (point_list_.size() > 0)
        return point_list_.back();
    else
        return BPoint(0, 0);
}

const float Trail::Length()  
{ 
    Update();
    return length_; 
}

const float Trail::Displacement() const
{
    if (point_list_.size() < 2)
        return 0;
    else
        return boost::geometry::distance(point_list_.front(), point_list_.back());
}

bool Trail::Within(const BPolygon& region) const
{
    for (std::list<BPoint >::const_iterator it = point_list_.begin(); 
        it != point_list_.end(); 
        ++it)
    {
        if (!boost::geometry::within(*it, region))
            return false;
    }
    return true;
}

bool Trail::Collide(const BPolygon& region, std::vector<BTurnInfo>& turns) const
{    
    BLine line;
    boost::geometry::detail::get_turns::no_interrupt_policy policy;
    
    for (std::list<BPoint >::const_iterator it = point_list_.begin(); 
        it != point_list_.end(); 
        ++it)
    {
        line.push_back(BPoint(it->x(), it->y()));
    }

    // Calculate intersection points (turn points)
    boost::geometry::get_turns<
        false, 
        false, 
        boost::geometry::detail::overlay::assign_null_policy>(
        line, region, turns, policy);
    
    return turns.size() > 0;
}

void Trail::SetMaxPoints(const short& value)
{
    if (max_points_ != value)
    {
        max_points_ = value;
        if (max_points_ > 0 &&
            point_list_.size() - max_points_ > 0)
        {
            short n = point_list_.size() - max_points_;
            while (n > 0)
            {
                point_list_.pop_front();
                n--;
            }
        }
        needs_update_ = true;
    }
}

void Trail::Update()
{
    if (needs_update_)
    {
        length_ = 0;
        if (point_list_.size() > 0)
        {
            std::list<BPoint >::iterator it;
            
            BPoint first = point_list_.front();
            BPoint second;
            it = point_list_.begin();
            it++;
            for (; it != point_list_.end(); ++it)
            {
                second = *it;
                length_ += boost::geometry::distance(first, second); 
                first = second;
            }
        }   

        needs_update_ = false;
    }
}

} // namespace
