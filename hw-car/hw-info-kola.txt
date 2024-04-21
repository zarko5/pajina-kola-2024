# hardware

ADC connected with voltage dividers to the positive terminal of the battery
4.35 V -- maximum voltage, corresponding ADC reading should be 1023

## board pinout 
GPIO2 -- MPU6050 SCL -- also internally wired to the on-board LED?
GPIO0 -- MPU6050 SDA
GPIO10 -- servo1 pwm // potencijalno ce ovo da zajebava mozda, ploca treba da se flashuje sa DIO ili Dout za svaki slucaj
ADC -- voltage divider to BAT+ - 4.35 V max
GPIO4
GPIO5
GPIO14
GPIO12
GPIO13
GPIO15
Vin -- MT3068 dc/dc step up, set to abt 5.9 volts, used for the servo supply

# step ups
one mt3068 for the esp8266 and the servo, set to 5.9volts
another mt3068 for the l298N driver board and motors, the l298n is set to use
it's own 5v regulator currently and 12 volts which this mt3068 is set to output
shouldn't be exceeded without supplying separate logic voltage

the mt3068's shouldn't be set on live voltage as they will die because of some
stupid unknown to me reason, like the feedback pin of the chip losing contact
while the pot wiper is in motion and things go kaboom and bam it won't work anymore

# charging mechanism
the battery charges via a tp4056 module which also acts as additional protection 
for the circuit by cutting off output voltage if things are drawing more
than 3A i think? (for example when the mt3068 boards go haywire)
the battery currently in use also has another protection board inside itself if
the tp4056 happens to fail or if some madman tries to connect it directly
to the haywire mt3068

# motor control
the motor control is done via a l298n motor driver, which takes it's input voltage
from one step up module and knocks off i think abt 2 volts on the output to
the motors due to internal logic

this drop could be compensated for by adjusting the step up module to output
a higher voltage but then the onboard jumper would have to be removed and
the voltage from the other step up module should be supplied as logic voltage



