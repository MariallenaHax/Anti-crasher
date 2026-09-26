#pragma once
#include "xaudio2.h"

class AudioUtils
{
public:
    static inline IXAudio2* pXAudio2 = nullptr;
    static inline IXAudio2MasteringVoice* pMasterVoice = nullptr;
    static inline IXAudio2SourceVoice* pSourceVoice = nullptr;

    static inline std::vector<float> VolumeLevels;
    static inline std::mutex VolumeMutex;

    static std::wstring Utf8ToWide(const std::string& utf8Str) {
        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
        if (sizeNeeded == 0) return L"";
        std::wstring wstr(sizeNeeded, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], sizeNeeded);
        if (!wstr.empty() && wstr.back() == L'\0') wstr.pop_back();
        return wstr;
    }

    static void PlayFromMC(std::string Name, float volume, float pitch)
    {
        if (!Address::getMinecraftGame())
        {
            return;
        }

        //Address::getMinecraftGame()->playUI(Name, volume, pitch);
        Address::getClientInstance()->playUI(Name, volume, pitch);
    }

    static void Play(std::string path, float volume, bool loop, float startpos) {
        if (pXAudio2 == nullptr)
            XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

        if (pMasterVoice == nullptr)
            pXAudio2->CreateMasteringVoice(&pMasterVoice);

        WAVEFORMATEXTENSIBLE wfx = { 0 };
        XAUDIO2_BUFFER buffer = { 0 };

        TCHAR* strFileName = new TCHAR[path.size() + 1];
        strFileName[path.size()] = 0;
        std::copy(path.begin(), path.end(), strFileName);

        std::wstring widePath = Utf8ToWide(path);
        HANDLE hFile = CreateFileW(widePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return;

        DWORD dwChunkSize, dwChunkPosition;

        auto findChunk = [&](DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) -> HRESULT {
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
                return HRESULT_FROM_WIN32(GetLastError());
            DWORD dwChunkType, dwChunkDataSize, dwRIFFDataSize = 0, dwFileType, bytesRead = 0, dwOffset = 0;
            while (true) {
                DWORD dwRead;
                if (!ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) return HRESULT_FROM_WIN32(GetLastError());
                if (!ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) return HRESULT_FROM_WIN32(GetLastError());
                switch (dwChunkType) {
                case 'FFIR':
                    dwRIFFDataSize = dwChunkDataSize;
                    dwChunkDataSize = 4;
                    if (!ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) return HRESULT_FROM_WIN32(GetLastError());
                    break;
                default:
                    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                        return HRESULT_FROM_WIN32(GetLastError());
                }
                dwOffset += sizeof(DWORD) * 2;
                if (dwChunkType == fourcc) {
                    dwChunkSize = dwChunkDataSize;
                    dwChunkDataPosition = dwOffset;
                    return S_OK;
                }
                dwOffset += dwChunkDataSize;
                if (bytesRead >= dwRIFFDataSize) return S_FALSE;
            }
            };

        auto readChunkData = [&](void* buffer, DWORD size, DWORD offset) -> HRESULT {
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, offset, NULL, FILE_BEGIN))
                return HRESULT_FROM_WIN32(GetLastError());
            DWORD dwRead;
            if (!ReadFile(hFile, buffer, size, &dwRead, NULL))
                return HRESULT_FROM_WIN32(GetLastError());
            return S_OK;
            };

        findChunk('FFIR', dwChunkSize, dwChunkPosition);
        DWORD filetype;
        readChunkData(&filetype, sizeof(DWORD), dwChunkPosition);
        if (filetype != 'EVAW') return;

        findChunk(' tmf', dwChunkSize, dwChunkPosition);
        readChunkData(&wfx, dwChunkSize, dwChunkPosition);

        findChunk('atad', dwChunkSize, dwChunkPosition);
        BYTE* pDataBuffer = new BYTE[dwChunkSize];
        readChunkData(pDataBuffer, dwChunkSize, dwChunkPosition);

        buffer.AudioBytes = dwChunkSize;
        buffer.pAudioData = pDataBuffer;
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        if (loop) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

        if (pSourceVoice) {
            pSourceVoice->DestroyVoice();
            pSourceVoice = nullptr;
        }

        pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);
        pSourceVoice->SubmitSourceBuffer(&buffer);
        pSourceVoice->SetVolume(volume);
        pSourceVoice->Start(startpos);

        delete[] strFileName;
    }
};
