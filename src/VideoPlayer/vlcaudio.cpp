#include <vlc/vlc.h>

#include "vlcaudio.h"
#include "vlcerror.h"
#include "vlcmediaplayer.h"

VlcAudio::VlcAudio(VlcMediaPlayer *player)
    : QObject(player), _vlcMediaPlayer(player->core())
{

}

VlcAudio::~VlcAudio()
{

}

bool VlcAudio::getMute() const
{
    bool mute = false;
    if(_vlcMediaPlayer)
    {
        mute = libvlc_audio_get_mute(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return mute;
}

void VlcAudio::setVolume(int volume)
{
    if(_vlcMediaPlayer)
    {
        if(volume != VlcAudio::volume())
        {
            libvlc_audio_set_volume(_vlcMediaPlayer, volume);
            VlcError::errmsg();
        }
    }
}

void VlcAudio::setTrack(int track)
{
    if(_vlcMediaPlayer)
    {
        libvlc_audio_set_track(_vlcMediaPlayer, track);
        VlcError::errmsg();
    }
}

bool VlcAudio::toggleMute() const
{
    if(_vlcMediaPlayer)
    {
        libvlc_audio_toggle_mute(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return getMute();
}

int VlcAudio::track() const
{
    int track = -1;
    if(_vlcMediaPlayer)
    {
        track = libvlc_audio_get_track(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return track;
}

int VlcAudio::trackCount() const
{
    int count = -1;
    if(_vlcMediaPlayer)
    {
        count = libvlc_audio_get_track_count(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return count;
}

QStringList VlcAudio::trackDescription() const
{
    QStringList descriptions;

    if(_vlcMediaPlayer)
    {
        libvlc_track_description_t *desc;
        desc = libvlc_audio_get_track_description(_vlcMediaPlayer);
        VlcError::errmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if(trackCount() > 1)
        {
            for (int i = 1; i < trackCount(); i++)
            {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> VlcAudio::trackIds() const
{
    QList<int> ids;

    if(_vlcMediaPlayer)
    {
        libvlc_track_description_t *desc;
        desc = libvlc_audio_get_track_description(_vlcMediaPlayer);
        VlcError::errmsg();

        ids << desc->i_id;
        if(trackCount() > 1)
        {
            for(int i = 1; i < trackCount(); i++)
            {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}

int VlcAudio::volume() const
{
    int volume = -1;
    if(_vlcMediaPlayer)
    {
        volume = libvlc_audio_get_volume(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return volume;
}
