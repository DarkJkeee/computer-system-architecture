format PE console
entry start

include 'win32a.inc'

section '.data' data readable writable

        inputMsg   db 'Input a number: ', 0    ; message with info for user
        answerMsg  db 'Answer: %f', newLine, 0 ; string for format output
        formatF    db '%lf', 0 ; format for float numbers
        rootDegree dq 3.0      ; degree
        eps        dq 0.0005   ; percent of algorithm precision
        one        dq 1.0      ; value 1.0 for intermediate calculations in algorithm

        number     dq ?        ; reserve memory for input number
        result     dq ?        ; reserve memory for answer
        rn         dq ?        ; reserve memory for intermediate calculations
        temp       dq ?        ; reserve memory for intermediate calculations

        newLine = 0xA          ; const for new line char
        NULL    = 0            ; NULL


section '.code' code readable executable
start:
        FINIT

        invoke printf, inputMsg           ; write message for user
        invoke scanf, formatF, number     ; request number from user
        ; set requirement values
        FLD  [number]
        FST  [rn]
        FDIV [rootDegree]
        FSTP [result]

        cycle:
                ; check for precision (if |result - rn| < 0.0005 then finish the program...)
                FLD  [result]
                FSUB [rn]
                FABS
                FCOMP [eps]
                FSTSW AX
                SAHF
                jb finish

                ; iteration algorithm
                FLD  [number]
                FDIV [result]
                FDIV [result]
                FSTP [rn]
                FLD  [one]
                FDIV [rootDegree]
                FSTP [temp]
                FLD  [one]
                FSUB [temp]
                FSTP [temp]
                FLD  [result]
                FMUL [temp]
                FSTP [temp]
                FLD  [rn]
                FDIV [rootDegree]
                FADD [temp]

                FSTP [result]
                jmp cycle


finish:

        invoke printf, answerMsg, dword[result], dword[result + 4] ; print answer
        ; finish the program correctly
        invoke getch
        invoke ExitProcess, NULL



section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll',\
                user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
        import kernel,\
               ExitProcess, 'ExitProcess'

include 'api\kernel32.inc'
        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'