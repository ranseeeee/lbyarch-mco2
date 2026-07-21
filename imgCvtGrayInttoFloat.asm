global imgCvtGrayInttoFloat
section .text

imgCvtGrayInttoFloat:
    ; Parameters:
    ; ecx = height (32-bit integer)
    ; edx = width (32-bit integer)
    ; r8  = input array pointer (const int*)
    ; r9  = output array pointer (float*)

    ; 1. Calculate total pixels = height * width
    movsxd r10, ecx         ; Sign-extend height to 64-bit register r10
    movsxd r11, edx         ; Sign-extend width to 64-bit register r11
    imul r10, r11           ; r10 = total_pixels

    ; If total pixels <= 0, exit function
    cmp r10, 0
    jle .done

    ; 2. Initialize constant divisor (255.0f) inside xmm1
    mov eax, 255
    cvtsi2ss xmm1, eax      ; Convert integer 255 to single-precision float 255.0f

    ; 3. Loop through and convert pixels
    xor rax, rax            ; Loop index i = 0

.loop:
    cmp rax, r10            ; Check if i >= total_pixels
    jge .done

    ; Load integer pixel value: input[i]
    mov edx, dword [r8 + rax*4]

    ; Convert integer pixel value to single-precision float
    cvtsi2ss xmm0, edx      ; xmm0 = (float)input[i]

    ; Map to 0.0-1.0 range: output = pixel / 255.0f
    divss xmm0, xmm1        ; xmm0 = xmm0 / xmm1

    ; Store the float pixel value into the output array: output[i]
    movss dword [r9 + rax*4], xmm0

    inc rax                 ; Move to the next pixel index
    jmp .loop

.done:
    ret