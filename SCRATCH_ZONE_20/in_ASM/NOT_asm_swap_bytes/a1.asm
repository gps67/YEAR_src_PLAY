
format ELF

include 'cdecl.inc'

section '.text' executable

 public main
 extrn printf
 extrn getpid

xFF = 0xFFFFFFFF ; biggest integer in UN-signed range
x80 = 0x80000000 ; smallest negative number
x7F = 0x7FFFFFFF ; bigest int not in signed range
x00 = 0x00000000 ; zero
x01 = 0x00000001 ; one

I_HI = x7F
I_LO = x80
U_HI = xFF
U_LO = x00

macro test1 op1, opstr, LHS, RHS {

	mov ebx, LHS
	mov ecx, RHS

	mov eax, ebx
	op1 eax, ecx

	call flags_take
	mov edx, opstr
	call print_b_dstr_c_a_flags
}

macro test2 op1, opstr, hi, lo {
	test1 op1, opstr, lo, lo
	test1 op1, opstr, hi, hi
	test1 op1, opstr, hi, lo
	test1 op1, opstr, lo, hi
	test1 op1, opstr, hi, x01
	test1 op1, opstr, lo, x01
	call blank_line
}

macro test3 op1, opstr {
	test2 op1, opstr, I_HI, I_LO
	test2 op1, opstr, U_HI, U_LO
	test2 op1, opstr, U_HI, I_HI
}

blank_line:
	ccall printf, str_blank_line
	ret

 main:
	test3 sub, s_sub
	test3 add, s_add

	; exit code ...
	xor eax, eax
	; mov eax, 6
	ret


UPPER_FROM_LOWER = 'A'-'a'
; LOWER_FROM UPPER = 'a'-'A'

flags_prep:
	mov BYTE [flags_z], 'z'
	mov BYTE [flags_s], 's'
	mov BYTE [flags_c], 'c'
	mov BYTE [flags_o], 'o'

	mov BYTE [flags_z], '-'
	mov BYTE [flags_s], '-'
	mov BYTE [flags_c], '-'
	mov BYTE [flags_o], '-'
	ret

flags_take:
	call flags_prep
	push eax
	lahf	; load AH from FLAGS - S Z A P C - NOT 'o'

	; xor ah, ah
	; mov ah, 0xff
	; jmp flag_nz

	JNO flag_no
	mov BYTE [flags_o], 'O'
	sahf	; save AH to FLAGS // ie restore after preserve!
 flag_no:

	JNS flag_ns
	mov BYTE [flags_s], 'S'
	sahf	; save AH to FLAGS // ie restore after preserve!
 flag_ns:

	JNC flag_nc
	mov BYTE [flags_c], 'C'
	sahf	; save AH to FLAGS // ie restore after preserve!
 flag_nc:

	JNZ flag_nz
	mov BYTE [flags_z], 'Z'
	sahf	; save AH to FLAGS // ie restore after preserve!
 flag_nz:

	pop eax
	ret


print_b_dstr_c_a_flags:
	; and print
	ccall printf, msg, ebx, edx, ecx, eax, flags_str
	ret

section '.data' writeable

; msg db "B = %8.8x %s",0xA,"C = %8.8x",0xA,"A = %8.8x ",0xA,"F = %s",0xA, 0xA,0
msg db " %8.8x %3s %8.8x ==> %8.8x %s", 0xA,0
flags_str db '{' ; "FLAGS: "
flags_s db "-"
flags_z db "-"
flags_o db "-"
flags_c db "-"
	db '}'
	db 0

str_blank_line db 0x0A, 0
s_sub db "SUB", 0
s_add db "ADD", 0

