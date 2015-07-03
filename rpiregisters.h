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