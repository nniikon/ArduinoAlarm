#ifndef PLAYER_H_
#define PLAYER_H_

namespace my {

class Player {
public:
    Player() = default;

    void Setup();
    void SetVolume(int volume);
    void Play(int id);
    void Stop();
};

}

#endif // PLAYER_H_
