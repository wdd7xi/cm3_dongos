; context_rvds.S

	AREA |.txt|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8

hw_interrupt_disable	PROC
	EXPORT	hw_interrupt_disable
	MRS		r0, 	PRIMASK
	CPSID	I
	BX		LR
	ENDP

hw_interrupt_enable	PROC
	EXPORT	hw_interrupt_enable
	MSR		PRIMASK, 	r0
	BX		LR
	ENDP


	ALIGN	4
		
	END
