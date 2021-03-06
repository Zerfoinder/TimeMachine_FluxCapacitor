/**
 * ----------------------------------------------------------------
 * Copyright (C) 2020  Luis Acosta <zerfoinder@gmail.com>
 *
 * FluxCapacitor library, This library for Arduino is used to manage leds to emulate
 * Back to the Future's Flux Capacitor
 *
 * www.github.com/Zerfoinder/TimeMachine_FluxCapacitor (github as default source provider)
 *
 * This file is part of FluxCapacitor library.
 *
 * FluxCapacitor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FluxCapacitor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------
 */

/**
 *  name:         FluxCapacitor
 *  version:      0.1.1
 *  Author:       Luis Acosta <zerfoinder@gmail.com>
 *  Date:         2020-07-22
 *  Description:  FluxCapacitor is an Arduino Library to manage leds to emulate Back to the Future's Flux Capacitor.
 */

#include <Arduino.h>
#include "FluxCapacitor.h"

namespace bttf::timemachine {

    /*                                          +-----------------------------------------------------+
     *                                          |                    L  E  V  E  L  S                 |
     *                                          +-----+-----+-----+-----+-----+-----+-----+-----+-----+
     *                                          |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |
     *                                          +-----+-----+-----+-----+-----+-----+-----+-----+-----+ */
    const byte FluxCapacitor::ledBrightness[] = {   0,    5,   10,   10,   20,   50,  100,  220,  255 };
    const byte FluxCapacitor::ledBlinkSpeed[] = { 150,  150,  100,   90,   80,   70,   20,   20,   10 };

    FluxCapacitor::FluxCapacitor(byte pinNumberCentral, byte pinNumberA, byte pinNumberB, byte pinNumberC)
        : _pins( {
        EasyPin(pinNumberCentral, OUT),
        EasyPin(pinNumberA, OUT),
        EasyPin(pinNumberB, OUT),
        EasyPin(pinNumberC, OUT),
        EasyPin(pinNumberC, OUT) }) {
        _fluxType = FOUR_LEDS;
        _level = 0;
        _state = STOPPED;
        _leds = 4;
    }

    FluxCapacitor::FluxCapacitor(byte pinNumberCentral, byte pinNumberA, byte pinNumberB, byte pinNumberC, byte pinNumberD)
        : _pins {
        EasyPin(pinNumberCentral, OUT),
        EasyPin(pinNumberA, OUT),
        EasyPin(pinNumberB, OUT),
        EasyPin(pinNumberC, OUT),
        EasyPin(pinNumberD, OUT) } {
        _fluxType = FIVE_LEDS;
        _level = 0;
        _state = STOPPED;
        _leds = 5;
    }

    void FluxCapacitor::init(void) {
        for (int i = 0; i < _leds; i++) {
            _pins[i].init();
        }
    }

    void FluxCapacitor::on(void) {
        _lastLoopMillis = millis();
        _state = RUNNING;
        _level = 1;
        _ledIterator = _leds - 1;
        _pins[_ledIterator].setValue(ledBrightness[_level]);
    }

    void FluxCapacitor::off(void) {
        _lastLoopMillis = millis();
        _state = STOPPED;
        _level = 0;
        _ledIterator = 0;
        for (int i = 0; i < _leds; i++) {
            _pins[i].off();
        }
        _centerLedBrightness = 0;
    }

    void FluxCapacitor::flash(void) {
        _lastLoopMillis = millis();
        if (_flashingMillis == 0) {
            _flashingMillis = defaultFlashingDuration;
        }

        _backupState = _state;
        _state = FLASHING;

        for (int i = 0; i < _leds; i++) {
            _pins[i].on();
        }
    }

    void FluxCapacitor::flash(unsigned long duration) {
        _flashingMillis = duration;
        flash();
    }

    void FluxCapacitor::setLevel(byte level) {
        _level = level;
    }

    void FluxCapacitor::next(void) {
        if (_level > 0) {
            _level++;

            if (_level > MAX_LEVEL) {
                _level = MAX_LEVEL;
            }
        }
    }

    byte FluxCapacitor::level(void) {
        return _level;
    }

    int FluxCapacitor::leds(void) {
        return _leds;
    }

    FluxState FluxCapacitor::state(void) {
        return _state;
    }

    void FluxCapacitor::loop(void) {
        if (_state == RUNNING ) {
            if (millis() >= _lastLoopMillis + ledBlinkSpeed[_level]) {
                _lastLoopMillis += ledBlinkSpeed[_level];

                _update();
            }
        }
        if (_state == FLASHING ) {
            if (millis() >= _lastLoopMillis + _flashingMillis) {
                _lastLoopMillis += _flashingMillis;

                _state = _backupState;
                _backupState = STOPPED;
                _flashingMillis = defaultFlashingDuration;
            }
        }
    }

    void FluxCapacitor::_update(void) {
        _nextLed();

        // loop all LEDs except middle one
        for (int i = 1; i < _leds; i++) {
            if (_ledIterator == i) {
                _pins[i].setValue(ledBrightness[_level]);
            } else {
                _pins[i].off();
            }

            // ---------- CENTRAL LED BLINKING ----------
            if (_ledIterator == 1) {
                _setCenterLedBrightness();

                if (_level > 4 && _level < MAX_LEVEL) {
                    _pins[0].setValue(_centerLedBrightness);
                }

                if (_level == (MAX_LEVEL)) {
                    _pins[0].setValue((rand() % 255) + 235);
                    _centerLedBrightness = 255;
                }

                if (_level == MAX_LEVEL && (rand() % 3) == 1) {
                    _pins[0].setValue(10);
                }
            }
            _centerLedBrightness -= 5;
            if (_centerLedBrightness < 0)
                _centerLedBrightness = 0;
        }
    }

    void FluxCapacitor::_nextLed(void) {
        // moves led iterator from A, B, C(, D).
        _ledIterator--;

        // if iterator is A then return to last led (D).
        if (_ledIterator == 0) {
            _ledIterator = _leds - 1;
        }
    }

    void FluxCapacitor::_setCenterLedBrightness(void) {
        if (_level > 4 & (rand() % (_level * 2)) > MAX_LEVEL) {
            int inicial = (_level - 5) * 35;
            _centerLedBrightness = _centerLedBrightness + (rand() % 35 + inicial);
        }

        if (_centerLedBrightness > 255)
            _centerLedBrightness = 255;
    }

} // end namespace bttf::timemachine
