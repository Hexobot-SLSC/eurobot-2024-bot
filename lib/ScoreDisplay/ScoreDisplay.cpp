#include <Arduino.h>
#include <ScoreDisplay.h>
#include <TM1637Display.h>
#include <Config.h>

TM1637Display ScoreDisplay::display(SCORE_CLK, SCORE_DIO);

ScoreDisplay::ScoreDisplay() {}

void ScoreDisplay::setup() {
    const uint8_t middleBar[1] = {SEG_G};
    display.setBrightness(0x0f);
    display.setSegments(middleBar, 4, 0);
}

void ScoreDisplay::update(byte score) {
    display.showNumberDec(score);
}