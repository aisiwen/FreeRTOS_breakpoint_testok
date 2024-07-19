# 单片机2个调试绝招_修改bin实现多个断点调通---(来源liuliu)

## 1. 背景

韦东山老师直播公开课:`单片机开发过程中的2个调试绝招`,修改bin实现多断点在视频最后没调完.
现将个人调通代码分享.
韦老师bilibili视频地址:【单片机开发过程中的调试绝招】 https://www.bilibili.com/video/BV1bB4y1q72a/?p=12&share_source=copy_web&vd_source=ea803f03a31a6fcb973a5fb165cb0f3b

GIT仓库：https://e.coding.net/weidongshan/livestream/doc_and_source_for_livestream.git
目录：doc_and_source_for_livestream\20220623‎_单片机开发过程中的2个调试绝招

## 2. 实现原理
*  在异常处理函数中增加的栈指针,退出函数时栈指针要恢复;这就是为什么汇编中PUSH与POP成对存在;
*  startup_stm32f103xe.s/ SVC_Handler_asm函数中 栈新增加 `r4-r11,lr,lr`,退出异常处理时栈针要恢复异常进入的位置;
*  在stm32f1xx_it.c / 函数 svc_exception 模拟POP指令栈帧上移,上移offset值要记录;
*  startup_stm32f103xe.s/ SVC_Handler_asm函数中将栈指针指向(异常进入的位置 + 上移offset值)


## 3. 代码修改

* startup_stm32f103xe.s/ SVC_Handler_asm函数中将栈指针指向(异常进入的位置 + 上移offset值),修改如下:
```
;前面数字指代码中的位置
239: mov r3, r0                      ;保存异常产生的sp值
257: PUSH	{R3,LR}                  ;将R3/LR入栈
258: BL		svc_exception
259: POP 	{R3,LR}
260: add r1, r0, r3                  ;因模拟POP,将栈针sp上移
	 TST 	lr, #0x04				; if(!EXC_RETURN[2])
	 ITE 	EQ
	 MSREQ	msp, r1  
	 MSRNE	psp, r1 				
				
     ;ORR 	lr, lr, #0x04       ;注释掉,异常返回不能设置为进程栈
```
* 为方便调试,在startup_stm32f103xe.s/ Reset_Handler函数main之前中插入SVC指令,这样可以用KEIL调试;   具体如下:

  注意:将stm32f1xx_it.c / 函数 svc_exception 中串口打印先注释,不然串口打印未初始化报错; 也可以在SVC指令前先串口初始化;

  ```
  151: ;SVC #3                     ;用于SVC调试
        LDR     R0, =__main
        BX      R0
  ```

* 在stm32f1xx_it.c / 函数 svc_exception 模拟POP指令栈帧上移,上移offset值传回汇编,需要1个返回值,具体如下:

  ```
  unsigned int svc_exception(struct exception_info * exception_info)
  {
  
  308:  return offset;
  }
  ```

  

 