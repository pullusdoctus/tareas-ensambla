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
    push rcx
    mov rcx, rbx
    call check_cell_state   ; returns new state in al
    mov byte [rdi + rbx], al
    pop rcx

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
    xor rbx, rbx            ; neighbor count
    mov rdx, rcx            ; current cell index

    ; Neighbor offsets: -11, -10, -9, -1, +1, +9, +10, +11
    mov rax, rdx
    sub rax, 11
    call count_if_alive
    mov rax, rdx
    sub rax, 10
    call count_if_alive
    mov rax, rdx
    sub rax, 9
    call count_if_alive
    mov rax, rdx
    dec rax
    call count_if_alive
    mov rax, rdx
    inc rax
    call count_if_alive
    mov rax, rdx
    add rax, 9
    call count_if_alive
    mov rax, rdx
    add rax, 10
    call count_if_alive
    mov rax, rdx
    add rax, 11
    call count_if_alive

    ; Current cell state
    movzx rax, byte [rsi + rdx]

    cmp rbx, 2
    jl .die

    cmp rbx, 3
    je .live

    cmp rbx, 3
    jg .die

.same:
    mov al, byte [rsi + rdx]
    jmp .end

.live:
    mov al, 1
    jmp .end

.die:
    mov al, 0

.end:
    pop rdx
    pop rbx
    ret

count_if_alive:
    ; Input: rax = neighbor index
    ; Uses: rsi = matrix base pointer, rbx = neighbor count
    push rdi
    push rbx
    cmp rax, 0
    jl .skip
    cmp rax, 99
    jg .skip
    movzx rdi, byte [rsi + rax]
    add rbx, rdi
.skip:
    pop rbx
    pop rdi
    ret
