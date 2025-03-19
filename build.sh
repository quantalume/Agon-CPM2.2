#!/bin/bash

# Build autoexec first
# On your PC
cd autoexec && sjasmplus autoexec.asm

# ----- Building for CON:=CRT: (VDP) -----
# On your PC
cd sources && sjasmplus -DCRT=1 main.asm

# On the Agon or emulator
(copy bootstrap folder from PC to Agon)
cd bootstrap && ez80asm cpm.asm
cp bootstrap/cpm.bin cpm.bin

# ----- Building for CON:=TTY: (UART1) -----
# On your PC
cd sources && sjasmplus main.asm

# On the Agon or emulator
(copy bootstrap folder from PC to Agon)
cd bootstrap && ez80asm cpm.asm
cp bootstrap/cpm.bin cpm-uart.bin
