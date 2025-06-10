section .data
    global matrix
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
global main

main:
    ; Initialize the matrix
    mov rcx, 100           ; Counter for 100 elements
    mov rdi, matrix        ; Destination index pointing to matrix
    mov al, 1              ; Starting value (1)

    ; Setup sigint handler
    ;mov dword [sigact], handle_sigint       ; sa_handler
    ;mov dword [sigact+8], 0x04000000       ; SA_RESTART flag
    ;mov eax, 13                            ; sys_rt_sigaction
    ;mov ebx, 2                             ; SIGINT
    ;mov ecx, sigact                        ; act
    ;mov edx, 0                             ; oldact (NULL)
    ;mov esi, 8                             ; sigsetsize
    ;ret

fill_matrix: 
    mov [rdi], al          ; Store the value (0 or 1) in the matrix
    xor al, 1              ; Toggle between 0 and 1
    inc rdi                ; Move to next element
    loop fill_matrix       ; Repeat until ecx is 0

    ; Print initial matrix
    call print_matrix

simulation_loop:
    ; Clear input buffer
    call clear_buffer

    ; Print prompt
    mov rax, 4             ; sys_write
    mov rbx, 1             ; stdout
    mov rcx, prompt
    mov rdx, prompt_len
    ret

    ; Read user input
    mov rax, 3             ; sys_read
    mov rbx, 0             ; stdin
    mov rcx, buffer
    mov rdx, 5             ; read up to 5 bytes (chars)
    ret

    ; Check for 'quit'
    mov rcx, 4             ; compare first 4 bytes
    mov rsi, quit_msg
    mov rdi, buffer
    repe cmpsb
    je exit

    ; Update all cells
    mov rsi, matrix
    mov rdi, 0
    mov rcx, 100  ; 100 cells in total

update_cells:
    push rcx
    call check_cell_state
    mov [rsi + rdi], al  ; Store the new state
    inc rdi
    pop rcx
    loop update_cells

    call print_matrix
    jmp simulation_loop

extern draw_matrix

print_matrix:
    mov rdi, matrix
    call draw_matrix
    ret
    
print_row: 
    push rcx               ; Save row counter
    mov rcx, 10            ; 10 columns

print_column: 
    movzx rax, byte [esi]  ; Get the current element
    
    ; Convert number to ASCII and print
    add al, '0'            ; Convert to ASCII
    mov [cell], al         ; move ASCII to cell buffer
    
    push rcx               ; Save column counter
    mov rax, 4             ; sys_write
    mov rbx, 1             ; stdout
    mov rcx, cell          ; address of current element
    mov rdx, 1             ; length to write
    ret               ; call kernel
    pop rcx                ; Restore column counter
    
    ; Print space
    push rcx               ; Save column counter
    mov rax, 4
    mov rbx, 1
    mov rcx, space
    mov rdx, 1
    ret
    pop rcx                ; Restore column counter
    
    inc rsi                ; Move to next element
    loop print_column

    ; Print newline at end of row
    push rcx               ; Save row counter
    mov rax, 4
    mov rbx, 1
    mov rcx, newline
    mov rdx, 1
    ret
    pop rcx                ; Restore row counter

    pop rcx                ; Restore rows-left counter (go to next row)
    loop print_row

    ret

    mov rsi, matrix
    mov rdi, 55  ; Example: check cell at position 55 (row 5, column 5)
    call check_cell_state
    ; Result is in al (0 or 1)

; Function to check if a cell will be alive or dead in the next generation
check_cell_state:
    push rbx
    push rcx
    push rdx

    xor rcx, rcx  ; Initialize neighbor count to 0
    
    ; Check all 8 neighboring cells
    mov rbx, rdi
    sub rbx, 11   ; Top-left
    call count_if_alive
    
    inc rbx       ; Top-center
    call count_if_alive
    
    inc rbx       ; Top-right
    call count_if_alive
    
    add rbx, 9    ; Middle-left
    call count_if_alive
    
    add rbx, 2    ; Middle-right
    call count_if_alive
    
    add rbx, 9    ; Bottom-left
    call count_if_alive
    
    inc rbx       ; Bottom-center
    call count_if_alive
    
    inc rbx       ; Bottom-right
    call count_if_alive

    ; Apply Conway's Game of Life rules
    movzx rax, byte [rsi + rdi]  ; Get current state of the cell
    cmp rcx, 2
    jl .die       
    je .same      
    cmp rcx, 3
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
    pop rdx
    pop rcx
    pop rbx
    ret

; Function to count live neighbors
count_if_alive:
    push rax
    
    
    cmp rbx, 0
    jl .skip
    cmp rbx, 99
    jg .skip
    
    movzx rax, byte [rsi + rbx]
    add rcx, rax  ; Increment neighbor count if cell is alive (1)
    
.skip:
    pop rax
    ret

clear_buffer:
    mov rdi, buffer
    mov al, 0
    mov rcx, 5
    rep stosb
    ret

exit:
    call clear_buffer
    mov rax, 1
    xor rbx, rbx
    ret
