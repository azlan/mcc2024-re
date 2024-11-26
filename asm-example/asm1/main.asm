include smallwin.inc

includelib msvcrt.lib
extern malloc:proc

.data
sum DWORD 1234h
message   byte "sum: %x",13h,0

.code

addNumber proc a:dword, b:dword
	mov eax, a
	add eax, b
	ret
addNumber endp


_main	proc
LOCAL local1: DWORD, local2: DWORD, pMemory: DWORD
	push 1
	push 2
	call addNumber
	xor eax, eax
	invoke printf, offset message, 12 ; using invoke macro
	push 10
	call malloc
	add esp, 4  ; cdecl
	mov pMemory, eax

	mov local1, 5
	mov local2, 6
	inc local1
	xor eax, eax
	mov ebx, sum
	mov eax, 3
	add eax, ebx
	mov sum, eax
	push sum
	push offset message
	call printf
	add esp, 8	; cdecl

	ret
_main endp

end _main