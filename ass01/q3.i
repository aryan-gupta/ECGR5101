# 1 "q3.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "q3.c"


int decode2(int x, int y, int z) {
    register int s = y - z;
    register int r = s;
    r <<= 31;
    r >>= 31;
    r ^= x * s;
    return r;
}

int
main(int argc, char* argv[]) {
    return decode2(2, 3, 5);
}
