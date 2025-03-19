IOBYTE:         EQU     $50003          ; Location of IOBYTE
UART0_THR:      EQU     $C0             ; UART0 transmit holding register
UART0_RBR:      EQU     $C0             ; UART0 receive buffer register
UART0_IER:      EQU     $C1             ; UART0 interrupt enable register
UART0_FTC:      EQU     $C2             ; UART0 FIFO control register
UART0_LSR:      EQU     $C5             ; UART0 line status register

UART1_THR:      EQU     $D0             ; UART1 transmit holding register
UART1_RBR:      EQU     $D0             ; UART1 receive buffer register
UART1_LSR:      EQU     $D5             ; UART1 line status register

; UART bitmasks
UART_LSR_TEMT:  EQU    $40             ; Transmit empty
UART_LSR_DR:    EQU    $01             ; Receive data ready

PD_DR:          EQU    $A2             ; Port D data register

;; Init devices

_device_init:

;; Reading serial configuration
                ld      hl, config_file
                ld      de, serial_cfg
                ld      bc, serial_cfg_end - serial_cfg
                MOSCALL mos_load

                ld      hl, vdu_init
                ld      bc, init_end - vdu_init
                rst.lil $18

                ld      ix, serial_cfg
                MOSCALL mos_uopen

                ld      a, 7
                out0    (UART0_FTC), a  ; Disable UART0 FIFO

                xor     a
                out0    (UART0_IER), a  ; Disable UART0 interrupts

@wait:
                in0     a, (UART0_LSR)
                and     UART_LSR_DR
                jr      z, @wait

                in0     a, (UART0_RBR)
                ret


config_file:
                db      "/mos/zinc.cfg", 0

serial_cfg:
                dl      57600
                db      8
                db      1
                db      0
                db      0
                db      0
serial_cfg_end:

vdu_init:
                db      23, 0, 255      ; Switch to terminal emulation
                db      "CP/M to MOS gate v.1.2", 13, 10
                db      "2025 (c) Aleksandr Sharikhin", 13, 10
                db      13,10
init_end:

const:          ld      a, (IOBYTE)
                and     3
                jp      z, tty_status

                in0     a, (UART0_LSR)
                and     UART_LSR_DR
                ret.lil z
                ld      a, $ff
                ret.lil

tty_status:     in0     a, (UART1_LSR)
                and     UART_LSR_DR
                ret.lil z
                ld      a, $ff
                or      a
                ret.lil

conin:          ld      a, (IOBYTE)
                and     3
                jp      z, tty_in

crt_in:         in0     a, (UART0_LSR)  ; wait for character from VDP
                and     UART_LSR_DR
                jr      z, crt_in

                in0     a, (UART0_RBR)
;; ^@ fix
                and     a
                jr      z, crt_in

; CRT character translations
                cp      127
                jr      nz, @f
                ld      a, 8
@@:
                ret.lil

tty_in:         in0     a, (UART1_LSR)
                and     UART_LSR_DR
                jr      z, tty_in

                in0     a, (UART1_RBR)

                ret.lil

conout:         ld      a, (IOBYTE)     ; fetch IOBYTE
                and     3               ; anything other than 0=TTY:?
                jp      z, tty_out      ; IOBYTE = 0, jump to UART1 output

crt_out:        in0     a, (PD_DR)      ; wait for VDP CTS signal
                tst     a, 8
                jr      nz, crt_out

wait_empty:     in0     a, (UART0_LSR)  ; check to see if transmit empty
                and     UART_LSR_TEMT
                jr      z, wait_empty

                ld      a, c
                out0    (UART0_THR), a

                xor     a
                ret.lil


tty_out:        in0     a, (UART1_LSR)
                and     UART_LSR_TEMT
                jr      z, tty_out


                ld      a, c
                out0    (UART1_THR), a

                xor     a
                ret.lil

list:           ld      a, (IOBYTE)     ; fetch IOBYTE
                and     $C0             ; anything other than 0=TTY:?
                jp      z, tty_out      ; IOBYTE = 0, jump to UART1 output
                jp      crt_out

punch:          xor     a
                ret.lil

reader:         xor     a
                ret.lil

listst:         xor     a
                ret.lil











