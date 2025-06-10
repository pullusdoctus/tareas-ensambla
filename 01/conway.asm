section .data
    global matrix
    matrix times 100 db 0  ; 10x10 matrix (100 elements)
    newline db 10          ; ASCII code for newline
    space db ' '           ; ASCII code for space
    prompt db 'Type "quit" to exit, or any other key to continue...', 10
    prompt_len equ $ - prompt
    quit_msg db 'quit', 10 ; "quit" + newline

section .bss
    buffer resb 5           ; Store up to 4 chars + newline
    cell resb 1             ; a byte to store each cell's char

section .text
global main

extern draw_matrix

main:
    ; Initialize the matrix
    mov rcx, 100           ; Counter for 100 elements
    mov rdi, matrix        ; Destination index pointing to matrix
    mov al, 1              ; Starting value (1)

fill_matrix:
    mov [rdi], al          ; Store the value (0 or 1) in the matrix
    xor al, 1              ; Toggle between 0 and 1
    inc rdi                ; Move to next element
    loop fill_matrix       ; Repeat until rcx is 0

    ; Print initial matrix
    call print_matrix

simulation_loop:
    ; Clear input buffer
    call clear_buffer

    ; Print prompt
    mov rax, 1             ; sys_write
    mov rdi, 1             ; stdout
    mov rsi, prompt
    mov rdx, prompt_len
    syscall

    ; Read user input
    mov rax, 0             ; sys_read
    mov rdi, 0             ; stdin
    mov rsi, buffer
    mov rdx, 5             ; read up to 5 bytes (chars)
    syscall

    ; Check for 'quit'
    mov rcx, 4             ; compare first 4 bytes
    mov rsi, quit_msg
    mov rdi, buffer
    repe cmpsb
    je exit

    ; Update all cells
    mov rsi, matrix
    xor rdi, rdi
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

print_matrix:
    mov rdi, matrix
    call draw_matrix
    ret

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
    mov rax, 60    ; sys_exit
    xor rdi, rdi   ; status 0
    syscall
