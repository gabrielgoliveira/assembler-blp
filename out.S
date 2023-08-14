.globl f3
f3:
pushq %rbp
movq  %rsp, %rbp
subq $80, %rbp

# ========== REGISTRADORES E PARAMETROS ==========
# pi1 => %edi
# pi2 => %esi
# vr4 => r11
# vr6 => r10
# ========== END  ===================================


## ----------- STACK ---------------
## vi5 => -4(%rbp)
## va3 => -44(%rbp)
## ----------- END STACK -----------

movl %edi, -4(%rbp)
cmpl edi, -4(%rbp)
jle end_inf1
movl %r11, %r10
subl -4(%rbp), %r10
end_if1:


movslq $2, %rcx
imulq $4, %rcx
leaq -44(%rbp), %rcx
movl (%rcx), %rax
movl %rax, -4(%rbp)



movslq $2, %rcx
imulq $4, %rcx
leaq -44(%rbp), %rcx
movl -4(%rbp), %rax
movl %rax, (%rcx)

movl %r11, %rdi
movl edi, %rsi
movl %eax, -4(%rbp)
movl %edi, -4(%rbp)
movl $5, %eax
leave
ret
