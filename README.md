# SMT172-Library for the ATmega microcontroller family 
The SMT172 is a ultra-low power, high accuracy temperature sensor. The output
signal is pulse width modulated (PWM) and the duty cycle is a function of
the measured temperature. For more details see the home page of the
manufacturer: http://smartec-sensors.com/ and the documentation.
Accuracy can be up to 0.1 degree Celsius for a limited temperature range and
the resolution up to 0.001 degree Celsius.

With a 16MHz CPU and a conversion time of 30 ms a resolution of about
0.01 degree Celsius can be reached.

This library uses Timer1, Timer4 or Timer5 and ICP1, ICP4 or ICP5. 

The SMT172 needs a clean powersupply (a 100uF capacitor with a 100 nF ceramic
capacitor in parallel fed through a small signal diode), a 220 ohm resistor in
series and a 100nF ceramic capacitor as close to the sensor as possible.
See also the diagram in the extras folder.

