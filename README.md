# PIC16 Serial I2C Bridge

I wrote a small bidirectional Serial to I2C bridge for the very first version of my Bluetooth iPod Board, but I ended up not needing it because I was going about the project in a very complicated way.

This is specifically written for a PIC16F15213 - it is not extensively tested, but it seems to work well enough. This is not intended to be used as-is since it has some of my project-specific code in here (very little, but still), but it's a good starting point for making your own Serial I2C bridge out of a cheap family of microcontrollers.

The MPLAB X Project name is still iPodBtCommBridge, I'm not sure if your install of MPLAB will like trying to import this. But there are very few files here, you should just copy and paste them into a new project if you really want to use this code.

## How It Works

This uses two ring buffers - one for each direction of communication. I2C uses interrupts to send and receive data. The Serial uses polling to send and receive data in the main loop.

### License

MIT