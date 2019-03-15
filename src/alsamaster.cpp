#include "alsamaster.h"

AlsaMaster :: AlsaMaster()
{

}
int AlsaMaster :: SetAlsaMasterVolume(long volume)
{

    long min, max;
    int ret;
    snd_mixer_t *handle;    
    snd_mixer_selem_id_t *sid;

    const char *card ="default";
    const char *selem_name ="Capture";
    ret=snd_mixer_open(&handle,0);
    if(ret<0)
    {
        printf("Failed to open\n");
        return ret;
    }
    ret=snd_mixer_attach(handle,card);
    if(ret<0)
    {
        printf("Failed to attach\n");
        return ret;
    }
    ret=snd_mixer_selem_register(handle, NULL, NULL);
    if(ret<0)
    {
        printf("Failed to register\n");
        return ret;
    }
    ret=snd_mixer_load(handle);
    if(ret<0)
    {
        printf("Failed to load\n");
        return ret;
    }
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);    
    ret=snd_mixer_selem_get_capture_volume_range(elem, &min, &max);
    if(ret<0)
    {
        printf("Failed to get volume range\n");
        return ret;
    }
    ret=snd_mixer_selem_set_capture_volume_all(elem, volume *max/ 100);
    if(ret<0)
    {
        printf("Failed to set capture volume\n");
        return ret;
    }
    ret=snd_mixer_close(handle);
    if(ret<0)
    {
        printf("Failed to close\n");
        return ret;
    }

}
int AlsaMaster:: SetAlsaMasterMute()
{
    long min, max;
    int ret;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Capture";
    ret=snd_mixer_open(&handle, 0);
    if(ret<0)
    {
        printf("Failed to open\n");
        return ret;
    }
    ret=snd_mixer_attach(handle, card);
    if(ret<0)
    {
        printf("Failed to attach\n");
        return ret;
    }
    ret=snd_mixer_selem_register(handle, NULL, NULL);
    if(ret<0)
    {
        printf("Failed to register\n");
        return ret;
    }
    ret=snd_mixer_load(handle);
    if(ret<0)
    {
        printf("Failed to load\n");
        return ret;
    }
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    if (snd_mixer_selem_has_capture_switch(elem)) {
        ret=snd_mixer_selem_set_capture_switch_all(elem, 0);
        if(ret<0)
        {
            printf("Failed to set capture switch\n");
            return ret;
        }
    }
    ret=snd_mixer_close(handle);
    if(ret<0)
    {
        printf("Failed to close\n");
        return ret;
    }
}

int AlsaMaster :: GetAlsaMasterVolume()
{
    long value;
    int ret;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Capture";
    ret=snd_mixer_open(&handle, 0);
    if(ret<0)
    {
        printf("Failed to open\n");
        return ret;
    }
    ret=snd_mixer_attach(handle, card);
    if(ret<0)
    {
        printf("Failed to attach\n");
        return ret;
    }
    ret=snd_mixer_selem_register(handle, NULL, NULL);
    if(ret<0)
    {
        printf("Failed to register\n");
        return ret;
    }
    ret=snd_mixer_load(handle);
    if(ret<0)
    {
        printf("Failed to load\n");
        return ret;
    }

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);
    ret=snd_mixer_selem_get_capture_volume(elem,SND_MIXER_SCHN_FRONT_LEFT,&value);
    if(ret<0)
    {
        printf("Failed to get capture volume\n");
        return ret;
    }
    return value;
}
int AlsaMaster:: SetAlsaMasterUnMute()
{
    snd_mixer_t *handle;
    int ret;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Capture";
    ret=snd_mixer_open(&handle, 0);
    if(ret<0)
    {
        printf("Failed to open\n");
        return ret;
    }
    ret=snd_mixer_attach(handle, card);
    if(ret<0)
    {
        printf("Failed to attach\n");
        return ret;
    }
    ret=snd_mixer_selem_register(handle, NULL, NULL);
    if(ret<0)
    {
        printf("Failed to register\n");
        return ret;
    }
    ret=snd_mixer_load(handle);
    if(ret<0)
    {
        printf("Failed to load\n");
        return ret;
    }

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    if (snd_mixer_selem_has_capture_switch(elem)) {
        ret=snd_mixer_selem_set_capture_switch_all(elem, 1);
        if(ret<0)
        {
            printf("Failed to set capture switch\n");
            return ret;
        }
    }
    ret=snd_mixer_close(handle);
    if(ret<0)
    {
        printf("Failed to close\n");
        return ret;
    }
}
