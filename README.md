# Fireflies

Firefly PCBs powered by the CH32V003.

These boards periodically flashes a xenon flash based on a timer, which gets accelerated when it detects flashes from surrounding PCBs.

There are currently 2 flash-detection circuits undergoing testing:

1. A reverse-biased diode using the ch32v003's internal OPA & ADC for sampling

2. A I2C ambiant light sensor for more "advanced" applications

The flash trigger isolates the hi-voltage trigger and the low-voltage circuitarry with a triac circuit, and uses a mosfet to trigger the flash.

