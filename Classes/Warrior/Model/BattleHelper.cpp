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

#include "BattleHelper.h"

namespace Warrior 
{

const short     kCircleSegments         = 16;
const float32   kCircleSegmentIncrement = 2.0f * b2_pi / static_cast<float32>(kCircleSegments);



BPolygon BattleHelper::GetPolygon(const b2Body& body)
{
    BPolygon polygon;
    BPoint pt;

    const b2Transform& xf = body.GetTransform();
    for (const b2Fixture* fixture = body.GetFixtureList(); 
         fixture; 
         fixture = fixture->GetNext())
	{
		switch (fixture->GetType())
	    {
	    case b2Shape::e_circle:
		    {
			    b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();	
                b2Vec2 vertices[kCircleSegments];
                float32 theta = 0.0f;
                for (int i = 0; i < kCircleSegments; ++i)
	            {
                    b2Vec2 center = b2Mul(xf, circle->m_p);
		            vertices[i] = PTM_RATIO * (center + circle->m_radius * b2Vec2(cosf(theta), sinf(theta)));
                    pt.set<0>(vertices[i].x);
                    pt.set<1>(vertices[i].y);
                    boost::geometry::append(polygon, pt);
		            theta += kCircleSegmentIncrement;
	            }
                if (kCircleSegments > 1)
                {
                    pt.set<0>(vertices[0].x);
                    pt.set<1>(vertices[0].y);
                    boost::geometry::append(polygon, pt);
                }
		    }
		    break;

	    case b2Shape::e_polygon:
		    {
			    b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			    int32 vertexCount = poly->m_vertexCount;
			    b2Assert(vertexCount <= b2_maxPolygonVertices);
			    b2Vec2 vertices[b2_maxPolygonVertices];
                                
                for (int32 i = vertexCount - 1; i >= 0; --i)
			    {
                    vertices[i] = PTM_RATIO * b2Mul(xf, poly->m_vertices[i]);
                    pt.set<0>(vertices[i].x);
                    pt.set<1>(vertices[i].y);
                    boost::geometry::append(polygon, pt);
                }
                if (vertexCount > 1)
                {
                    pt.set<0>(vertices[vertexCount - 1].x);
                    pt.set<1>(vertices[vertexCount - 1].y);
                    boost::geometry::append(polygon, pt);
                }
		    }
		    break;
	    }
	}
    return polygon;
}

BPolygon BattleHelper::ConvertToPolygon(const float& minX, const float& minY, const float& maxX, const float& maxY)
{
    BPolygon polygon;                                                           
    boost::geometry::append(polygon, BPoint(minX, maxY));
    boost::geometry::append(polygon, BPoint(maxX, maxY));
    boost::geometry::append(polygon, BPoint(maxX, minY));
    boost::geometry::append(polygon, BPoint(minX, minY));
    boost::geometry::append(polygon, BPoint(minX, maxY));
    return polygon;
}

std::set<Unit*> BattleHelper::Collide(const Trail& trail, std::vector<Unit*>& units)
{
	std::set<Unit*> result;
	Unit* unit;
    std::vector<BTurnInfo> turns;
    for (unsigned int i = 0; i < units.size(); ++i)
    {
        unit = units[i];
        turns.clear();
        if (trail.Collide(unit->GetBoundingRegion(), turns))
        {
            result.insert(unit);
        }
    }
	return result;
}

void BattleHelper::ApplyForceOnUnit(Unit* target, const BPoint& position, const b2Vec2& impulse)
{
    if (target->Body())
    {
        target->Body()->ApplyForce(
            impulse, 
            b2Vec2(position.x() * GFort::Core::Physics::kINV_PTM_RATIO, 
                   position.y() * GFort::Core::Physics::kINV_PTM_RATIO));
    }
}

std::string BattleHelper::GetAbbreviation(const UnitActionType& type)
{
    switch (type)
    {
    case kUnitActionTypeIdle:
        return "I";
    case kUnitActionTypeWalk:	
        return "W";
    case kUnitActionTypeMoveLeft:
        return "L";
    case kUnitActionTypeMoveRight:
        return "R";
    case kUnitActionTypeRun:
        return "S";
    case kUnitActionTypeAttack:
        return "A";
    default:
        return "X";
    }
}

} // namespace
