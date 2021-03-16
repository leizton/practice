# -fstack-protector-strong

栈空间溢出检查

# c++源码
```c++
int bob() {
  int a[2] = {10, 11};
  *(a+2) = 12;
  return 15;
}
```

# 对比有无‘-fstack-protector-strong’的汇编代码

$ g++ -S main.cc -fno-stack-protector

```s
__Z3bobv:
LFB0:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	movl	$10, -8(%rbp)
	movl	$11, -4(%rbp)
	movl	$12, 0(%rbp)
	movl	$15, %eax
	popq	%rbp
LCFI2:
	ret
```

$ g++ -S main.cc -fstack-protector-strong

```s
__Z3bobv:
LFB0:
	pushq	%rbp                           ; 把上一个函数的栈底指针(rbp)压栈
LCFI0:
	movq	%rsp, %rbp                     ; 把上一个函数的栈顶指针(rsp)赋给rbp, 变成当前函数的栈底指针
LCFI1:
	subq	$16, %rsp
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	movq	%rcx, -8(%rbp)
	xorl	%ecx, %ecx
  ;
	movl	$10, -16(%rbp)
	movl	$11, -12(%rbp)
	movl	$12, -8(%rbp)
	movl	$15, %eax
  ;
	movl	%eax, %edx
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	-8(%rbp), %rsi
	subq	(%rax), %rsi
	je	L3
	call	___stack_chk_fail
L3:
	movl	%edx, %eax
	leave
LCFI2:
	ret
```