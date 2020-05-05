
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        mov rax, rdx    ; move row value into rax
        mul rsi         ; multiply rax by rsi and store it into rdx:rax (row * row_width)
        add rax, rcx    ; add rcx to rax and store it in rax (row * row_width + col) to get bit index
        mov rcx, 8      ; set up 8 bit word size
        div rcx         ; divide rax by rcx (quotient is put into rax, remainder in rdx)
        add rdi, rax    ; offset the array pointer to the correct 8 bit address where this bit is located
        dec rcx         ; we need to use 8 for the shift base (# of left shifts = 8 - col)
        sub rcx, rdx    ; rcx now holds the # of left shifts needed for our mask in that byte
        mov rdx, 1      ; rdx will hold our mask value
        sal rdx, cl     ; we are left shifting '1' cl (first 8 bits of rcx) times, so rdx now holds the mask value
        or [rdi], rdx   ; apply the mask to that address to set the bit

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        mov rax, rdx    ; move row value into rax
        mul rsi         ; multiply rax by rsi and store it into rdx:rax (row * row_width)
        add rax, rcx    ; add rcx to rax and store it in rax (row * row_width + col) to get bit index
        mov rcx, 8      ; set up 8 bit word size
        div rcx         ; divide rax by rcx (quotient is put into rax, remainder in rdx)
        add rdi, rax    ; offset the array pointer to the correct 8 bit address where this bit is located
        dec rcx         ; we need to use 8 for the shift base (# of left shifts = 8 - col)
        sub rcx, rdx    ; rcx now holds the # of left shifts needed for our mask in that byte
        mov rdx, 1      ; rdx will hold our mask value
        sal rdx, cl     ; we are left shifting '1' cl (first 8 bits of rcx) times, so rdx now holds the mask value
        and rdx, [rdi]  ; if both bits are set, it will return a non-zero value
        cmp rdx, 0      ; check if non-zero
        setg al         ; put 1 or 0 into al based on non-zero
        movsx rax, al   ; put result into rax to return it

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
