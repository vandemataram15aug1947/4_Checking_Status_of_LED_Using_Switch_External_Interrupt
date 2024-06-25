/*Maaz khan
 * Reg No : 19jzele0320
 */

//*****************************************
//Blinking of LED Using External interrupts  based on switch
//*****************************************

#include "F28x_Project.h"

////////////  MAAZ KHAN  ///////////

// Function prototype
#define BLINKY_LED1_GPIO    31
#define BLINKY_LED2_GPIO    34
#define Logic1     GpioDataRegs.GPADAT.bit.GPIO18
#define Logic2     GpioDataRegs.GPADAT.bit.GPIO22
interrupt void myExternalInterrupt(void);
void main(void)
{

    ////////////  MAAZ KHAN  ///////////

    // Initialize System Control:
    InitSysCtrl();

    DINT;

    // Initialize GPIO:

    InitGpio();
    //Configure GPIO_34 and GPIO_31 as an Output Using driver libraries
    GPIO_SetupPinMux(BLINKY_LED1_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(BLINKY_LED1_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(BLINKY_LED2_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(BLINKY_LED2_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);

    ////////////  MAAZ KHAN   //////////////

    // Initialization of GPIO_18 and GPIO_22 for logic analyzer as an output
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;
    // Use GPIO 18 as an Output
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
    // Use GPIO 22 as an Output
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;
    EDIS;

    ////////////  MAAZ KHAN   //////////////

    //Configure GPIO_67 as an Input
    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO67= 0;
    EDIS;

    // Initialize the PIE control registers to their default state.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    //Initialize PIE Vector Table
    InitPieVectTable();

    ////////////  MAAZ KHAN   //////////////

    //Configure External Interrupt 1 in PIE Vector table
    EALLOW;
    PieVectTable.XINT1_INT = &myExternalInterrupt;
    EDIS;

    //Setup GPIO_67 as an External Interrupt
    GPIO_SetupXINT1Gpio(67);

    // Rising edge trigger
    XintRegs.XINT1CR.bit.POLARITY = 1;
    // Enable XINT1
    XintRegs.XINT1CR.bit.ENABLE = 1;

    //Enable Core Interrupt 1 and Sub interrupt 4 in PIE Vector table
    EALLOW;
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
    EDIS;

    // M- means to Enable,In vice-versa N-means to disable
    IER |= M_INT1;

    // Enable global Interrupts and higher priority real-time debug events:

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    // Forever loop
    for(;;)
    {
        ////////////  MAAZ KHAN   //////////////

        // Turn on LED1

        GPIO_WritePin(BLINKY_LED1_GPIO, 1);

        // Delay for a bit. 500ms
        DELAY_US(1000*500);

        // Turn off LED
        GPIO_WritePin(BLINKY_LED1_GPIO, 0);

        // Delay for a bit.
        DELAY_US(1000*500);

        // Logic 1 (logic analyzer)
        Logic1 = 1;
        DELAY_US(1000*500);
        Logic1 = 0;
        DELAY_US(1000*500);

        // Wait for CPU to wake up when interrupt is trigger
        asm("IDLE:");
    }
}
// ISR for GPIO67
interrupt void myExternalInterrupt(void)
{
    /////////////  MAAZ KHAN ///////////////
    GPIO_WritePin(BLINKY_LED2_GPIO, 1);
    DELAY_US(1000*500);
    GPIO_WritePin(BLINKY_LED2_GPIO, 0);
    DELAY_US(1000*500);
    ////////////  MAAZ KHAN   //////////////
    Logic2 = 1;
    DELAY_US(1000*500);
    Logic2 = 0;
    DELAY_US(1000*500);
    // Acknowledge interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}



