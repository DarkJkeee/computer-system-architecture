; Домашнее задание по обработке одномерных массивов на FASM.
; Вариант 5. Составить массив B из элементов A, значение которых не совпадает с введённым числом x
; Студент БПИ192
; Бурштейн Глеб

format PE console

entry start
include 'win32a.inc'

section '.data' data readable writable

        strInputSize db 'Input size of array: ', 0
        strInputArr  db 'Input element A[%d]: ', 0
        strInputX    db  newLine, 'Input x: ', 0
        strArrA      db  newLine, 'A = [', 0
        strArrB      db 'B = [', 0
        strEndArr    db '%d]', newLine, 0
        numFormatArr db '%d, ', 0
        numFormat    db '%d', 0
        emptyArr     db ']', 0
        exception    db 'Something is wrong!', 0

        arrA     dd 100 dup (?)
        arrB     dd 100 dup (?)
        sizeA    dd ?
        sizeB    dd ?
        x        dd ?
        element  dd ?

        NULL = 0
        newLine = 0xA

section '.code' code readable executable

        start:

                call InputArr ; Ввод массива A

                ; Ввод x
                push strInputX
                call [printf]
                push x
                push numFormat
                call [scanf]

                call createB ; Создание массива B по условию из варианта

                call printArrA ; Вывод массива A
                call printArrB ; Вывод массива B

                call finish ; Завершение программы и передача управления ОС

        ; Ввод массива A
        InputArr:
                push strInputSize
                call [printf]
                add esp, 4

                push sizeA
                push numFormat
                call [scanf]
                add esp, 8

                cmp [sizeA], 0
                jle except

                xor ebx, ebx

                inputLoop:
                        push ebx
                        push strInputArr
                        call [printf]
                        add esp, 8

                        push element
                        push numFormat
                        call [scanf]
                        add esp, 8

                        mov edx, [element]
                        mov [arrA + 4*ebx], edx
                        inc ebx
                        cmp ebx, [sizeA]
                        jne inputLoop
        ret

        ; Создание массива B на основе условия варианта
        createB:
                xor edx, edx
                xor ebx, ebx
                mov eax, [x]
                mov esi, [sizeA]

                skipElement:
                        cmp ebx, esi
                        je finishCreating
                        cmp eax, [arrA + 4*ebx]
                        jne addElement

                        inc ebx
                        jmp skipElement

                addElement:
                        mov ecx, [arrA + 4*ebx]
                        mov [arrB + 4*edx], ecx
                        inc ebx
                        inc edx
                        jmp skipElement

                finishCreating:
                        mov[sizeB], edx
        ret

        ; Вывод массива A
        printArrA:
                xor ebx, ebx
                mov esi, [sizeA]
                dec esi
                push strArrA
                call [printf]
                add esp, 4
                cmp ebx, esi
                je endPrintA
                printLoopA:
                        push [arrA + 4*ebx]
                        push numFormatArr
                        call [printf]
                        add esp, 8
                        inc ebx
                        cmp ebx, esi
                        jl printLoopA

                endPrintA:
                push [arrA + 4*ebx]
                push strEndArr
                call [printf]
                add esp, 8
        ret

        ; Вывод массива B
        printArrB:
                xor ebx, ebx
                mov esi, [sizeB]
                push strArrB
                call [printf]
                add esp, 4
                cmp esi, 0
                je emptyArray
                dec esi
                cmp ebx, esi
                je endPrintB
                printLoopB:
                        push [arrB + 4*ebx]
                        push numFormatArr
                        call [printf]
                        add esp, 8
                        inc ebx
                        cmp ebx, esi
                        jl printLoopB

                endPrintB:
                push [arrB + 4*ebx]
                push strEndArr
                call [printf]
                add esp, 8
        ret

        ; Вывод в случае пустого массива
        emptyArray:
                push emptyArr
                call [printf]
                jmp finish

        ; Вывод в случае некорректных данных
        except:
                push exception
                call [printf]
                jmp finish

        ; Корректное завершение программы
        finish:
                call [getch]
                push NULL
                call [ExitProcess]


section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'