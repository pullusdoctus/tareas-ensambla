section .data
    matrix times 100 db 0  ; 10x10 matrix (100 elements)
    newline db 10          ; ASCII code for newline
    space db ' '           ; ASCII code for space
    prompt db 'Type "quit" to exit, or any other key to continue...', 10
    prompt_len equ $ - prompt
    quit_msg db 'quit', 10 ; "quit" + newline
    ;sigint_msg db 10, 'Detected CTRL-C. Exiting...', 10
    ;sigint_len equ $ - sigint_msg

section .bss
    buffer resb 5           ; Store up to 4 chars + newline
    cell resb 1             ; a byte to store each cell's char
    ;sigact resb 152        ; sizeof(struct sigaction) = 152 bytes (64-bit)

section .text
global _start

_start: 
    ; Initialize the matrix
    mov ecx, 100           ; Counter for 100 elements
    mov edi, matrix        ; Destination index pointing to matrix
    mov al, 1              ; Starting value (1)

    ; Setup sigint handler
    ;mov dword [sigact], handle_sigint       ; sa_handler
    ;mov dword [sigact+8], 0x04000000       ; SA_RESTART flag
    ;mov eax, 13                            ; sys_rt_sigaction
    ;mov ebx, 2                             ; SIGINT
    ;mov ecx, sigact                        ; act
    ;mov edx, 0                             ; oldact (NULL)
    ;mov esi, 8                             ; sigsetsize
    ;int 0x80

fill_matrix: 
    mov [edi], al          ; Store the value (0 or 1) in the matrix
    xor al, 1              ; Toggle between 0 and 1
    inc edi                ; Move to next element
    loop fill_matrix       ; Repeat until ecx is 0

    ; Print initial matrix
    call print_matrix

simulation_loop:
    ; Clear input buffer
    call clear_buffer

    ; Print prompt
    mov eax, 4             ; sys_write
    mov ebx, 1             ; stdout
    mov ecx, prompt
    mov edx, prompt_len
    int 0x80

    ; Read user input
    mov eax, 3             ; sys_read
    mov ebx, 0             ; stdin
    mov ecx, buffer
    mov edx, 5             ; read up to 5 bytes (chars)
    int 0x80

    ; Check for 'quit'
    mov ecx, 4             ; compare first 4 bytes
    mov esi, quit_msg
    mov edi, buffer
    repe cmpsb
    je exit

    ; Update all cells
    mov esi, matrix
    mov edi, 0
    mov ecx, 100  ; 100 cells in total

update_cells:
    push ecx
    call check_cell_state
    mov [esi + edi], al  ; Store the new state
    inc edi
    pop ecx
    loop update_cells

    call print_matrix
    jmp simulation_loop

print_matrix:
    mov ecx, 10            ; 10 rows
    mov esi, matrix        ; Source index pointing to matrix

print_row: 
    push ecx               ; Save row counter
    mov ecx, 10            ; 10 columns

print_column: 
    movzx eax, byte [esi]  ; Get the current element
    
    ; Convert number to ASCII and print
    add al, '0'            ; Convert to ASCII
    mov [cell], al         ; move ASCII to cell buffer
    
    push ecx               ; Save column counter
    mov eax, 4             ; sys_write
    mov ebx, 1             ; stdout
    mov ecx, cell          ; address of current element
    mov edx, 1             ; length to write
    int 0x80               ; call kernel
    pop ecx                ; Restore column counter
    
    ; Print space
    push ecx               ; Save column counter
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    pop ecx                ; Restore column counter
    
    inc esi                ; Move to next element
    loop print_column

    ; Print newline at end of row
    push ecx               ; Save row counter
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    pop ecx                ; Restore row counter

    pop ecx                ; Restore rows-left counter (go to next row)
    loop print_row

    ret

    mov esi, matrix
    mov edi, 55  ; Example: check cell at position 55 (row 5, column 5)
    call check_cell_state
    ; Result is in al (0 or 1)

; Function to check if a cell will be alive or dead in the next generation
check_cell_state:
    push ebx
    push ecx
    push edx

    xor ecx, ecx  ; Initialize neighbor count to 0
    
    ; Check all 8 neighboring cells
    mov ebx, edi
    sub ebx, 11   ; Top-left
    call count_if_alive
    
    inc ebx       ; Top-center
    call count_if_alive
    
    inc ebx       ; Top-right
    call count_if_alive
    
    add ebx, 9    ; Middle-left
    call count_if_alive
    
    add ebx, 2    ; Middle-right
    call count_if_alive
    
    add ebx, 9    ; Bottom-left
    call count_if_alive
    
    inc ebx       ; Bottom-center
    call count_if_alive
    
    inc ebx       ; Bottom-right
    call count_if_alive

    ; Apply Conway's Game of Life rules
    movzx eax, byte [esi + edi]  ; Get current state of the cell
    cmp ecx, 2
    jl .die       
    je .same      
    cmp ecx, 3
    je .live      
    jg .die      

.same:

    jmp .end

.live:
    mov al, 1
    jmp .end

.die:
    mov al, 0

.end:
    pop edx
    pop ecx
    pop ebx
    ret

; Function to count live neighbors
count_if_alive:
    push eax
    
    
    cmp ebx, 0
    jl .skip
    cmp ebx, 99
    jg .skip
    
    movzx eax, byte [esi + ebx]
    add ecx, eax  ; Increment neighbor count if cell is alive (1)
    
.skip:
    pop eax
    ret

clear_buffer:
    mov edi, buffer
    mov al, 0
    mov ecx, 5
    rep stosb
    ret

exit:
    call clear_buffer
    mov eax, 1
    xor ebx, ebx
    int 0x80