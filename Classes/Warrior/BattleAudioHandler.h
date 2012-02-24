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

#ifndef WARRIOR_BATTLE_AUDIO_HANDLER_H_
#define WARRIOR_BATTLE_AUDIO_HANDLER_H_

//#include <string>
//#include <map>

#include <GFort/Core/IObserver.h>
#include <Warrior/Model/Battle.h>
#include "cocos2d.h"
#include "CocosDenshion/include/SimpleAudioEngine.h"

namespace Warrior 
{

/// Node Class for handling battle audio
class BattleAudioHandler : public cocos2d::CCNode, public GFort::Core::IObserver<Battle>
{    
public:
    /// Constructor.
    BattleAudioHandler();

    /// Destructor.
    ~BattleAudioHandler();

    /// Initialize the handler.
    void Initialize();

    /// Play background music.
    void PlayBackgroundMusic();

    /// Play effect.
    void PlaySliceEffect();

    /// Play effect.
    void PlayKillEffect();

	/// Update observer.
    /// @param subject The subject that notify the observer
    void Update(Battle* subject);
        
protected:
private:
    // Stores system information
    float                               background_volume_;
    float                               sound_effect_volume_;

    // Stores audio list with tag
    //std::map<int, std::string >         audio_list_;

};

} // namespace

#endif // WARRIOR_BATTLE_AUDIO_HANDLER_H_
