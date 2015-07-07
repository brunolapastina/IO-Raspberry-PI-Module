/*
 *    This file contains part of the register map for the Raspberry PI 1
 *    Taken mostly from the BCM 2835 datasheet
 *
 * Copyright
 *
 *    Copyright (C) 2015 Bruno La Pastina
 *
 * License
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published
 *    by the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details at
 *    <http://www.gnu.org/licenses/lgpl-3.0-standalone.html>
 *
 * Documentation
 *
 *    https://lapastina.wordpress.com/linux-device-driver-development/
 *    https://github.com/brunolapastina/IO-Raspberry-PI-Module
 */
#ifndef _RPI_REGISTERS_H_
#define _RPI_REGISTERS_H_

//------[ GPIO Registers map ]-----------------------------------------------------------
struct SGpioRegistersMap
{
   uint32_t GPFSEL[6];     // [R/W] GPIO Function Select
   uint32_t Reserved1;
   uint32_t GPSET[2];      // [W]   GPIO Pin Output Set
   uint32_t Reserved2;
   uint32_t GPCLR[2];      // [W]   GPIO Pin Output Clear
   uint32_t Reserved3;
   uint32_t GPLEV[2];      // [R]   GPIO Pin Level
   uint32_t Reserved4;
   uint32_t GPEDS[2];      // [R/W] GPIO Pin Event Detect Status
   uint32_t Reserved5;
   uint32_t GPREN[2];      // [R/W] GPIO Pin Rising Edge Detect Enable
   uint32_t Reserved6;
   uint32_t GPFEN[2];      // [R/W] GPIO Pin Falling Edge Detect Enable
   uint32_t Reserved7;
   uint32_t GPHEN[2];      // [R/W] GPIO Pin High Detect Enable
   uint32_t Reserved8;
   uint32_t GPLEN[2];      // [R/W] GPIO Pin Low Detect Enable
   uint32_t Reserved9;
   uint32_t GPAREN[2];     // [R/W] GPIO Pin Async. Rising Edge Detect
   uint32_t Reserved10;
   uint32_t GPAFEN[2];     // [R/W] GPIO Pin Async. Falling Edge Detect
   uint32_t Reserved11;
   uint32_t GPPUD;         // [R/W] GPIO Pin Pull-up/down Enable
   uint32_t GPPUDCLK[2];   // [R/W] GPIO Pin Pull-up/down Enable Clock
};

// GPFSEL[] - GPIO Function Select Registers Values
#define  GPIO_INPUT  0     // Pin is an Input
#define  GPIO_OUTPUT 1     // Pin is an output
#define  GPIO_ALT0   4     // Pin takes alternate function 0
#define  GPIO_ALT1   5     // Pin takes alternate function 1
#define  GPIO_ALT2   6     // Pin takes alternate function 2
#define  GPIO_ALT3   7     // Pin takes alternate function 3
#define  GPIO_ALT4   3     // Pin takes alternate function 4
#define  GPIO_ALT5   2     // Pin takes alternate function 5


//------[ PWM Registers map ]------------------------------------------------------------
struct SPWMRegistersMap
{
   uint32_t CTL;           // PWM Control
   uint32_t STA;           // PWM Status
   uint32_t DMAC;          // PWM DMA Configuration
   uint32_t Reserved1;
   uint32_t RNG1;          // PWM Channel 1 Range
   uint32_t DAT1;          // PWM Channel 1 Data
   uint32_t FIF1;          // PWM FIFO Input
   uint32_t Reserved2;
   uint32_t RNG2;          // PWM Channel 2 Range
   uint32_t DAT2;          // PWM Channel 2 Data
};

// CTL Register
#define  MSEN2       15    // Channel 2 M/S Enable
#define  USEF2       13    // Channel 1 Use Fifo
#define  POLA2       12    // Channel 1 Polarity
#define  SBIT2       11    // Channel 1 Silence Bit
#define  RPTL2       10    // Channel 1 Repeat Last Data
#define  MODE2       9     // Channel 1 Mode
#define  PWEN2       8     // Channel 1 Enable
#define  MSEN1       7     // Channel 1 M/S Enable
#define  CLRF1       6     // Clear Fifo
#define  USEF1       5     // Channel 1 Use Fifo
#define  POLA1       4     // Channel 1 Polarity
#define  SBIT1       3     // Channel 1 Silence Bit
#define  RPTL1       2     // Channel 1 Repeat Last Data
#define  MODE1       1     // Channel 1 Mode
#define  PWEN1       0     // Channel 1 Enable

// STA Register
#define  STA4        12    // Channel 4 State
#define  STA3        11    // Channel 3 State RW 0x0
#define  STA2        10    // Channel 2 State
#define  STA1        9     // Channel 1 State
#define  BERR        8     // Bus Error Flag RW 0x0
#define  GAPO4       7     // Channel 4 Gap Occurred Flag
#define  GAPO3       6     // Channel 3 Gap Occurred Flag
#define  GAPO2       5     // Channel 2 Gap Occurred Flag
#define  GAPO1       4     // Channel 1 Gap Occurred Flag
#define  RERR1       3     // Fifo Read Error Flag
#define  WERR1       2     // Fifo Write Error Flag
#define  EMPT1       1     // Fifo Empty Flag RW 0x1
#define  FULL1       0     // Fifo Full Flag

