char *chrdev = (char *)0x10000000;

void print(char *str) {
    do {
        *chrdev = *str;
    } while ( *(str++) != 0 );

}
int _start() {
    print("hello world\r\n");
    while (1) {

    }
    return 0;
}