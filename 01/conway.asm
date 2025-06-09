section .data
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
global _start

_start:
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
    lea rsi, [quit_msg]
    lea rdi, [buffer]
    repe cmpsb
    je exit

    ; Update all cells
    mov rsi, matrix
    xor rdi, rdi           ; Start from index 0
    mov rcx, 100           ; 100 cells in total

update_cells:
    push rcx
    call check_cell_state
    and al, 1              ; Ensure the value is either 0 or 1
    mov [rsi + rdi], al    ; Store the new state
    inc rdi
    pop rcx
    loop update_cells

    call print_matrix
    jmp simulation_loop

print_matrix:
    mov rcx, 10            ; 10 rows
    mov rsi, matrix        ; Source index pointing to matrix

print_row:
    push rcx               ; Save row counter
    mov rcx, 10            ; 10 columns

print_column:
    movzx rax, byte [rsi]  ; Get the current element
    and al, 1              ; Ensure the value is either 0 or 1
    
    ; Convert number to ASCII and print
    add al, '0'            ; Convert to ASCII
    mov [cell], al         ; move ASCII to cell buffer
    
    push rcx               ; Save column counter
    mov rax, 1             ; sys_write
    mov rdi, 1             ; stdout
    mov rsi, cell          ; address of current element
    mov rdx, 1             ; length to write
    syscall
    pop rcx                ; Restore column counter
    
    ; Print space
    push rcx               ; Save column counter
    mov rax, 1
    mov rdi, 1
    mov rsi, space
    mov rdx, 1
    syscall
    pop rcx                ; Restore column counter
    
    inc rsi                ; Move to next element
    loop print_column

    ; Print newline at end of row
    push rcx               ; Save row counter
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall
    pop rcx                ; Restore row counter

    pop rcx                ; Restore rows-left counter (go to next row)
    loop print_row

    ret

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
    and al, 1                    ; Ensure current state is 0 or 1
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
    and al, 1              ; Ensure the value is either 0 or 1
    add rcx, rax           ; Increment neighbor count if cell is alive (1)
    
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
    mov rax, 60    ; sys_exit
    xor rdi, rdi   ; exit code 0
    syscall
