#ifndef MUSIC_H_
#define MUSIC_H_

bool loadFile(std::string filename, char **buffer, int *length, WAVEFORMATEX *format);
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

#endif