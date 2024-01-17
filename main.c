extern unsigned long sbi_putchar(char);
extern unsigned long sbi_shutdown();

char *chrdev = (char *)0x10000000;
char _buffer[64];

void read_from_mmio() {
    char *tmp = _buffer;
    do {
        *tmp = *chrdev;
    } while(*tmp++ != '\0');
}

void print_chardev(char* buffer) {
    do {
        *chrdev = *buffer;
        buffer++;
    } while(*buffer != '\0');
}

void print_sbi(char *buffer) {
    do {
        sbi_putchar(*buffer);
        buffer++;
    } while(*buffer != '\0');
}

void start() {
    print_chardev("hello world!\r\n");
    read_from_mmio();
    print_sbi(_buffer);
    sbi_putchar('\n');
    sbi_shutdown();
}