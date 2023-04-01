# Calendino
An arduino application that lights the appropriate LED according to the current day. This can be used to indicate the current day with physical calendars.

The physical setup uses four shift registers, 32 LEDs and an RTC module. The wiring of the shift registers is similar to the [example](https://docs.arduino.cc/tutorials/communication/guide-to-shift-out#shftout22), but on ports 3,4,2 (for the second pair) and 6,7,5 (for the first pair) instead of ports 8,12,11. The RTC module is wired as described in [this](https://arduinogetstarted.com/tutorials/arduino-rtc) tutorial.

I used an Arduino Nano, but it should work similarly with others. Instead of using one resistor per LED, I used one common resistor, as only one LED lights up at a time anyway.

Have fun!
