# Crystallizer

> Author: Ratnodeep Bandyopadhyay ([@QuarterShotofEspresso](https://github.com/QuarterShotofEspresso))

## What is it?
The crystallizer is a lid for mason jars; however, it can deposit up to 500mA of current through a solution within the jar.
I've been using it to grow copper metal specifically.
A rendering of what the device looks like.

![3D Rendering of Crystallizer](https://github.com/QuarterShotofEspresso/crystallizer/blob/master/pcb/current/images/3d_render.png)



## How to use it
Since the crystallizer is, itself, a lid, the original lid of the mason jar can be disposed. With the mason jar open, the crystallizer can be placed on top of the jar, and the ring
re-screwed to seal the crystallizer in place.  

![Diagram](https://github.com/QuarterShotofEspresso/crystallizer/blob/master/pcb/current/images/diagram.png)

[Red]: The board is powered by a 5V power supply.  
[Orange]: The 7-segment display is responsible for displaying the current, in milliamps, running through the solution.
The maximum current capacity of the circuit is 500mA.  
[Purple]: The current can be adjusted using the potentiometer located near the bottom left of the image.  
[Light Blue]: The largest hole in the PCB is to refill the solvent as it will evaporate over time.  
[Green]: Two separate holes exist on opposite sides of the Crystallizer to serve as locations where the anode and cathode dip into the solution.  
[White]: Two clips connect to the board using the block connector.   
[Yellow]: The 6 row female headers are used to flash the microcontroller.


## Uploading code to the board
To upload code to the board, an atmel_isp progammer was used. The makefile is setup with the atmel_isp board.
If the board used to flash the mcu is different, change the `PROGRAMMER` variable in the Makefile to match the appropriate programmer.
Navigate to the code directory. In the directory, run `./configure`. This only needs to be run the first time.
Run `make && make flash` to compile and flash the program.  

## Mason Jar Specifications
The Mason Jar must have an 80mm diameter mouth to be compatible with the Crystallizer. The capacity of the jar is up to the experiment, but I've been using an 8oz jar.

## Warnings to using Mason Jars
Mason Jars were a great selection for my particular use case. Of course, other metals and solutions can certainly be experimented with, but there are a few concerns
when using mason jars in chemical experimentation.
Mason Jars along with most other household glass containers are made of soda lime glass. Compared to borosilicate glass, which was designed for the lab,
soda lime glass is less resistant to acidic/basic solutions and thermal shock.  
  
PLEASE KEEP IN MIND:  
DO NOT apply massive, sudden changes in temperature to the jar.  
DO NOT use solutions that are too acidic or too basic.  
Running electricity through any solution causes a chemical reaction. Ensure the byproducts will NOT dissolve the glass.

> NOTE: I take no responsibility for any damage caused to people or property.  

