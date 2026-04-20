//  ***************************************************************************************
//
//                              Simple Game framework
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
#include "..\FrontEnd\FrontEnd.h"
#include "data.h"

#include "GamePlay.h"
#include "Map.h"

#pragma output CRT_ORG_CODE = 0x0000

    uint16  counter;

//  ***************************************************************************************
//  Init the Game
//  ***************************************************************************************
void GP_Init( void )
{
    ClsATTR(7);
    ClsULA();
    InitSpriteData();
    Map_Init();
    Map_Render();
    //Print(9*8,16*8,"Central Cavern");

    // Set game process state when ready
    SetState(State_Game);
} 

//  ***************************************************************************************
//  Process the game
//  ***************************************************************************************
void GP_Process( void )
{
    if( Keys[VK_2]!=0)
    {
        Keys[VK_2] = 0;
        SetState(State_QuitGame);
    }

    Map_Process();
    counter++;
} 

//  ***************************************************************************************
//  Render the game
//  ***************************************************************************************
void GP_Render( void )
{
    Map_Render();
} 

//  ***************************************************************************************
//  Quiit the game and cleanup
//  ***************************************************************************************
void GP_Quit( void )
{
    //Print(0,48,"GP_Quit\n");

    // Set game process state when ready
    SetState(State_InitFrontEnd);
} 





