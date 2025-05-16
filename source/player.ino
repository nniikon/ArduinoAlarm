#include "display.h"

#include <DFRobotDFPlayerMini.h>

#include <SoftwareSerial.h>

namespace {
    SoftwareSerial softSerial(/*rx =*/10, /*tx =*/11);
    DFRobotDFPlayerMini myDFPlayer;
}

namespace {
    const uint16_t DEFAULT_VOLUME = 20;
}

void my::Player::Setup() {
    softSerial.begin(9600);

    if (!myDFPlayer.begin(softSerial, true, true)) {
        Serial.println("Unable to setup DFPlayer!");
    }
    else {
        Serial.println("DFPlayer Mini online.");
    }

    player.SetVolume(DEFAULT_VOLUME);
}

void my::Player::SetVolume(int volume) {
    myDFPlayer.volume(volume);
}

void my::Player::Play(int id) {
    myDFPlayer.playMp3Folder(1);
    myDFPlayer.enableLoop();
}

void my::Player::Stop() {
    myDFPlayer.stop();
}
