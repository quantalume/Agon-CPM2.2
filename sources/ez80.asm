; eZ80 memory modes that Z80 assembler doesn't understand

                macro   LIL             ; ADL mode for both data and control
                db      $5b             ; Follow with page number, e.g., "db $4"
                endm

                macro   LIS             ; ADL mode for data, Z80 mode for control
                db      $49
                endm
