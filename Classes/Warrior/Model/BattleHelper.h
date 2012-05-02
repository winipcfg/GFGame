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

#ifndef WARRIOR_BATTLE_HELPER_H_
#define WARRIOR_BATTLE_HELPER_H_

#include <string>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <Warrior/Model/Struct.h>
#include <Warrior/Units/Unit.h>
#include <Warrior/Units/Enemy.h>
#include <Warrior/Model/Trail.h>

namespace Warrior 
{

/// Helper class for battle
class BattleHelper
{
public:
    /// Returns clockwise polygon of given b2Body.
    /// @param body
    static BPolygon GetPolygon(const b2Body& body);

    /// Convert to Polygon.
    /// @param minX
    /// @param minY
    /// @param maxX
    /// @param maxY
    static BPolygon ConvertToPolygon(const float& minX, const float& minY, const float& maxX, const float& maxY);

    ///// Returns bounding region of given b2Body.
    ///// @param body
    //static BPolygon GetBoundingRegion(const b2Body& body);

	/// Find units that collide with the trail.
	/// @param trail
	/// @param units
	static std::set<Unit*> Collide(const Trail& trail, std::vector<Unit*>& units);

    /// Find units that collide with the trail.
	/// @param trail
	/// @param units
    /// @param[out] collideUnits
    /// @param[out] collidePoints
	static void Collide(const Trail& trail, std::vector<Unit*>& units, std::set<Unit*>& collideUnits, std::list<std::vector<BTurnInfo> >& collidePoints);
    
    /// Apply force on unit.
    /// @param target
    /// @param position
    /// @param impulse
    static void ApplyForceOnUnit(Unit* target, const BPoint& position, const b2Vec2& impulse);

    /// Get abbreviation of specified object.
    /// @param type
    static std::string GetAbbreviation(const UnitActionType& type);
};

} // namespace

#endif // WARRIOR_BATTLE_HELPER_H_
