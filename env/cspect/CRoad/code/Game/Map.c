//  ***************************************************************************************
//
//                              Road processing
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
#include "data.h"

#include "GamePlay.h"
#include "Map.h"

#define	STARTY	(32)

extern  uint8 	CopperStart;

	uint8 	BankStore;
	uint8*	pMapData;

	uint8* pRnd = 0x0000;
	uint16 XScroll = 480-160;

	int16	XScrollValues[200];
	uint8	YPosValues[200];
	uint8	RoadColour[200];
	int16	DivTable[256];

	int32	XOffset=0;
	int32	Xcurve=0x00000000;
	int32	ZPos=0;

	// local variables...globals operate faster, as locals are loaded via LD A,(IX+?) instead of LD A,(ADD)
	int32	xpos = 0;
	uint16	yPos = 0;
	uint8* 	pRoadCol;					
	uint8* 	pYPosValues;
	uint8* 	pYPosValues_End;
	int16* 	pXScrollValues;
	int16 	yy;
	int32	dx;
	int32	curvex;

void GenerateCopper(void);

// ***************************************************************************************
//  Draw the inital map
// ***************************************************************************************
void DrawRoad(void)
{
	NextReg(0x56,0x40);

}


// ***************************************************************************************
//  Initialise the Map system
// ***************************************************************************************
void Map_Init(void)
{
	uint16 bank56 = ReadNextReg(0x56);
	XScroll = 480-160;

	NextReg(0x14,0xe3);					// global transparency
	NextReg(0x43,0x00);					// select ULA palette 1
	NextReg(0x40,0x13);					// set palette index 3 - Magenta
	NextReg(0x41,0xe3)
	NextReg(0x40,0x1b);					// set palette index 3 - Magenta
	NextReg(0x41,0xe3)
	NextReg(0x6b,0x00);					// disable tilemaps

	NextReg(0x18,16);					// set L2 window to ULA size
	NextReg(0x18,143);
	NextReg(0x18,32);
	NextReg(0x18,224);


	Map_Load();

	NextReg(0x56,14);					// page in shadow ULA
	PrintOffset = 0x80;
	uint8* pScreen = (uint8*) 0xc000;
	for(int i=0;i<6144;i++){
		*pScreen++=0;
	}
	// F_B_PPP_III
	for(int i=0;i<1000;i++){
		*pScreen++=7+0x18+0x40;
	}
	NextRegA(0x56,(uint8)bank56);

	NextReg(0x12,0x25);
	NextReg(0x70,0x10);				// 320x256 mode
	NextReg(0x69,0xc0);				// use Shadow ULA screen + Layer 2
	NextReg(0x71,1);				// X Scroll MSB ALWAYS set to activate over scroll
	NextReg(0x16,0x40);				// X Scroll lowqer
	
	NextReg(0x15,16);				// U S L
	NextReg(0X64,21);				// offset copper lines by -21 (top of 60hz screen)

	for(int i=0;i<256;i++){
		DivTable[i] = (-1000 / i);
	}

	GenerateFullCopper();			// generates all copper instructions - we can update the values quicker later
}


uint8 mapret;
// ***************************************************************************************
//  Load the map file
// ***************************************************************************************
void Map_Load(void)
{
	mapret = Load("data\\road.256",0x40,0x0000);		// load road with red edges + middle line
	mapret = Load("data\\road2.256",0x60,0x0000);		// load road with white edges and no line
	if(mapret==0xff){
		Border(2);
		while(1){
			mapret++;
		}
	}
}


// ***************************************************************************************
//  Process map scrolling etc - if any
// ***************************************************************************************
 void 	Map_Process(void)
 {
    if( Keys[VK_O]!=0)
    {
        XOffset-=0x80000;
    }
    else if( Keys[VK_P]!=0)
    {
        XOffset+=0x80000;
    }


    if( Keys[VK_Z]!=0)
    {
        Xcurve += 0x0040;
    }
    else if( Keys[VK_X]!=0)
    {
        Xcurve -= 0x0040;
    }
    
    if( Keys[VK_Q]!=0)
    {
        ZPos += 0x0002;
    }
    else if( Keys[VK_A]!=0)
    {
        ZPos -= 0x0002;
    }
    


 }


// ***************************************************************************************
//  Render the map
// ***************************************************************************************
void Map_Render(void)
{
	UploadCopper((uint8*) &CopperStart, 2048);	
	NextReg(0x61,0)
        NextReg(0x62,0xc0)		// %11000000   ; Copper control

	// print the text we loaded in
	uint16 val = ReadNextReg(0x56);
	NextReg(0x56,14);
	Print(16,64,"Hello World");
	NextRegA(0x56,(uint8)val);

	//Border(2);
	CalcRoad();
	GenerateCopper();
	Border(0);
}


// ***************************************************************************************
//  Free up the map file/variables etc if need be
// ***************************************************************************************
void Map_Quit(void)
{

}

