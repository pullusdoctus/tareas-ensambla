section .bss
    global matrix
    matrix resb 100          ; 10x10 matrix

section .bss
    matrix_next resb 100     ; temporary buffer for next state

section .text
    global init_matrix
    global update_matrix

; Initialize matrix with alternating pattern (0,1,0,1,...)
init_matrix:
    mov rcx, 100
    mov rdi, matrix
    mov al, 1

.init_loop:
    mov [rdi], al
    xor al, 1
    inc rdi
    loop .init_loop
    ret

; update_matrix: computes next generation into matrix_next and copies back to matrix
update_matrix:
    ; rdi = matrix base
    ; We'll hardcode matrix base as global label 'matrix'
    ; Save registers used
    push rbx
    push rsi
    push rdi
    push rcx
    push rdx

    mov rsi, matrix          ; current matrix
    mov rdi, matrix_next     ; next matrix buffer
    mov rcx, 100             ; 100 cells
    xor rbx, rbx             ; index i = 0

.next_cell:
    ; Calculate alive neighbors for cell rbx
    xor rdx, rdx             ; rdx = neighbor count

    ; Compute row and col of cell
    mov r8, rbx              ; r8 = index
    mov r9, 10
    xor r10, r10
    div r9                   ; rax = quotient (row), rdx = remainder (col)
    mov r10, rax             ; row
    mov r11, rdx             ; col

    ; neighbors relative coords:
    ; (-1,-1) (-1,0) (-1,1)
    ; (0,-1)          (0,1)
    ; (1,-1)  (1,0)  (1,1)

    mov r12, r10            ; row copy
    mov r13, r11            ; col copy

    ; Check all 8 neighbors:
    ; We'll write a macro below for checking neighbor validity and adding count

    ; Macro equivalent inline:
    ; For each neighbor delta (dr, dc):
    ;   nr = row + dr
    ;   nc = col + dc
    ;   if 0 <= nr < 10 and 0 <= nc < 10 then
    ;      idx = nr*10 + nc
    ;      add matrix[idx] to rdx

    ; We'll do this 8 times:

    ; dr = -1, dc = -1
    mov rax, r10
    dec rax
    cmp rax, -1
    jle .skip_n1
    mov rdi, r11
    dec rdi
    cmp rdi, -1
    jle .skip_n1
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n1:

    ; dr = -1, dc = 0
    mov rax, r10
    dec rax
    cmp rax, -1
    jle .skip_n2
    mov rdi, r11
    cmp rdi, 10
    jge .skip_n2
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n2:

    ; dr = -1, dc = 1
    mov rax, r10
    dec rax
    cmp rax, -1
    jle .skip_n3
    mov rdi, r11
    inc rdi
    cmp rdi, 10
    jge .skip_n3
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n3:

    ; dr = 0, dc = -1
    mov rax, r10
    cmp rax, 10
    jge .skip_n4
    mov rdi, r11
    dec rdi
    cmp rdi, -1
    jle .skip_n4
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n4:

    ; dr = 0, dc = 1
    mov rax, r10
    cmp rax, 10
    jge .skip_n5
    mov rdi, r11
    inc rdi
    cmp rdi, 10
    jge .skip_n5
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n5:

    ; dr = 1, dc = -1
    mov rax, r10
    inc rax
    cmp rax, 10
    jge .skip_n6
    mov rdi, r11
    dec rdi
    cmp rdi, -1
    jle .skip_n6
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n6:

    ; dr = 1, dc = 0
    mov rax, r10
    inc rax
    cmp rax, 10
    jge .skip_n7
    mov rdi, r11
    cmp rdi, 10
    jge .skip_n7
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n7:

    ; dr = 1, dc = 1
    mov rax, r10
    inc rax
    cmp rax, 10
    jge .skip_n8
    mov rdi, r11
    inc rdi
    cmp rdi, 10
    jge .skip_n8
    mov rsi, matrix
    mov rax, rax
    imul rax, 10
    add rax, rdi
    movzx rbx, byte [rsi + rax]
    add rdx, rbx
.skip_n8:

    ; Current cell state
    mov rsi, matrix
    movzx rbx, byte [rsi + rbx]

    ; Apply Game of Life rules:
    ; alive neighbors = rdx
    ; current state = rbx (0 or 1)
    cmp rbx, 1
    jne .dead_cell
    ; Cell alive
    cmp rdx, 2
    jl .dead_next
    cmp rdx, 3
    jg .dead_next
    mov al, 1
    jmp .store_result

.dead_cell:
    ; Cell dead
    cmp rdx, 3
    jne .dead_next
    mov al, 1
    jmp .store_result

.dead_next:
    mov al, 0

.store_result:
    mov rsi, rdi             ; matrix_next
    mov rdi, rbx             ; current index (reuse rbx as index)
    mov byte [rsi + rbx], al

    inc rbx
    cmp rbx, 100
    jne .next_cell

    ; Copy matrix_next back to matrix
    mov rsi, matrix_next
    mov rdi, matrix
    mov rcx, 100
    rep movsb

    pop rdx
    pop rcx
    pop rdi
    pop rsi
    pop rbx
    ret