// DMAC Register
#define  ENAB        31    // DMA Enable
#define  PANIC       8     // DMA Threshold for PANIC signal
#define  DREQ        0     // DMA Threshold for DREQ signal

//------[ PCM Registers map ]------------------------------------------------------------
struct SPCMRegistersMap
{
   uint32_t CS_A;          // PCM Control and Status
   uint32_t FIFO_A;        // PCM FIFO Data
   uint32_t MODE_A;        // PCM Mode
   uint32_t RXC_A;         // PCM Receive Configuration
   uint32_t TXC_A;         // PCM Transmit Configuration
   uint32_t DREQ_A;        // PCM DMA Request Level
   uint32_t INTEN_A;       // PCM Interrupt Enables
   uint32_t INTSTC_A;      // PCM Interrupt Status & Clear
   uint32_t GRAY;          // PCM Gray Mode Control
};

// CS_A Register
#define  STBY        25    // RAM Standby
#define  SYNC        24    // PCM Clock sync helper.
#define  RXSEX       23    // RX Sign Extend
#define  RXF         22    // RX FIFO is Full
#define  TXE         21    // TX FIFO is Empty
#define  RXD         20    // Indicates that the RX FIFO contains data
#define  TXD         19    // Indicates that the TX FIFO can accept data
#define  RXR         18    // Indicates that the RX FIFO needs reading
#define  TXW         17    // Indicates that the TX FIFO needs Writing
#define  RXERR       16    // RX FIFO Error
#define  TXERR       15    // TX FIFO Error
#define  RXSYNC      14    // RX FIFO Sync
#define  TXSYNC      13    // TX FIFO Sync
#define  DMAEN       9     // DMA DREQ Enable
#define  RXTHR       7     // Sets the RX FIFO threshold at which point the RXR flag is set
#define  TXTHR       5     // Sets the TX FIFO threshold at which point the TXW flag is set
#define  RXCLR       4     // Clear the RX FIFO
#define  TXCLR       3     // Clear the TX FIFO
#define  TXON        2     // Enable transmission
#define  RXON        1     // Enable reception
#define  PCM_EN      0     // Enable the PCM Audio Interface

// MODE_A Register
#define  CLK_DIS     28    // PCM Clock Disable
#define  PDMN        27    // PDM Decimation Factor (N)
#define  PDME        26    // PDM Input Mode Enable
#define  FRXP        25    // Receive Frame Packed Mode
#define  FTXP        24    // Transmit Frame Packed Mode
#define  CLKM        23    // PCM Clock Mode
#define  CLKI        22    // Clock Invert this logically inverts the PCM_CLK signal
#define  FSM         21    // Frame Sync Mode
#define  FSI         20    // Frame Sync Invert This logically inverts the frame sync signal
#define  FLEN        10    // Frame Length
#define  FSLEN       0     // Frame Sync Length

// RXC_A and TXC_A Registers
#define  CH1WEX      31    // Channel 1 Width Extension Bit
#define  CH1EN       30    // Channel 1 Enable
#define  CH1POS      20    // Channel 1 Position
#define  CH1WID      16    // Channel 1 Width
#define  CH2WEX      15    // Channel 2 Width Extension Bit
#define  CH2EN       14    // Channel 2 Enable
#define  CH2POS      4     // Channel 2 Position
#define  CH2WID      0     // Channel 2 Width

// DREQ_A Register
#define  TX_PANIC    24    // TX Panic Level
#define  RX_PANIC    16    // RX Panic Level
#define  TX          8     // TX Request Level
#define  RX          0     // RX Request Level

// INTEN_A and INTSTC_A Registers
#define  RXERR       3     // RX Error Interrupt
#define  TXERR       2     // TX Error Interrupt
#define  RXR         1     // RX Read Interrupt Enable
#define  TXW         0     // TX Write Interrupt Enable

// GRAY Register
#define  RXFIFOLEVEL 16    // The Current level of the RXFIFO
#define  FLUSHED     10    // The Number of bits that were flushed into the RXFIFO
#define  RXLEVEL     4     // The Current fill level of the RX Buffer
#define  FLUSH       2     // Flush the RX Buffer into the RX FIFO
#define  CLR         1     // Clear the GRAY Mode Logic
#define  GRAY_EN     0     // Enable GRAY Mode

//------[ DMA Registers map ]------------------------------------------------------------

