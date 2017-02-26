
inline void* yimemzero_sse2_asm(void* dest, int size)
{
	void* ret; 
#ifndef __x64__
asm volatile ("\n"
	"mov %1, %%ecx\n"
    "initbuff:\n"
	"pxor %%xmm0, %%xmm0\n"
    "memzero16:\n"
	"movdqa %%xmm0,(%%edi)\n"
	"lea 16(%%edi), %%edi\n"
	"sub $16, %%ecx\n"
	"jz memzero_end\n"
	"cmp $16, %%ecx\n"
	"jge memzero16\n"
    " memzero_end:\n"
	"mov %%edi, %0\n" 
	"    emms" :"=r"(ret): "r" (size), "D" (dest):"%ecx");

	return ret;
#else
asm volatile ("\n"
	"mov %0, %%rcx\n"
	"lea (%%rdi, %0), %%rbx\n"
	"cmp $16, %%rcx\n"
	"jb  memzero8\n" 
	"mov %1, %%rdx\n" 
	"mov $4, %%rax\n"
    "initbuff:\n"
	"pxor %%xmm0, %%xmm0\n"
    "memzero16:\n"
	"movdqa %%xmm0,(%%rdi)\n"
	"lea 16(%%rdi), %%rdi\n"
	"sub $16, %%rcx\n"
	"cmp $16, %%rcx\n"
	"jge memzero16\n"
	"cmp %%rbx, %%rdi\n"
	"je  memzero_end\n"
    "memzero8:\n"
	"xor  %%rax, %%rax\n"
    "memzero8_loop:\n"
	"cmp $8, %%rcx\n"
	"jb  memzero1_loop\n" 
	"mov %%rax, (%%rdi)\n"
	"lea 8(%%rdi), %%rdi\n"
	"cmp %%rbx, %%rdi\n"
	"jne memzero8_loop\n"
     "memzero1_loop:\n"
	"movb %%al, (%%rdi)\n"
	"lea 1(%%rdi), %%rdi\n"
	"cmp %%rbx, %%rdi\n"
	"jne memzero1_loop\n"
     "memzero_end:\n"
	"    emms" :: "r" (size), "D" (dest));

#endif
	return dest;
}


