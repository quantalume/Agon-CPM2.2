; CP/M function jump table (starts at $40050)
; Called from sources/bios/gate.asm -- tables must match

                jp      _cpm_restore

; Device functions in devices.inc
                jp      const
                jp      conin
                jp      conout
                jp      list
                jp      punch
                jp      reader

; Disk functions in drive_emu.inc
                jp      home
                jp      seldsk
                jp      settrk
                jp      setsec
                jp      setdma
                jp      read
                jp      write

; Misc functions
                jp      listst          ; See devices.inc
