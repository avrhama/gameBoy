# gameBoy
gameboy emulator written in cpp.

Its all start when I wrote my first console emulator 8-chip, I was fascinated that I make it work but not satisfied with the result.
So instead investing my time to improve 8-chip I decide to write gameboy emultor.

The most difficult part in this project was to find an reliable and understantable(for me) gameboy documentation, unfortunately
I did not found it in one place, but fortunately for each component in the gameboy emultor(cpu,gpu,apu and etc)
I could find a bunch of good documentations, so I forge them together to create my emulator.

Here is the place to give them their credits:
The main resource I used, mainly for cpu instructions but also for other things.
  https://gbdev.gg8.se/wiki/articles/Pan_Docs
Gpu:
  http://www.codeslinger.co.uk/pages/projects/gameboy/beginning.html
Apu:
  http://belogic.com/gba/channel1.shtml
Cpu instruction and other:
  http://www.codeslinger.co.uk/pages/projects/gameboy/files/GB.pdf
I used this openSource gameboy emulator, for debugging, not by looking into the code but by pipeline data between
This emulator and my emulator, perform comparation and investigate the section where the compare is false:
  https://github.com/Humpheh/goboy
  
 And the last credit is for the javidx9 from oneLoneCoder who inspire me in the first place.
 His youtube channel:
  https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA
 His nes emulator playlist:
  https://www.youtube.com/watch?v=nViZg02IMQo&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
