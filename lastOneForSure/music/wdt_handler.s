
.text
    .balign 2          ; Align to an even address
    .global WDT
    ; WDT uses interrupt #11
    .section __interrupt_vector_11,"ax"
    .word WDT          ; Entry for vector table
    .text

    .extern play_next_note

WDT:
    ; Prologue
    PUSH R15

    ; Call play_next_note() to process the current note
    CALL #play_next_note

    ; Epilogue
    POP R15
    RETI
