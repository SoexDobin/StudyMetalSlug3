#pragma once
#include "CSingleTon.h"
#include "Define.h"

class CSoundManager final : public CSingleTon<CSoundManager>
{
    friend class CSingleTon;
private:
    CSoundManager();
    ~CSoundManager() override;

public:
    void        Initialize();
    void        Release();

    void        PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
    void        PlayBGM(const TCHAR* pSoundKey, float fVolume);
    void        StopSound(CHANNELID eID);
    void        StopAll();
    void        SetChannelVolume(CHANNELID eID, float fVolume);

private:
    void        LoadSoundFile();

private:
    unordered_map<TCHAR*, FMOD_SOUND*> m_umapSound;
    FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
    FMOD_SYSTEM* m_pSystem;
};

