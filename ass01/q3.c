// compiled with: gcc q3.c -S -save-temps -m32 -march=i386 -O0


// @note register keyword used to force gcc to keep s and r
//       in registers and not write them back to memory
int
decode2(int x, int y, int z) {
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




/* lines 19-27

movl	12(%ebp), %eax      ; move y into %eax
subl	16(%ebp), %eax      ; subtract z from y
movl	%eax, %esi          ; copy result to %esi (s)
movl	%esi, %ebx          ; copy s to %ebx (r)
sall	$31, %ebx           ; shift left r
sarl	$31, %ebx           ; shift right r
movl	%esi, %eax          ; move s into %eax
imull	8(%ebp), %eax       ; multiply x with s
xorl	%eax, %ebx          ; xor result and r

*/