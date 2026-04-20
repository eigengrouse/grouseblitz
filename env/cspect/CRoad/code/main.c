//
//      ZX Spectrum Next z88dk Gaming FrameWork
//
//      CSpect map file: -zmap=hello_world.map
// Nightly Builds
// http://nightly.z88dk.org/

#pragma output REGISTER_SP = 0xbfff

#include <arch/zxn.h>           // ZX Spectrum Next architecture specfic functions
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <z80.h>
#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <input.h>
#include <errno.h>

#include "FrontEnd\FrontEnd.h"
#include "Game\GamePlay.h"
#include "FrameWork.h"
#include "Kernel.h"
#include "data.h"

//z80_outp(0x7ffd, 1);
//ZXN_WRITE_MMU6(2);


void NakedTest(void) __naked
{
__asm
        GLOBAL ASM_MYVALUE
        //defc    ASM_MYVALUE = MYVALUE
        push    hl
        push    af
        ld  a,8
        ld  hl,$4000
        ld  (hl),a
        swap
        pop af
        pop hl
        ret
__endasm;
}


// ****************************************************************************************
//  Handle the main loop and state changes
// ****************************************************************************************
void MainLoop(void)
{
    SetState(State_InitFrontEnd);
    bool quit = false;
    while(quit!=true)
    {        
        WaitVBlank();
        CopySpriteData();
        ReadKeyboard();

        // Do game states
        switch(GameState)
        {
            case State_InitFrontEnd:
            {
                NextReg(0x50,8);
                FE_Init();
                break;
            }
            case State_FrontEnd:
            {
                NextReg(0x50,8);
                FE_Process();
                FE_Render();
                break;
            }
            case State_QuitFrontEnd:
            {
                NextReg(0x50,8);
                FE_Quit();
                break;
            }
            case State_InitGame:
            {
                NextReg(0x50,6);
                GP_Init();
                break;
            }
            case State_Game:
            {
                NextReg(0x50,6);
                GP_Process();
                GP_Render();
                break;
            }
            case State_QuitGame:
            {
                NextReg(0x50,6);
                GP_Quit();
                break;
            }
            default:{
                SetState(State_InitFrontEnd);
            }
        }
        quit = false;
    }

    int i=12;
}


typedef struct tStructType
{
    uint8   value1;
    uint8   value2;
    uint16  value3;
    uint8   value4;
    uint8   value5;
} SStructType;
 
// ************************************************************************************************************************
//
//  Main program start
//
// ************************************************************************************************************************
int  main(void)
{
    SStructType Str;
    BREAK;
    intrinsic_label(Main_Label);

    NextReg(0x7,3);           // 28Mhz
    NextReg(0x8,0x4A);        // Disable RAM contention, enable DAC and turbosound
    NextReg(0x5,0x04);        // 60Hz mode
    NextReg(0x15,0x03);       // layer order - and sprites on
    NextReg(0x4b,0xe3);       // sprite transparency
    NextReg(0x80,0);          // Make sure expansion bus is off.....

    NextReg(0x57,2);          // page in kernel

    InitKernel();
    SetUpIRQs();

    Str.value1=12;

    Layer2Enable(false);
    ClsATTR(56);                // white paper, black ink
    ClsULA();

    PrintHex(0x12,0x4000);
    DMACopy(0x4000,0x4800,0x800);
    UploadSprites(0,0x04,(uint16*) 0x5678);

    MainLoop();

    // never return
    while(1){}
}









