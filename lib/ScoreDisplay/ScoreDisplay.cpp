#include <Arduino.h>
#include <ScoreDisplay.h>
#include <TM1637Display.h>
#include <Config.h>

ScoreDisplay::ScoreDisplay(): display(SCORE_CLK, SCORE_DIO) {};

void ScoreDisplay::setup() {
    display.setBrightness(0x0f);
    display.showNumberDec(0, true);
}

void ScoreDisplay::set(byte number) {
    display.showNumberDec(number, true);
}

void ScoreDisplay::update(byte score) {
    display.showNumberDec(score);
}