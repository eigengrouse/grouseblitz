
SECTION GAMEPLAY_SEG
PUBLIC _GamePlayTest,_CopperStart

    include "includes.inc"

_GamePlayTest:
    push    af
    ld      a,1
    nop
    ld      a,2
    pop     af
    ret

    dw _GamePlayTest
    db "Hello"



    defc    COP_NOP                 =     0
    defc    COP_PER_1               =     5
    defc    COP_PER_2               =     6
    defc    COP_TURBO               =     7        
    defc    COP_PER_3               =     8
    defc    COP_LAYER2_BANK         =     18      ; layer 2 bank
    defc    COP_LAYER2_SBANK        =     19      ; layer 2 shadow bank
    defc    COP_TRANSPARENT         =     20      ; Global transparency color
    defc    COP_SPRITE              =     21      ; Sprite and Layers system
    defc    COP_LAYER2_XOFF         =     22
    defc    COP_LAYER2_YOFF         =     23
    defc    COP_LAYER2_CLIP         =     24
    defc    COP_SPRITE_CLIP         =     25
    defc    COP_ULA_CLIP            =     26
    defc    COP_CLIP_CNT            =     28
    defc    COP_IRQ                 =     34
    defc    COP_IRQ_RAST_LO         =     35
    defc    COP_TILEMAP_X_MSB       =     47
    defc    COP_TILEMAP_X_LSB       =     48
    defc    COP_TILEMAP_Y_OFF       =     49
    defc    COP_LOWRES_XOFF         =     50
    defc    COP_LOWRES_YOFF         =     51
    defc    COP_PALETTE_INDEX       =     64
    defc    COP_PALETTE_COLOUR      =     65      ; 8 bit palette colour
    defc    COP_PALETTE_FORMAT      =     66      
    defc    COP_PALETTE_CONTROL     =     67       
    defc    COP_PALETTE_COLOUR_9    =     68      ; 9 bit palette colour
    defc    COP_MMU0                =     80
    defc    COP_MMU1                =     81
    defc    COP_MMU2                =     82
    defc    COP_MMU3                =     83
    defc    COP_MMU4                =     84
    defc    COP_MMU5                =     85
    defc    COP_MMU6                =     86
    defc    COP_MMU7                =     87
    defc    COP_DATA                =     96
    defc    COP_CONTROL_LO          =     97
    defc    COP_CONTROL_HI          =     98
    defc    COP_FALLBACK_COLOUR     =     $4a


MOVE    macro   dest,value
        db  dest
        db  value
        endm
WAIT    macro   VPOS,HPOS
        db  $80+(HPOS*2)+(VPOS/256)
        db  VPOS&$ff
        endm        

_CopperStart:

        WAIT    400,$3f
        MOVE    COP_NOP,0
        MOVE    COP_NOP,0

        WAIT    400,$3f

        ds      2048

        



