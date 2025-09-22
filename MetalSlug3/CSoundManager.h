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
    // 사운드 리소스 정보를 갖는 객체 
    unordered_map<TCHAR*, FMOD_SOUND*> m_mapSound;

    // FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
    FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

    // 사운드 ,채널 객체 및 장치를 관리하는 객체 
    FMOD_SYSTEM* m_pSystem;

};

