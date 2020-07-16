/*
 * FLUX CAPACITOR - Demo 4 LEDs
 *
 * Example for FluxCapacitor library
 *
 * HOW TO USE IT
 *
 * Assign one Arduino pin number to each LED according to its position.
 * All pins should be PWM.
 *
 *  +-----------------------+
 *  + FLUX CAPACITOR 4 LEDS |
 *  +-----------------------+
 *  |                       |
 *  | +(C)+           +(C)+ |
 *  |   +(B)+       +(B)+   |
 *  |     +(A)+   +(A)+     |
 *  |         +(X)+         |
 *  |         +(A)+         |
 *  |         +(B)+         |
 *  |         +(C)+         |
 *  +-----------------------+
 *  
 *  FluxCapacitor fluxCapacitor(X, A, B, C);
 */

//-----------------------------------------
// Add include to FluxCapacitor
#include <FluxCapacitor.h>

//-----------------------------------------
// Add using namespace bttf::timemachine
using namespace bttf::timemachine;

//-----------------------------------------
/** FluxCapacitor declaration
 *    - Use PWM arduino pins
 *    - First parameter is used as pin for Central led
 *    - next pins are sorted from the center outward
 */
FluxCapacitor fluxCapacitor(13, 3, 5, 6); // some Arduino MEGA PWM pins.

/**
 * Used to increase Flux Capacitor level every 5 seconds.
 */
unsigned long _previousExecutionMillis = 0;

// the setup function runs once when you press reset or power the board
void setup() {

    // initialize led
    fluxCapacitor.init();       // REQUIRED

}

// the loop function runs over and over again forever
void loop() {

    //  If flux is STOPPED then it is turned ON
    if (fluxCapacitor.state() == STOPPED) {
        fluxCapacitor.on();         // Flux capacitor ON
    }

    // Enter every 3 seconds
    if (millis() >= _previousExecutionMillis + 3000) {
        // if flux level is 8, then turn it off
        if (fluxCapacitor.level() == 8) {
            fluxCapacitor.off();    // Flux capacitor OFF
        }

        // set flux capacitor to next level
        fluxCapacitor.next();

        _previousExecutionMillis += 3000;
    }

    // Call fluxCapacior.loop() ALWAYS in the end of sketch loop.
    fluxCapacitor.loop();
}
