	AREA |.text|,CODE,READONLY,ALIGN=2
	THUMB	
	EXTERN current_tcb
	EXPORT SysTick_Handler
	EXPORT osscehdulerlaunch
	EXTERN	system_tick		
SysTick_Handler	;save r0 r1 r2 r3 r12 lr pc psr
	CPSID		I
	PUSH		{R4-R11}
	LDR			R0,=current_tcb
	LDR			R1,[R0]
	
	STR			sp,[R1]
	LDR			R1,[R1,#4]
	STR			R1,[R0]
	
	LDR			sp,[R1]
	
	POP			{R4-R11}
	
	LDR			R0,=system_tick
	LDR			R1,[R0]
	ADD			R1,R1,#1
	STR			R1,[R0]
	
	CPSIE		I
	
	BX			LR

osscehdulerlaunch
	LDR		R0,=current_tcb
	LDR		R2,[R0]
	
	LDR		sp,[R2]   ;
	
	POP		{R4-R11}
	POP		{R0-R3}
	POP		{R12}
	ADD		sp,sp,#4
	POP		{LR}
	ADD		sp,sp,#4
	CPSIE	I
	BX		LR
	ALIGN
	END
	


