typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[], __stack_top[];
// extern 跨檔案宣告

void *memset(void *buf, char c, size_t n) {
    uint8_t *p = (uint8_t *) buf;
    while (n--)
        *p++ = c;
    return buf;
}

void kernel_main(void) {
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    for (;;);
    // 這是一個無限循環，相當於while(true).
}

__attribute__((section(".text.boot")))
__attribute__((naked))
// 属性告诉编译器不要在函数体前后生成不必要的代码

void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // 社置栈指针
        "j kernel_main\n"       // 跳转到内核主函数
        :
        : [stack_top] "r" (__stack_top) // 将栈顶地址作为 %[stack_top] 传递
    );
}