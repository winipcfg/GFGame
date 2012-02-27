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

#ifndef SHMUP_GAME_H_
#define SHMUP_GAME_H_

#include <iostream>
#include <string>
#include <vector>
#include <Box2D/Box2D.h>
#include <Cistron/Cistron.h>
#include <GFort/Core/Game/Game.h>
#include <GFort/Core/Physics/PhysicsController.h>
#include "Units/Ship.h"

namespace GFort { namespace Games { namespace Shmup 
{

/// Game class that provide logics
class Game : public Cistron::ObjectManager, public GFort::Core::Game::Game
{
public:
    /// Constructor
    Game();

    /// Destructor
    ~Game()                                                         {};

    /// Initialize the game.
    virtual bool Initialize();

    /// Initialize the game.
    /// @param filePath The path of file
    virtual bool Initialize(const std::string& filePath);

    /// Takes actions for housekeeping. It should be called whenever necessary.
    virtual bool DoHousekeep();

    /// Exits the game.
    virtual void ExitGame();

    /// Takes actions for game.
    bool DoGameLoop() const;

    /// Returns true if the game is over.
    bool GameOver() const                                           { return game_over_; }

    /// Sets whether the game is over.
    /// @param gameOver
    void SetGameOver(const bool& gameOver)                          { game_over_ = gameOver; }

    /// Gets ship.
    /// @param index
    Ship& GetShip(const short& index)                               { return ships_[index]; }

    /// Gets current number of lives.
    const short& NumLives() const                                   { return num_lives_; }        

    /// Set current number of lives.
    /// @param numLives
    void SetNumLives(const short& numLives)                         { num_lives_ = numLives; }

    /// Gets maximum number of lives.
    const short& MaxNumLives() const                                { return max_num_lives_; }        

    /// Set maximum number of lives.
    /// @param maxNumLives
    void SetMaxNumLives(const short& maxNumLives)                   { max_num_lives_ = maxNumLives; }

    /// Gets the physics world.
    b2World* World()                                                { return phys_controller_.World(); }

    /// Gets the physics settings.
    GFort::Core::Physics::Box2dSettings& PhysicsSettings()          { return phys_settings_; }

public:
    /// Spawn an entity.
    Cistron::ObjectId SpawnEntity()                                 { return this->createObject(); }

    /// Spawn player ship at specified location.
    /// @param position
    /// @param side    
    Cistron::ObjectId SpawnPlayerShip(const b2Vec2& position)       { return SpawnPlayerShip(position, 0); }

    /// Spawn player ship at specified location.
    /// @param position
    /// @param side    
    Cistron::ObjectId SpawnPlayerShip(const b2Vec2& position, const short& side);

    /// Spawn player ship at specified location.
    /// @param position
    /// @param side    
    Cistron::ObjectId SpawnLaser(const b2Vec2& position, const short& side);

    /// Update the battle.
    void Update(const float& dt);

protected:
    /// Fire some bullets, laser, etc
    /// @param ship The ship to shoot
    void DoAttack(Ship* ship);
    
private:
    // Ships controlled by players
    Ship                                        ships_[1];

    short                                       num_lives_;
    short                                       max_num_lives_;

    bool                                        game_over_;  

    // Physics controller
    GFort::Core::Physics::PhysicsController     phys_controller_;
    GFort::Core::Physics::Box2dSettings         phys_settings_;

    // Instances
    bool                                        is_shooting_;
};  

} } } // namespace

#endif // SHMUP_GAME_H_
