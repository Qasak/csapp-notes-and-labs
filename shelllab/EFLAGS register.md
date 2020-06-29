32位的EFLAGS寄存器包含一组状态标志、系统标志以及一个控制标志。在x86处理器初始化之后，EFLAGS寄存器的状态值为0000 0002H。第1、3、5、15以及22到31位均被保留，这个寄存器中的有些标志通过使用特殊的通用指令可以直接被修改，但并没有指令能够检查或者修改整个寄存器。通过使用LAHF/SAHF/PUSHF/POPF/POPFD等指令，可以将EFLAGS寄存器的标志位成组移到程序栈或EAX寄存器，或者从这些设施中将操作后的结果保存到EFLAGS寄存器中。在EFLAGS寄存器的内容被传送到栈或是EAX寄存器后，可以通过位操作指令(BT, BTS, BTR, BTC)检查或修改这些标志位。当调用中断或异常处理程序时，处理器将在程序栈上自动保存EFLAGS的状态值。若在中断或异常处理时发生任务切换，那么EFLAGS寄存器的状态将被保存在TSS中 【the state of the EFLAGS register is saved in the TSS for the task being suspended.】 ，注意是将要被挂起的本次任务的状态。

![img](https://github.com/Qasak/all-about-computer-system/blob/master/shelllab/EFLAGS%20register.jpg)

1、状态标志(Status Flags)
EFLAGS寄存器的状态标志(0、2、4、6、7以及11位)指示算术指令（如ADD, SUB, MUL以及DIV指令）的结果，这些状态标志的作用如下：

CF(bit 0) [Carry flag]   若算术操作产生的结果在最高有效位(most-significant bit)发生进位或借位则将其置1，反之清零。这个标志指示无符号整型运算的溢出状态，这个标志同样在多倍精度运算(multiple-precision arithmetic)中使用。
PF(bit 2) [Parity flag]   如果结果的最低有效字节(least-significant byte)包含偶数个1位则该位置1，否则清零。
AF(bit 4) [Adjust flag]   如果算术操作在结果的第3位发生进位或借位则将该标志置1，否则清零。这个标志在BCD(binary-code decimal)算术运算中被使用。
ZF(bit 6) [Zero flag]   若结果为0则将其置1，反之清零。
SF(bit 7) [Sign flag]   该标志被设置为有符号整型的最高有效位。(0指示结果为正，反之则为负)
OF(bit 11) [Overflow flag]   如果整型结果是较大的正数或较小的负数，并且无法匹配目的操作数时将该位置1，反之清零。这个标志为带符号整型运算指示溢出状态。
在这些状态标志中，只有CF标志能够通过使用STC, CLC以及CMC指令被直接修改，或者通过位指令(BT, BTS, BTR以及BTC)将指定的位拷贝至CF标志中。


这些状态标志允许单个的算术操作产生三种不同数据类型的结果：无符号整型，有符号整型以及BCD整型。如果把该结果当做无符号整型，那么CF标志指示越界(out-of-range)状态——即进位或借位，如果被当做有符号整型，则OF标志指示进位或借位，若作为BCD数，那么AF标志指示进位或借位。SF标志指示有符号整数的符号位，ZF指示结果为零。此外在执行多倍精度算术运算时，CF标志用来将一次运算过程中带进位的加法(ADC)或带借位的减法(SBB)产生的进位或借位传递到下一次运算过程中。


2、DF标志(DF flag)
这个方向标志(位于EFLAGS寄存器的第10位)控制串指令(MOVS, CMPS, SCAS, LODS以及STOS)。设置DF标志使得串指令自动递减（从高地址向低地址方向处理字符串），清除该标志则使得串指令自动递增。STD以及CLD指令分别用于设置以及清除DF标志。


3、系统标志以及IOPL域(System Flags and IOPL Field)
EFLAGS寄存器中的这部分标志用于控制操作系统或是执行操作，它们不允许被应用程序所修改。这些标志的作用如下：

TF(bit 8) [Trap flag]   将该位设置为1以允许单步调试模式，清零则禁用该模式。
IF(bit 9) [Interrupt enable flag]   该标志用于控制处理器对可屏蔽中断请求(maskable interrupt requests)的响应。置1以响应可屏蔽中断，反之则禁止可屏蔽中断。
IOPL(bits 12 and 13) [I/O privilege level field]   指示当前运行任务的I/O特权级(I/O privilege level)，正在运行任务的当前特权级(CPL)必须小于或等于I/O特权级才能允许访问I/O地址空间。这个域只能在CPL为0时才能通过POPF以及IRET指令修改。
NT(bit 14) [Nested task flag]   这个标志控制中断链和被调用任务。若当前任务与前一个执行任务相关则置1，反之则清零。
RF(bit 16) [Resume flag]   控制处理器对调试异常的响应。
VM(bit 17) [Virtual-8086 mode flag]   置1以允许虚拟8086模式，清除则返回保护模式。
AC(bit 18) [Alignment check flag]   该标志以及在CR0寄存器中的AM位置1时将允许内存引用的对齐检查，以上两个标志中至少有一个被清零则禁用对齐检查。
VIF(bit 19) [Virtual interrupt flag]   该标志是IF标志的虚拟镜像(Virtual image)，与VIP标志结合起来使用。使用这个标志以及VIP标志，并设置CR4控制寄存器中的VME标志就可以允许虚拟模式扩展(virtual mode extensions)
VIP(bit 20) [Virtual interrupt pending flag]   该位置1以指示一个中断正在被挂起，当没有中断挂起时该位清零。【Software sets and clears this flag; the processor only reads it.】与VIF标志结合使用。
ID(bit 21) [Identification flag]   程序能够设置或清除这个标志指示了处理器对CPUID指令的支持。
【For a detailed description of these flags: see Chapter 3, "Protected-Mode Memory Management"】
————————————————
版权声明：本文为CSDN博主「随心随意随缘」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/jn1158359135/article/details/7761011