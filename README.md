# Crystallizer

> Author: Ratnodeep Bandyopadhyay ([@QuarterShotofEspresso](https://github.com/QuarterShotofEspresso))

> I'm still updating the README, I'm waiting for the PCB to be delivered...

## What is it?
The crystallizer is a lid for mason jars. An advanced, applications-specific lid. It allows you, the user, to grow metal crystals in metal-salt solutions.
The crystallizer itself is a tool that can deposit a current into any solution, but I've been using it to grow copper metal specifically.
Here's a rendering of what the device looks like.

![3D Rendering of Crystallizer](https://github.com/QuarterShotofEspresso/crystallizer/blob/master/pcb/current/images/3d_render.png)


TODO: Insert image with indications once PCB arrives


## How to use it
Since the crystallizer is, itself, a lid, the original lid of the mason jar can be disposed. With the mason jar open, the crystallizer can be placed on top of the jar, and the ring
re-screwed to seal the crystallizer in place.  

> NOTE: The format below is <Color> : <Purpose>. Correlate the <Color> with the colored circles of the image above.  

[Blue]: The board is powered by a 5V power supply. TODO: Put a link to the supply after you know it works.  
[Red]: Notice the seven segment display at the top of the board. This display is responsible for displaying the current, in milliamps, flowing through the solution.
The maximum current capacity of the circuit is 500mA.  
[Green]: The current can be adjusted using the potentiometer located near the bottom left of the image.  
[Orange]: The largest hole in the PCB is to refill the solvent as it will evaporate over time.  
[Purple]: Then there are two separate holes on opposite sides of the crystallizer. This is where the anode and cathode can attach to the crystallizer by alligator clips.  
[White]: These clips will connect to the board using the block connector as shown below:  

TODO: Image will be inserted once I have the PCB  


[Yellow]: The 6 row female headers are used to upload the code to the board.


## Uploading code to the board


## Mason Jar Specifications
The Mason Jar must have a 80mm diameter mouth to be compatible with the Crystallizer. The capacity of the jar is up to the experiment, but I've been using an 8oz jar.

## Warnings to using Mason Jars
Mason Jars were a great selection for my particular use case. Of course, other metals and solutions can certainly be experimented with, but there are a few concerns
when using mason jars in chemical experimentation.
Mason Jars along with most other household glass containers are made of soda lime glass. Compared to borosilicate glass which was designed for the lab,
soda lime glass is less resistant to acidic/basic solutions and, in particular, thermal shock.  
  
So keep in mind:  
Don't apply massive, sudden changes in temperature to the jar.  
Don't use solutions that are too acidic or too basic. Especially if the solution will sit in the jar for extended periods of time.  

> NOTE: I take no responsibility for any damage caused to people or property.  


## Progress with Crystals

> Note: I'll post an image of my progress with the crystals soon. I'm just waiting for the PCB to finish production.  

