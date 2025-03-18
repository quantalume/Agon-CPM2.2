;; CP/M loader and MOS calls wrapper
;; (c) 2023 Aleksandr Sharikhin

;; Constants
mos_getkey:     equ     $00
mos_load:       equ     $01
mos_sysvars:    equ     $08
mos_fopen:      equ     $0a
mos_fclose:     equ     $0b
mos_fgetc:      equ     $0c
mos_fputc:      equ     $0d
mos_feof:       equ     $0e
mos_setint:     equ     $14
mos_uopen:      equ     $15
mos_uclose:     equ     $16
mos_ugetc:      equ     $17
mos_uputc:      equ     $18
mos_fread:      equ     $1a
mos_fwrite:     equ     $1b
mos_flseek:     equ     $1c

;; File modes
fa_read:        equ     $01
fa_write:       equ     $02
fa_exist:       equ     $00
fa_create:      equ     $04
fa_cr_always:   equ     $08
fa_op_always:   equ     $10
fa_append:      equ     $30
CPM_SEG:        equ     $5

                macro   MOSCALL func
                ld      a, func
                rst.lil $08
                endmacro

                ASSUME ADL=1

;; USE org $40000 for usual application
;; or $B0000 for MOS command
                org     $40000
                jp      _start

; MOS command file header
                align   64              ; Start at $40
                db      "MOS"
                db      00              ; Version
                db      01              ; ADL

                align   16              ; Leave room for future MOS header spec
                include "bios_gate.inc" ; BIOS gate will start at $50
                include "devices.inc"
                include "drive_emu.inc"

_start:
                push    ix
                push    iy

;               ld      a, mos_sysvars
;               rst.lil $08

;               xor     a
;               ld      (ix+$28), a

                call    _main
                pop     iy
                pop     ix

                ld      hl, 0
                ret

sys_ix:         dl      0
sys_iy:         dl      0

_main:
                ld      (sys_ix), ix
                ld      (sys_iy), iy

                xor     a
                ld      mb, a

                call    fs_init
                and     a
                jp      z, _error

                call    _device_init

                call.lil _cpm_restore

                ld      hl, _autoexec
                ld      de, CPM_SEG * $10000 + $100
                ld      bc, _autoexec_end - _autoexec
                ldir
	
                ld      a, CPM_SEG
                ld      mb, a
                stmix
                jp.sis  $DCFD

_error:
                xor     a
                ld      mb, a

                ld      c, 0
                MOSCALL mos_fclose

                ld      hl, @msg
                xor     a
                ld      bc, 0
                rst.lil $18
                ret
@msg:
                db      "Error initing OS", 13, 10
                db      "Check system files and try again", 13, 10, 0

_cpm_restore:
                ld      hl, _cpm_image
                ld      de, $5dcfd
                ld      bc, _cpm_end - _cpm_image
                ldir
                ret.lil

_cpm_image:
                incbin  "cpm.sys"
_cpm_end:

_autoexec:
                incbin  "../autoexec/autoexec.com"
_autoexec_end:
