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

#ifndef TIMEMACHINE_FLUX_CAPACITOR_H
#define TIMEMACHINE_FLUX_CAPACITOR_H

#include <EasyPin.h>

using namespace zsoft::io::utilities;

namespace bttf::timemachine {

    /**
     * @brief      FluxType is an enum to define number of leds
     */
    enum FluxType {
        FIVE_LEDS = 0,   //  FIVE LEDS FLUX CAPACITOR
        FOUR_LEDS = 1,   //  FOUR LEDS FLUX CAPACITOR
        //ONE_LED = 2      //  ONE LED FLUX CAPACITOR
    };

    /** FluxType is an enum to define pin mode. */

    /**
     * @brief      FluxState indicates the current state of Flux.
     */
    enum FluxState {
        STOPPED = 0,   //  Flux Capacitor is OFF (All LEDs OFF).
        RUNNING = 1,   //  Flux Capacitor is running a sequence.
        FLASHING = 2   //  Flux Capacitor is flashing.
    };

    /**
     * @brief      This class describes a flux capacitor.
     */
    class FluxCapacitor {

        /** -------------- CONSTANTS --------------- */
        /**
         * brightness level array
         */
        static const byte ledBrightness[];

        /**
         * blink speed level array
         */
        static const byte ledBlinkSpeed[];

        /**
         * Flux Capacitor MAX_LEVEL to reach.
         */
        static const byte MAX_LEVEL = 8;

        /**
         * Flashing duration milliseconds
         */
        static const unsigned long defaultFlashingDuration = 200;

      public:

        // /**
        //  * @brief      Constructs a new instance.
        //  *
        //  * @param[in]  pinNumber  The pin number in Arduino board.
        //  */
        //FluxCapacitor(byte pinNumber);

        /**
         * @brief      Constructs a new instance for 4 LEDs.
         *
         * @param[in]  pinNumberCentral  The Arduino pin number for LED Central
         * @param[in]  pinNumberA        The Arduino pin number for LED A
         * @param[in]  pinNumberB        The Arduino pin number for LED B
         * @param[in]  pinNumberC        The Arduino pin number for LED C
         */
        FluxCapacitor(byte pinNumberCentral, byte pinNumberA, byte pinNumberB, byte pinNumberC);

        /**
         * @brief      Constructs a new instance for 5 LEDs.
         *
         * @param[in]  pinNumberCentral  The Arduino pin number for LED Central
         * @param[in]  pinNumberA        The Arduino pin number for LED A
         * @param[in]  pinNumberB        The Arduino pin number for LED B
         * @param[in]  pinNumberC        The Arduino pin number for LED C
         * @param[in]  pinNumberD        The Arduino pin number for LED D
         */
        FluxCapacitor(byte pinNumberCentral, byte pinNumberA, byte pinNumberB, byte pinNumberC, byte pinNumberD);

        /**
         * @brief      Initializes the object.
         * @warning    It should ALWAYS run in setup() section.
         */
        void init(void);

        /**
         * @brief      Turns on Flux Capacitor.
         * @
         */
        void on(void);

        /**
         * @brief      Turns off Flux Capacitor.
         */
        void off(void);

        /**
         * @brief      All Flux Capacitor flash for default milliseconds.
         *
         * @note       All LEDs in flux capacitor turn on at the sametime for a short period.
         */
        void flash(void);

        /**
         * @brief      All Flux Capacitor flash for N milliseconds.
         *
         * @param[in]  duration  Milliseconds flashing duration.
         */
        void flash(unsigned long duration);

        /**
         * @brief      Set Flux Capacitor level.
         *
         * @param[in]  level  Level to set. (0-8).
         */
        void setLevel(byte level);

        /**
         * @brief      Set Flux Capacitor to next level.
         */
        void next(void);

        /**
         * @brief      Returns the current Flux Capacitor level.
         *             
         * @return     The current Flux Capacitor level.
         */
        byte level(void);

        /**
         * @brief      Number of LEDs in Flux Capacitor.
         *
         * @return     The number of LEDs.
         */
        int leds(void);

        /**
         * @brief      Get the current state of Flux Capacitor.
         *
         * @return     STOPPED, RUNNING or FLASHING.
         * @see        [FluxState]
         */
        FluxState state(void);

        /**
         * @brief      function used to manage concurrent actions
         *
         * @warning    it should be ALWAYS added in the end of sketch loop() method.
         */
        void loop(void);

      protected:

      private:
        /************ METHODS ***************/

        /**
         * @brief      Updates FluxCapacitor state.
         */
        void _update(void);

        /**
         * @brief      Increase LED iterator (_ledIterator)
         */
        void _nextLed(void);

        /**
         * @brief      Sets the center led brightness.
         */
        void _setCenterLedBrightness(void);


        /************ PROPERTIES ***************/

        /**
         * Flux Type
         */
        FluxType _fluxType;

        /**
         * Number of LEDs
         */
        int _leds;

        /**
         * EasyPin array
         */
        EasyPin _pins[5];

        /************ VARIABLES ***************/
        /**
         * Flux Capacitor level
         */
        byte _level;

        /**
         * Flux Capacitor state
         */
        FluxState _state;

        /**
         * Led Iterator for LEDs array
         */
        byte _ledIterator;

        /**
         * local variable used to save last loop millis()
         */
        unsigned long _lastLoopMillis = 0;

        /**
         * Variables used for flashing
         */
        unsigned long _flashingMillis = 0;
        FluxState _backupState = STOPPED;

        /**
         * local variable for center LED brightness
         */
        int _centerLedBrightness = 0;

    }; // end class FluxCapacitor

} // end namespace bttf::timemachine

#endif // TIMEMACHINE_FLUX_CAPACITOR_H
