# Calendino
An arduino application that lights the appropriate LED according to the current day. This can be used to indicate the current day with physical calendars.

The physical setup uses four shift registers, 32 LEDs, one OLED (Sainsmart SPI 1.3) and an RTC module. The wiring of the shift registers is similar to the [example](https://docs.arduino.cc/tutorials/communication/guide-to-shift-out#shftout22), but on ports 3,4,2 (for the second pair) and 6,7,5 (for the first pair) instead of ports 8,12,11. The OLED is wired similar to [this](https://forum.arduino.cc/t/problem-with-sainsmart-spi-1-3-oled/274912/4), but using 8 instead of 6 and 10 instead of 7. The RTC module is wired as described in [this](https://arduinogetstarted.com/tutorials/arduino-rtc) tutorial.

I used an Arduino Nano, but it should work similarly with others. Instead of using one resistor per LED, I used one common resistor, as only one LED lights up at a time anyway.

Have fun!

![photo_5294223224923605948_y](https://user-images.githubusercontent.com/13370101/230446326-89a51c52-debb-4777-8d27-a83ece6ded83.jpg)
