/* extern (library) versions of inline functions defined in winnt.h */

#if defined(__GNUC__)

void* GetCurrentFiber(void)
{
    void* ret;
    __asm__ volatile (
	      "movl	%%fs:0x10,%0"
	      : "=r" (ret) /* allow use of reg eax,ebx,ecx,edx,esi,edi */
		  :	
		);
    return ret;
}

void* GetFiberData(void)
{
    void* ret;
    __asm__ volatile (            
	      "movl	%%fs:0x10,%0\n"
	      "movl	(%0),%0"
	      : "=r" (ret) /* allow use of reg eax,ebx,ecx,edx,esi,edi */
		  :	
	      );
    return ret;
}

#elif defined (__WATCOMC__)

void* __GetCurrentFiber(void);
#pragma aux __GetCurrentFiber = \
        "mov    eax, dword ptr fs:0x10" \
        value [eax] \
        modify [eax];

void* __GetFiberData(void);
#pragma aux __GetFiberData = \
        "mov    eax, dword ptr fs:0x10" \
        "mov    eax, [eax]" \
        value [eax] \
        modify [eax];
        
void* GetCurrentFiber(void)
{
   return __GetCurrentFiber();
}

void* GetFiberData(void)
{
   return __GetFiberData();
}

#else

void* GetCurrentFiber(void)
{
    void* res;
    _asm {
    	mov	eax, dword ptr fs:0x10
    	mov	res, eax
    };
    return res;
}

void* GetFiberData(void)
{
    void* res;
    _asm {
	mov	eax, dword ptr fs:0x10
	mov	eax, [eax]
	mov	res, eax
    };
    return res;
}

#endif /* __GNUC__ */
