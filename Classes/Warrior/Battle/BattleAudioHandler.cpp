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

#include "BattleAudioHandler.h"

namespace Warrior 
{

const std::string kBgAudio              = "Assets/RT/zetuhomura.mp3";
const std::string kSliceAudio           = "Assets/FlashKit/Sword_Sl-Doogens-2173.wav";
const std::string kKillAudio            = "Assets/FlashKit/Hurt1-kayden_r-8958.wav";

enum AudioEffect
{
    kTag,
};
  
BattleAudioHandler::BattleAudioHandler() 
    : background_volume_(0),
      sound_effect_volume_(0)
{
}

BattleAudioHandler::~BattleAudioHandler()
{
}

void BattleAudioHandler::Initialize()
{
    CCLOG("[%s][%d] - Initializing", __FUNCTION__, __LINE__ );
    CocosDenshion::SimpleAudioEngine* engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    if (engine)
    {
        engine->preloadBackgroundMusic(kBgAudio.c_str());    
        engine->preloadEffect(kSliceAudio.c_str());
        engine->preloadEffect(kKillAudio.c_str());

        background_volume_ = engine->getBackgroundMusicVolume();
        sound_effect_volume_ = engine->getEffectsVolume();  
    }
}

void BattleAudioHandler::PlayBackgroundMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(kBgAudio.c_str(), true);
}

void BattleAudioHandler::PlaySliceEffect()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(kSliceAudio.c_str());
}

void BattleAudioHandler::PlayKillEffect()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(kKillAudio.c_str());
}

void BattleAudioHandler::Update(Battle* subject)
{
}

} // namespace
