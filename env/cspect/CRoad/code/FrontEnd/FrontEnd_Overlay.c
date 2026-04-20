//  ***************************************************************************************
//
//                              Simple Front End framework
//
//  ***************************************************************************************
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

#include "Kernel.h"
#include "FrontEnd.h"
#include "data.h"

#pragma output CRT_ORG_CODE = 0x0000

//  ***************************************************************************************
//  Init the Front End system
//  ***************************************************************************************
void FE_Init( void )
{
    ClsATTR(56);
    ClsULA();
    WipeSprites();
    
    // Set state when we're ready to move into the main processing
    SetState(State_FrontEnd);
} 

//  ***************************************************************************************
//  Process the game
//  ***************************************************************************************
void FE_Process( void )
{    
    if( Keys[VK_1]!=0)
    {
        Keys[VK_1] = 0;
        SetState(State_QuitFrontEnd);
    }
} 

//  ***************************************************************************************
//  Render all game items
//  ***************************************************************************************
void FE_Render( void )
{
    Print(90,0,"Game Name\n");    
    Print(70,32,"1. Start Game\n");    
    Print(70,48,"2. Redefine Keys\n");    
    Print(70,64,"3. Instructions\n");    

    Print(50,180,"z88dk Game Framework");    
} 

//  ***************************************************************************************
//  Quit and shut down the frontend
//  ***************************************************************************************
void FE_Quit( void )
{    
    SetState(State_InitGame);
} 

