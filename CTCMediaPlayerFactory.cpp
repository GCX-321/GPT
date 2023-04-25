/* 
 * Copyright (C) 2019 Amlogic, Inc. All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#define LOG_TAG "CTCMediaPlayerFactory"
#define LOG_NDEBUG  0

#include <MediaPlayerFactory.h>
#include <media/ammediaplayerext.h>
#include <cutils/properties.h>
#include "CTCMediaPlayer.h"

namespace android {
class CTCMediaPlayerFactory : public MediaPlayerFactory::IFactory
{
public:
    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const char* url,
                               float curScore) {
        if (isEnabled()) {
            return kOurScore;
        }

        return 0.0;
	}

	virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                                   int /*fd*/,
                                   int64_t /*offset*/,
                                   int64_t /*length*/,
                                   float /*curScore*/) {
        if (isEnabled()) {
            return kOurScore;
        }

        return 0.0;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const sp<IStreamSource>& /*source*/,
                               float /*curScore*/) {

        return 0.0;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const sp<DataSource>& /*source*/,
                               float /*curScore*/) {
        // Only NuPlayer supports setting a DataSource source directly.
        return 0.0;   
    }

    virtual sp<MediaPlayerBase> createPlayer(pid_t pid) {
        ALOGV(" create CTCMediaPlayer, pid = %d", pid);
        return new CTCMediaPlayer(pid);
    }

private:
    bool isEnabled() const {
        return property_get_bool("media.ctcmediaplayer.enable", false);
    }

    static constexpr float kOurScore = 2.0;
};

int AmlogicCTCMediaPlayerFactoryInit(void) //_ZN7android32AmlogicCTCMediaPlayerFactoryInitEv
{

    status_t err = OK;

    err = MediaPlayerFactory::registerFactory(new CTCMediaPlayerFactory,
            (player_type)CTC_MEDIAPLAYER);

    ALOGI("register CTCMediaPlayerFactory err = %d", err);

    return err;
}

}
