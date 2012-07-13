
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\TOM\Desktop\SpaceInvaders\objdebug")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM\CircleOS\Application_Startup.o")
INPUT("C:\Users\TOM\Desktop\SpaceInvaders\objdebug\Application.o")
GROUP(
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\TOM\Desktop\SpaceInvaders\Circle_App.ld"
OUTPUT("C:\Users\TOM\Desktop\SpaceInvaders\objdebug\Space Invaders.elf")
