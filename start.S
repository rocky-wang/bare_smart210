/*
 *@Author	: Rocky
 *@Date		: 2014-12-29
 *@Descri	: The Initial for smart210 after reset
*/

.global _start
_start:
	b		reset_handler
	nop

reset_handler:
	mov		r0,r1
loop:
	b		loop

.end