// ***************************************************************************************
//  Page the map/collision map in
// ***************************************************************************************
char* Map_In(void)
{
	// print the text we loaded in
	BankStore = (uint8) ReadNextReg(0x56);
	NextReg(0x56,0x40);
	return pMapData;
}


// ***************************************************************************************
//  Page the map/collision map in
// ***************************************************************************************
void Map_Out(void)
{
	NextRegA(0x56,(uint8)BankStore);
}

// ***************************************************************************************
//  Page the map/collision map in
// ***************************************************************************************
void GenerateFullCopper(void)
{
	uint8* pCopper = &CopperStart;
	*pCopper++ = 0x80;			// first wait
	*pCopper++ = 18;
	*pCopper++ = 0x43;			// set palette control
	*pCopper++ = (uint8)0;
	*pCopper++ = 0x40;			// colour index select
	*pCopper++ = 0x01;

	// bank base
	uint16 bank = 0x25;
	uint8 cnt = 0x40;

	uint8 yscroll=(uint8) -32;
	int16* pXScrollValues = &XScrollValues[0];
	uint8* pRoadCol = &RoadColour[0];
	for(int i=0;i<200;i++)
	{
		int xscr = *pXScrollValues++;
		uint8 xx = (xscr&0x3f)+0x40;
		uint8 bank = ((xscr>>6)&0xf)+0x20;
		if(*pRoadCol++!=0) bank+=0x10;

		*pCopper++ = 0xe0;					// WAIT Rast,$30  (far right)
		*pCopper++ = (uint8)i+1+19;
		*pCopper++ = 0x12;					// layer 2 bank
		*pCopper++ = bank;
		*pCopper++ = 0x16;					// x scroll
		*pCopper++ = xx;
		*pCopper++ = 0x17;					// y scroll
		*pCopper++ = -32+YPosValues[i]-i;
	}
	*pCopper++ = 0x81;						// infinite wait
	*pCopper++ = 0xff;
}

// ***************************************************************************************
//  Page the map/collision map in
// ***************************************************************************************
void GenerateCopper(void)
{
	uint8* pCopper = &CopperStart;
	pCopper += 7;
//	*pCopper++ = 0x80;			// first wait
//	*pCopper++ = 18;
//	*pCopper++ = 0x43;			// set palette control
//	*pCopper++ = (uint8)0;
//	*pCopper++ = 0x40;			// colour index select
//	*pCopper++ = 0x01;

	// bank base
	uint16 bank = 0x25;
	uint8 cnt = 0x40;

	uint8 yscroll=(uint8) -32;
	int16* pXScrollValues = &XScrollValues[0];
	uint8* pRoadCol = &RoadColour[0];
	for(int i=0;i<200;i++)
	{
		int xscr = *pXScrollValues++;
		uint8 xx = (xscr&0x3f)+0x40;
		uint8 bank = ((xscr>>6)&0xf)+0x20;
		if(*pRoadCol++!=0) bank+=0x10;

		*pCopper = (uint8)i+1+19;
		pCopper+=2;
		*pCopper = bank;
		pCopper+=2;
		*pCopper = xx;
		pCopper+=2;
		*pCopper = -32+YPosValues[i]-i;
		pCopper+=2;
	}
//	*pCopper++ = 0x81;						// infinite wait
//	*pCopper++ = 0xff;
}


// ***************************************************************************************
//  Generate the road offsets
// ***************************************************************************************
void CalcRoad(void)
{
	uint16	YDelta =(uint16)(0xf800/128);		// work out delta
	yPos = (uint16)(255<<8); 


	// work out line draw from (center,top) of road, to (XOffset,bottom). But we subtract so we can work bottom up from XOffset (allowing curves to be added)
	dx = XOffset/128;
	curvex = 0;
	xpos = XOffset;

	pRoadCol = &RoadColour[199];
	pYPosValues = &YPosValues[199];
	pYPosValues_End = &YPosValues[-1];		// use this as the end of the loop
	pXScrollValues = &XScrollValues[199];
	
	yy = 128;
	while(1)
	{	
		uint8 yline = (uint8) (yPos>>8);					// resolves into a "LD A,(&yPos+1)"
		if( yline<8) break;
		*pYPosValues-- = (uint8)yline;

		// dodgy perspective calc (-1000/yy) to give us some kind of red/white and middle line stripes.
		uint8 red_white = (uint8) ((( DivTable[yy--] - ZPos) >>3)&1);

 		yPos -= YDelta;

 		int16 xxpos = (xpos>>16);									// resolves to "LD HL(&xpos+2)"
 		if(red_white==0) *pRoadCol--=1; else *pRoadCol--=0;

 		*pXScrollValues-- = (480-160)+xxpos;
		
		curvex += Xcurve;
		xpos += curvex;
		xpos -= dx;		
	}


	// wipe rest of screen
	while(pYPosValues!=pYPosValues_End){
		*pXScrollValues-- = 0;
		*pYPosValues-- = 255;
	}

}




