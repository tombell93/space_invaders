SEARCH_DIR(".")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\LIB\ARM")
INPUT("C:\Program Files (x86)\Raisonance\Ride\LIB\ARM\CircleOS\Application_Startup.o")
INPUT("Application.o")
INPUT("STM32F10x_thumb.lib")
OUTPUT(".\_tmp_.ld.elf")
/* Memory Spaces Definitions */

MEMORY
  {
  RAM (xrw)     : ORIGIN = 0x20000000, LENGTH = 54K
  FLASHB1 (r)   : ORIGIN = 0x08000000, LENGTH = 40K
  FLASH (rx)    : ORIGIN = 0x0800A000, LENGTH = 352K
  EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  }
INCLUDE "C:\Program Files (x86)\Raisonance\Ride\LIB\ARM\CircleOS\STM32Primer.ld"
