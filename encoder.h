#ifndef ENCODER_H_
#define ENCODER_H_

#include <EncButton.h>

namespace my {

class Encoder {
public:
    Encoder() = default;

    void Setup();
    void StartLoop();
    int GetDir();
    int GetCounter();
    bool IsPressed();
    bool IsClick();
    bool IsTurn();
    bool IsFast();
};

} // namespace

#endif // ENCODER_H_
