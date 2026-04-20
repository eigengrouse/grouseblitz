    
    SECTION FRONTEND_SEG
    PUBLIC _FrontEndTest

    include "includes.inc"

_FrontEndTest:
    push    af
    ld      a,1
    nop
    ld      a,2
    pop     af
    ret

    dw _FrontEndTest
    db "Hello"



