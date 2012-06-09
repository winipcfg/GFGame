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

#ifndef WARRIOR_TRAIL_H_
#define WARRIOR_TRAIL_H_

#include <vector>
#include <GFort/Core/Structs.h>
#include <Warrior/TypeDef.h>

namespace Warrior 
{

/// The class stores the path of trail
class Trail
{    
public:
    /// Constructor.
    Trail();
    
    /// Destructor.
    ~Trail();  
    
    /// Push node into container.
    /// @param point
    void Push(const BPoint& point);
    
    /// Pop.
    void Pop();
    
    /// Pop.
    /// @param numPoints
    void Pop(const short& numPoints);
    
    /// Simplify path nodes.
    /// @param distance distance of a vertex to other segments to be removed
    void Simplify(const short& distance);
    
    /// Clear the container.
    void Clear();    

    /// Gets the front of the trail.
    const BPoint& GetStartPosition() const;

    /// Gets the back of the trail.
    const BPoint& GetEndPosition() const;

    /// Gets the trail nodes.
    const std::list<BPoint >& Path() const        { return point_list_; }

    /// Gets the length of trail.
    const float Length();

    /// Gets the displacement between start and end point of trail.
    const float Displacement() const;

    /// Returns true if it is inside specified region.
    /// @param region
    bool Within(const BPolygon& region) const;

    /// Returns true if it collides with region.
    /// @param region
    /// @param[out] turns intersection points
    bool Collide(const BPolygon& region, std::vector<BTurnInfo>& turns) const;
    
    /// Sets maximum number of nodes allowed. -1 means no limit
    /// @param value
    void SetMaxPoints(const short& value);
    
protected:
    /// Update internal data.
    void Update();
                
private:
    // Stores nodes of trail
    std::list<BPoint >          point_list_;   

    // Store the length of trail
    float                       length_;

    // Store maximum number of points allowed
    short                       max_points_;

    // Flag that requires system to update data
    bool                        needs_update_;
};

} // namespace

#endif // WARRIOR_TRAIL_H_
