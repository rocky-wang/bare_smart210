The Project For Studying SMART210 Board,The plan is :
1. Interface
	1.1 SD-card for LED blinking
	1.2 SD-card for UART input/output
	1.3 SD-card for printf
2. Bootloader for Linux
=========================================================
Version0.1:
	按照系统默认情况，不改变Icache,系统时钟等参数，配置GPIO口操作LED灯。
Version0.2:
	实验系统指令cache打开后的效果，同时改变delay函数为C语言内嵌汇编的方式。
Version0.3:
	使用内置时序配置UART最简模型，非FIFO，非中断的轮询模式，实现输出功能puts。
Version0.4:
	增加printf函数功能，支持除法与求余功能，但不显示64bit的空间。
Version0.5:
	在系统汇编启动中，加入PLL超频功能，默认主频超到800MHZ。同时增加系统关键时钟的自动计算函数。
	UART波特率使用程序计算分配系数和余数。

