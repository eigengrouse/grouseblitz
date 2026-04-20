
IDIR =../include
CC=zcc
CFLAGS=+zxn -vn -c -SO3 --list --c-code-in-asm --opt-code-speed -clib=sdcc_iy -pragma-include:zpragma.inc  --max-allocs-per-node400000
MCFLAGS=+zxn -vn -c -SO3 --list --c-code-in-asm --opt-code-speed -clib=sdcc_iy -pragma-include:zpragma.inc  --max-allocs-per-node2000000

ODIR=obj
LDIR =../lib
LIBS=-lm



OBJS = Obj\main.o Obj\FrontEnd_Overlay.o Obj\GamePlay_Overlay.o Obj\GamePlay_Asm.o Obj\FrontEnd_Asm.o Obj\Kernel.o Obj\IRQs.o Obj\data.o Obj\Map.o


hello_world.nex: $(OBJS) 
	$(CC) +zxn -vn -m -clib=sdcc_iy -Cz"--clean" -pragma-include:zpragma.inc -startup=1 $(OBJS) -o hello_world.nex -create-app -subtype=nex


# Main program at $8000
Obj\main.o: 	main.c FrontEnd\FrontEnd.h FrameWork.h Kernel.h
		$(CC) $(CFLAGS) -o Obj\main.o main.c
		cmd /c move /Y main.c.lis Obj
Obj\data.o: 	data.c FrameWork.h Kernel.h
		$(CC) $(CFLAGS) -o Obj\data.o data.c
		cmd /c move /Y data.c.lis Obj



# Kernel section
Obj\kernel.o: Kernel.asm
	$(CC) $(CFLAGS) --codesegPAGE_02_KERNEL_CODE --constsegPAGE_02_KERNEL_CODE -o Obj\Kernel.o Kernel.asm
	cmd /c move /Y Kernel.asm.lis Obj
Obj\irqs.o: IRQs.asm
	$(CC) $(CFLAGS) --codesegPAGE_02_KERNEL_IRQ --constsegPAGE_02_KERNEL_IRQ -o Obj\IRQs.o IRQs.asm
	cmd /c move /Y IRQs.asm.lis Obj



# Front End overlay
Obj\FrontEnd_Overlay.o: FrontEnd\FrontEnd_Overlay.c FrontEnd\FrontEnd.h FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o Obj\FrontEnd_Overlay.o FrontEnd\FrontEnd_Overlay.c 
	cmd /c move /Y FrontEnd\FrontEnd_Overlay.c.lis Obj
Obj\FrontEnd_Asm.o: FrontEnd\FrontEnd_Asm.asm
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o Obj\FrontEnd_Asm.o FrontEnd\FrontEnd_Asm.asm
	cmd /c move /Y FrontEnd\FrontEnd_Asm.Asm.lis Obj



# Game Play overlay
Obj\GamePlay_Overlay.o: Game\GamePlay_Overlay.c Game\Map.h FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o Obj\GamePlay_Overlay.o Game\GamePlay_Overlay.c
	cmd /c move /Y Game\GamePlay_Overlay.c.lis Obj
Obj\Map.o: Game\Map.c Game\Map.h FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o Obj\Map.o Game\Map.c
	cmd /c move /Y Game\Map.c.lis Obj
Obj\GamePlay_Asm.o: Game\GamePlay_Asm.asm
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o Obj\GamePlay_Asm.o Game\GamePlay_Asm.asm	
	cmd /c move /Y Game\GamePlay_Asm.asm.lis Obj

clean:
	del -s -f Obj\*.o 
	del -s -f *.lis
	del -f *.bin
	del -f *.nex
	del -f *.map




