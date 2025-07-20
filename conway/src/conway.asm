section .data
    global matrix
    matrix times 100 db 0           ; 10x10 matrix (100 cells)

section .bss
    matrix_next resb 100

section .text
    global init_matrix
    global update_matrix

; Initialize matrix with alternating 0s and 1s
init_matrix:
    mov rcx, 100
    mov rdi, matrix
    mov al, 1
.fill_loop:
    mov [rdi], al
    xor al, 1
    inc rdi
    loop .fill_loop
    ret

; Update matrix for next generation using Game of Life rules
update_matrix:
    mov rbx, 0              ; index 0
    mov rsi, matrix         ; current matrix base
    mov rdi, matrix_next    ; next generation matrix base

.update_loop:
    push rbx
    push rdi
    mov rcx, rbx
    call check_cell_state   ; returns new state in al
    pop rdi
    mov byte [rdi + rbx], al
    pop rbx

    inc rbx
    cmp rbx, 100
    jne .update_loop

    ; Copy matrix_next to matrix
    mov rcx, 100
    mov rsi, matrix_next
    mov rdi, matrix

.copy_loop:
    mov al, [rsi]
    mov [rdi], al
    inc rsi
    inc rdi
    loop .copy_loop
    ret

; Calculate next cell state (Game of Life rules)
; Input: rcx = cell index
; Uses: rsi = matrix base pointer
; Output: al = new state (0 or 1)
check_cell_state:
    push rbx
    push rdx
    push r8
    xor rbx, rbx            ; neighbor count
    mov rdx, rcx            ; current cell index

    ; Calculate row and column for boundary checking
    mov rax, rdx
    mov r8, 10
    xor rdx, rdx
    div r8                  ; rax = row, rdx = col
    mov r8, rax             ; r8 = row
    mov r9, rdx             ; r9 = col
    mov rdx, rcx            ; restore original index

    ; Check all 8 neighbors with proper boundary checking
    ; Top-left (-11)
    mov rax, rdx
    sub rax, 11
    call count_neighbor
    
    ; Top (-10)
    mov rax, rdx
    sub rax, 10
    call count_neighbor
    
    ; Top-right (-9)
    mov rax, rdx
    sub rax, 9
    call count_neighbor
    
    ; Left (-1)
    mov rax, rdx
    dec rax
    call count_neighbor
    
    ; Right (+1)
    mov rax, rdx
    inc rax
    call count_neighbor
    
    ; Bottom-left (+9)
    mov rax, rdx
    add rax, 9
    call count_neighbor
    
    ; Bottom (+10)
    mov rax, rdx
    add rax, 10
    call count_neighbor
    
    ; Bottom-right (+11)
    mov rax, rdx
    add rax, 11
    call count_neighbor

    ; Current cell state
    movzx rax, byte [rsi + rdx]

    ; Apply Game of Life rules
    cmp rax, 1              ; Is current cell alive?
    je .check_alive_rules

.check_dead_rules:
    ; Dead cell: becomes alive only with exactly 3 neighbors
    cmp rbx, 3
    je .live
    jmp .die

.check_alive_rules:
    ; Live cell: survives with 2 or 3 neighbors
    cmp rbx, 2
    je .live
    cmp rbx, 3
    je .live
    jmp .die

.live:
    mov al, 1
    jmp .end

.die:
    mov al, 0

.end:
    pop r8
    pop rdx
    pop rbx
    ret

; Count neighbor if it's within bounds and alive
; Input: rax = neighbor index, r8 = current row, r9 = current col
; Uses: rsi = matrix base pointer
; Modifies: rbx (neighbor count)
count_neighbor:
    push rax
    push rcx
    push rdx
    
    ; Check if index is within bounds
    cmp rax, 0
    jl .skip
    cmp rax, 99
    jg .skip
    
    ; Calculate neighbor's row and col
    mov rcx, 10
    xor rdx, rdx
    div rcx                 ; rax = neighbor_row, rdx = neighbor_col
    
    ; Check if neighbor is adjacent (not wrapping around)
    ; Row difference should be at most 1
    sub rax, r8             ; neighbor_row - current_row
    cmp rax, -1
    jl .skip
    cmp rax, 1
    jg .skip
    
    ; Col difference should be at most 1
    sub rdx, r9             ; neighbor_col - current_col
    cmp rdx, -1
    jl .skip
    cmp rdx, 1
    jg .skip
    
    ; Restore original index and check if alive
    pop rdx
    pop rcx
    pop rax
    
    movzx rax, byte [rsi + rax]
    add rbx, rax
    ret

.skip:
    pop rdx
    pop rcx
    pop rax
    ret
