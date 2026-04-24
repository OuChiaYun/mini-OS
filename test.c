typedef unsigned char uint8_t;            // 定義 8 位元無號整數型別，別名為 uint8_t
typedef unsigned int uint32_t;            // 定義 32 位元無號整數型別，別名為 uint32_t（在 32-bit 環境通常等於 unsigned int）
typedef uint32_t size_t;                  // 將 size_t 另行定義為 uint32_t（注意：一般標準庫中 size_t 依平台而異，這裡為簡化）

extern char __bss[], __bss_end[], __stack_top[];  // 從連結器載入的符號：.bss 起始、結束、堆疊頂端地址（以 char* 表示位址）

void *memset(void *buf, char c, size_t n) {       // 自行實作簡化版 memset：把連續 n 個位元組設成 c
    uint8_t *p = (uint8_t *) buf;                 // 以位元組指標走訪緩衝區
    while (n--)                                   // 當 n 尚未耗盡，每次遞減 1
        *p++ = c;                                 // 寫入一個位元組 c，指標遞增到下一個位元組
    return buf;                                   // 回傳原始指標（與標準 memset 行為一致）
}

void kernel_main(void) {                          // 核心進入點（C 層級），由 boot 例程跳轉進來
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);  
                                                  // 將 .bss 區段清零：範圍 = __bss_end - __bss（位址差）
                                                  // 依賴連結器腳本保證這兩個符號指向連續的 .bss 邊界

    for (;;);                                     // 無限迴圈，避免 core main 返回；等待中斷/排程器或後續初始化接手
}

__attribute__((section(".text.boot")))            // 指示編譯器把此函式放到 .text.boot 區段（常用於啟動程式碼靠前排）
__attribute__((naked))                            // 裸函式：不產生函式前/後言（prologue/epilogue），適合手動控管堆疊
void boot(void) {                                 // 最早期的啟動入口，由 reset/啟動向量跳來
    __asm__ __volatile__(                         // 內嵌組合語言；volatile 防止編譯器重新排序或刪除
        "mv sp, %[stack_top]\n"                   // RISC-V：把 sp 設為堆疊頂端；mv 是 addi 的別名（addi sp, x, 0）
                                                   // 此步驟在 naked 函式中很重要：必須自行建立有效堆疊
        "j kernel_main\n"                         // 無條件跳躍到 kernel_main；j 是 jal x0, kernel_main 的別名（不保存返回位址）
                                                   // 直接移交控制權給 C 層級主程序
        :
        : [stack_top] "r" (__stack_top)           // 輸入約束：請求把 __stack_top 放到暫存器（"r"），命名為 %[stack_top]
    );                                            // 組合語句結束
}
