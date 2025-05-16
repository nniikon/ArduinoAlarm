#ifndef DISPLAY_H_
#define DISPLAY_H_

namespace my {

class Display {
public:
    Display() = default;

    void Setup();
    void Print(const char* str);
    void Clear();
};

}

#endif // DISPLAY_H_