struct SDMAChannelRegistersMap
{
   uint32_t CS;            // DMA Channel Control and Status
   uint32_t CONBLK_AD;     // DMA Channel Control Block Address
   uint32_t TI;            // DMA Channel CB Word 0 (Transfer Information)
   uint32_t SOURCE_AD;     // DMA Channel CB Word 1 (Source Address)
   uint32_t DEST_AD;       // DMA Channel CB Word 2 (Destination Address)
   uint32_t TXFR_LEN ;     // DMA Channel CB Word 3 (Transfer Length) 
   uint32_t STRIDE;        // DMA Channel CB Word 4 (2D Stride)
   uint32_t NEXTCONBK;     // DMA Channel CB Word 5 (Next CB Address)
   uint32_t DEBUG;         // DMA Channel Debug 
};

// This struct must start at a 256-bit aligned address.
struct SDMAControlBlock
{
   uint32_t TI;            // Transfer Information
   uint32_t SOURCE_AD;     // Source Address
   uint32_t DEST_AD;       // Destination Address
   uint32_t TXFR_LEN;      // Transfer Length
   uint32_t STRIDE;        // 2D Mode Stride 
   uint32_t NEXTCONBK;     // Next Control Block Address 
   uint32_t Zeros[2];      // Reserved – set to zero
};

// DMA Channel Offsets
#define  DMA1_BASE   DMA0_BASE+0x100   // DMA Channel 1 Register Set
#define  DMA2_BASE   DMA0_BASE+0x200   // DMA Channel 2 Register Set
#define  DMA3_BASE   DMA0_BASE+0x300   // DMA Channel 3 Register Set
#define  DMA4_BASE   DMA0_BASE+0x400   // DMA Channel 4 Register Set
#define  DMA5_BASE   DMA0_BASE+0x500   // DMA Channel 5 Register Set
#define  DMA6_BASE   DMA0_BASE+0x600   // DMA Channel 6 Register Set
#define  DMA7_BASE   DMA0_BASE+0x700   // DMA Channel 7 Register Set
#define  DMA8_BASE   DMA0_BASE+0x800   // DMA Channel 8 Register Set
#define  DMA9_BASE   DMA0_BASE+0x900   // DMA Channel 9 Register Set
#define  DMA10_BASE  DMA0_BASE+0xA00   // DMA Channel 10 Register Set
#define  DMA11_BASE  DMA0_BASE+0xB00   // DMA Channel 11 Register Set
#define  DMA12_BASE  DMA0_BASE+0xC00   // DMA Channel 12 Register Set
#define  DMA13_BASE  DMA0_BASE+0xD00   // DMA Channel 13 Register Set
#define  DMA14_BASE  DMA0_BASE+0xE00   // DMA Channel 14 Register Set

// CS Registers
#define  RESET       31    // DMA Channel Reset
#define  ABORT       30    // Abort DMA
#define  DISDEBUG    29    // Disable debug pause signal
#define  WAIT_FOR_OUTSTANDING_WRITES      28    // Wait for outstanding writes
#define  PANIC_PRIORITY                   20    // AXI Panic Priority Level
#define  PRIORITY    16    // AXI Priority Level
#define  ERROR       8     // DMA Error
#define  WAITING_FOR_OUTSTANDING_WRITES   6     // DMA is Waiting for the Last Write to be Received
#define  DREQ_STOPS_DMA                   5     // DMA Paused by DREQ State
#define  PAUSED      4     // DMA Paused State
#define  DREQ        3     // DREQ State
#define  INT         2     // Interrupt Status
#define  END         1     // DMA End Flag
#define  ACTIVE      0     // Activate the DMA


//------[ Clock Registers map ]----------------------------------------------------------
// The clock map on the datasheet is not complete. It has only the GPIO clock.
// I menage to find the PWM clock too, but anything is going to need some more research
struct SClockManagerRegistersMap
{
   uint32_t Unknown1[28];  // Off=0x00 to 0x70 - 28 Unkown registers
   uint32_t CM_GP0CTL;     // Off=0x70 - Clock Manager General Purpose Clocks Control 0
   uint32_t CM_GP0DIV;     // Off=0x74 - Clock Manager General Purpose Clock Divisors 0
   uint32_t CM_GP1CTL;     // Off=0x78 - Clock Manager General Purpose Clocks Control 1
   uint32_t CM_GP1DIV;     // Off=0x7C - Clock Manager General Purpose Clock Divisors 1
   uint32_t CM_GP2CTL;     // Off=0x80 - Clock Manager General Purpose Clocks Control 2
   uint32_t CM_GP2DIV;     // Off=0x84 - Clock Manager General Purpose Clock Divisors 2
   uint32_t Unknown2[4];   // Off=0x88 to 0x9C - 4 Unkown registers
   uint32_t CM_PCMCTL;     // Off=0x98 - Clock Manager PCM Clock Control
   uint32_t CM_PCMDIV;     // Off=0x9C - Clock Manager PCM Clock Divisors
   uint32_t CM_PWMCTL;     // Off=0xA0 - Clock Manager PWM Clock Control
   uint32_t CM_PWMDIV;     // Off=0xA4 - Clock Manager PWM Clock Divisors
};

#endif