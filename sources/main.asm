                device  zxspectrum48
    
                org     $dd00-3
                DISPLAY "ORG: ", $

cpm:            jp      BOOT

                include "ez80.asm"
                DISPLAY "CPM: ", $
                include "cpm.asm"
                include "bios/bios.asm"

cpm_size        equ     $ - cpm

                savebin "../bootstrap/cpm.sys", cpm, cpm_size
    
