#include <stdio.h>

int return_int(char c)
{
    int i;
    if (c >= '0' && c <= '9') {
        i = (int)c - 48;
    } else if (c >= 'A' && c <= 'Z') {
        i = (int)c - 54;
    } else if (c >= 'a' && c <= 'z') {
        i = (int)c - 86;
    } else if (c == '!') {
        i = 31415;
    } else {
        i = -1;
    }
    return i;
}

int main (void)
{
    char x = 'b', y = '1', z = 'A';
    int ix, iy, iz;

    ix = return_int(x); iy = return_int(y);
    printf("%d\n", 'A'-54);
    printf("%d %d %d", ix, iy, return_int(z));
    ix = return_int('!'); iy = return_int('@'); iz = return_int(' ');
    printf("\n%d %d %d", ix, iy, iz);

    return 0;

}


