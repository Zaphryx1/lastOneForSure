
    .text
    .global score_increment
    .align 2

score_increment:
    PUSH R5             ; Save registers
    MOV R12, R5         ; Move input value to R5
    CALL #increment_fn  ; Call the helper function
    MOV R5, R12         ; Return updated value in R12
    POP R5              ; Restore registers
    RET

increment_fn:
    ADD #1, R5          ; Increment the value
    RET
