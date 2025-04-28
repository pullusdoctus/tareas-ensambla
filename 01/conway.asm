section .data
    matrix times 100 db 0  ; 10x10 matrix (100 elements)
    newline db 10          ; ASCII code for newline
    space db ' '           ; ASCII code for space

section .text
global _start

_start: 
    ; Initialize the matrix
    mov ecx, 100           ; Counter for 100 elements
    mov edi, matrix        ; Destination index pointing to matrix
    mov al, 1              ; Starting value (1)

fill_matrix: 
    mov [edi], al          ; Store the value (0 or 1) in the matrix
    xor al, 1              ; Toggle between 0 and 1
    inc edi                ; Move to next element
    loop fill_matrix       ; Repeat until ecx is 0

    ; Print the matrix
    mov ecx, 10            ; 10 rows
    mov esi, matrix        ; Source index pointing to matrix

print_row: 
    push ecx               ; Save row counter

    mov ecx, 10            ; 10 columns
print_column: 
    movzx eax, byte [esi]  ; Get the current element
    
    ; Convert number to ASCII and print
    add eax, '0'           ; Convert to ASCII
    push ecx               ; Save column counter
    mov [esi], al          ; Store ASCII value back
    
    mov eax, 4             ; sys_write
    mov ebx, 1             ; stdout
    mov ecx, esi           ; address of current element
    mov edx, 1             ; length to write
    int 0x80               ; call kernel
    
    ; Print space
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    pop ecx                ; Restore column counter
    inc esi                ; Move to next element
    loop print_column

    ; Print newline at end of row
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    pop ecx                ; Restore row counter
    loop print_row

    mov esi, matrix
    mov edi, 55  ; Example: check cell at position 55 (row 5, column 5)
    call check_cell_state
    ; Result is in al (0 or 1)

    ; Exit program
    mov eax, 1
    xor ebx, ebx
    int 0x80

; Function to check if a cell will be alive or dead in the next generation
; Input: esi = address of matrix, edi = cell index
; Output: al = next state (0 or 1)
check_cell_state:
    push ebx
    push ecx
    push edx
    
    ; Exit program
    mov eax, 1             ; sys_exit
    xor ebx, ebx           ; exit code 0
    int 0x80