;https://stackoverflow.com/questions/6212665/why-is-eax-zeroed-before-a-call-to-printf

bits 64
default rel

; Constants

ROWS        equ 3
COLS        equ 3

EMPTY       equ 0
X_MARK      equ 1
O_MARK      equ -1

DRAW        equ 0
X_PLAYER    equ 1
O_PLAYER    equ -1

section .data

    visualGrid:
        db 1, 2, 3
        db 4, 5, 6
        db 7, 8, 9

    gameGrid:
        db EMPTY, EMPTY, EMPTY
        db EMPTY, EMPTY, EMPTY
        db EMPTY, EMPTY, EMPTY

    msg_draw:       db `You both drew, sorry, maybe next time\n\0`
    msg_x_wins:     db `Player X wins!\n\0`
    msg_o_wins:     db `Player O wins!\n\0`

    turn:           dd 0
    currentPlayer:  dd X_PLAYER
    input:          dd 0

section .text

global main

extern printf
extern scanf
extern getchar

main:

    sub rsp, 40

    mov dword [turn], 0
    mov dword [currentPlayer], X_PLAYER

.gameLoop:

    ; Print grid

    call printGrid

    ; Get player move

    mov ecx, [currentPlayer]
    call playerMove

    inc dword [turn]

    ; Check for winner

    mov ecx, [currentPlayer]
    call endGameCheck

    cmp eax, [currentPlayer]
    je .gameOver

    ; Check for draw

    cmp dword [turn], 9
    jge .gameDraw

    ; Switch player

    neg dword [currentPlayer]

    jmp .gameLoop

.gameDraw:

    mov dword [currentPlayer], DRAW
    jmp .gameOver

.gameOver:

    call printGrid

    mov ecx, [currentPlayer]
    call winScreen

    xor eax, eax

    add rsp, 40
    ret

winScreen:

    ; ecx = winner

    ; TODO
    ret

printGrid:

    ; TODO
    ret


getGridValue:

    ; ecx = row
    ; edx = column
    ; eax = returned character

    ; TODO
    ret

;--------------- EndGame checks ---------------
endGameCheck:
    ; ecx = player
    ; eax = winner or DRAW
    sub rsp, 40
    mov dword [rsp + 32], ecx

    ; Check horizontal
    mov ecx, [rsp + 32]
    lea rdx, [gameGrid]
    call endGameCheckHorizontal
    cmp eax, [rsp + 32]
    je .gameOver

    ; Check vertical
    mov ecx, [rsp + 32]
    lea rdx, [gameGrid]
    call endGameCheckVertical
    cmp eax, [rsp + 32]
    je .gameOver

    ; Check diagonal
    mov ecx, [rsp + 32]
    lea rdx, [gameGrid]
    call endGameCheckDiagonal
    cmp eax, [rsp + 32]
    je .gameOver

    ; No winner
    mov eax, DRAW
    jmp .gameOver

endGameCheckHorizontal:
    ; ecx = player
    ; rdx = pointer to gameGrid
    ; eax = player if won, DRAW otherwise

    ; First row
    cmp byte [rdx], cl
    jne .secondRow
    cmp byte [rdx + 1], cl
    jne .secondRow
    cmp byte [rdx + 2], cl
    jne .secondRow

    mov eax, ecx
    ret

.secondRow:
    ; Second row
    cmp byte [rdx + 3], cl
    jne .thirdRow
    cmp byte [rdx + 4], cl
    jne .thirdRow
    cmp byte [rdx + 5], cl
    jne .thirdRow

    mov eax, ecx
    ret

.thirdRow:
    ; Third row
    cmp byte [rdx + 6], cl
    jne .noWin
    cmp byte [rdx + 7], cl
    jne .noWin
    cmp byte [rdx + 8], cl
    jne .noWin

    mov eax, ecx
    ret

.noWin:
    mov eax, DRAW
    ret

endGameCheckVertical:
    ; ecx = player
    ; rdx = pointer to gameGrid
    ; eax = player if won, DRAW otherwise

    ; First column
    cmp byte [rdx], cl
    jne .secondColumn
    cmp byte [rdx + 3], cl
    jne .secondColumn
    cmp byte [rdx + 6], cl
    jne .secondColumn

    mov eax, ecx
    ret

.secondColumn:
    ; Second column
    cmp byte [rdx + 1], cl
    jne .thirdColumn
    cmp byte [rdx + 4], cl
    jne .thirdColumn
    cmp byte [rdx + 7], cl
    jne .thirdColumn

    mov eax, ecx
    ret

.thirdColumn:
    ; Third column
    cmp byte [rdx + 2], cl
    jne .noWin
    cmp byte [rdx + 5], cl
    jne .noWin
    cmp byte [rdx + 8], cl
    jne .noWin

    mov eax, ecx
    ret

.noWin:
    mov eax, DRAW
    ret

endGameCheckDiagonal:
    ; ecx = player
    ; rdx = pointer to gameGrid
    ; eax = player if won, DRAW otherwise

    ; Check centre position
    cmp byte [rdx + 4], cl
    jne .noWin

    ; Top-left to bottom-right
    cmp byte [rdx], cl
    jne .otherDiagonal
    cmp byte [rdx + 8], cl
    jne .otherDiagonal

    mov eax, ecx
    ret

.otherDiagonal:
    ; Top-right to bottom-left
    cmp byte [rdx + 2], cl
    jne .noWin
    cmp byte [rdx + 6], cl
    jne .noWin

    mov eax, ecx
    ret

.noWin:
    mov eax, DRAW
    ret

.gameOver:
    add rsp, 40
    ret

;--------------- EndGame checks ---------------


getPlayerMove:
    ; ecx = charPlayer
    ; eax = returned input
    ; Reserve shadow space (32) and (8) align stack
    sub rsp, 40
    mov dword [rsp + 32], ecx ; Store player character on the stack so that RCX can be used
.prompt: ;loop section, falls through
    
    ; Print player prompt
    lea rcx, [msg_prompt]
    mov edx, [rsp + 32]
    xor eax, eax ; no floating point arguments
    call printf
    
    ; Get user input
    lea rcx, [fmt_input]
    lea rdx, [input]
    call scanf

    ; Check whether scanf successfully read an integer
    cmp eax, 1
    je .checkMove

    ; Check for end-of-file
    cmp eax, -1
    je .inputEnded

    ; Invalid input: discard remaining characters
.flushInput:
    call getchar

    ; Stop discarding at a newline
    cmp eax, 10
    je .invalidInput

    ; Stop if end-of-file is reached
    cmp eax, -1
    je .inputEnded

    jmp .flushInput

.invalidInput:
    lea rcx, [msg_invalid]
    call printf
    jmp .prompt

.checkMove:
    ; Check if input is valid
    mov ecx, [input]
    call validPlayerMove

    cmp eax, 1
    je .valid

    ; Print invalid move message
    lea rcx, [msg_invalid]
    call printf
    jmp .prompt

.valid:
    mov eax, [input]
    add rsp, 40
    ret

.inputEnded:
    mov eax, -1
    add rsp, 40
    ret
validPlayerMove:
    ; ecx = input
    ; eax = 1 if valid, 0 otherwise
    ; Check input is at least 1
    cmp ecx, 1
    jl .invalid
    
    ; Check input is at most 9
    cmp ecx, 9
    jg .invalid
    
    ; Calculate grid index
    dec ecx
    
    ; Check if square is empty
    lea r8, [gameGrid]
    cmp byte [r8 + rcx], EMPTY
    jne .invalid

    ; Return 1 (valid)
    mov eax, 1
    ret

.invalid:

    ; Return 0 (invalid)

    xor eax, eax
    ret

; Input: ECX (currentPlayer)
; Output: EAX
getPlayer:
    ; ecx = player
    ; eax = returned character
    
    ; The `ret` statement returns to the last `call` statment not the last jmp
    ; This means that the function .playerX would return to the original call not back to getPlayer
    cmp ecx, X_PLAYER
    je .playerX
    
    ; Return O
    mov eax, 'O'
    ret

; Subfunction of getPlayer
; Input: ECX (currentPlayer)
; Output: EAX
.playerX:
    ; Return X
    mov eax, 'X'
    ret

; Input: ECX (currentPlayer)
; Uses: EAX, EDX, ECX, R8
; Edits: gameGrid
; Output: None
playerMove:
    ; Reserve shadow space (32) and (8) align stack becuse the return address is 8 bytes
    sub rsp, 40

    ; Get player character
    call getPlayer ; eax = returned character

    ; Get player move
    mov ecx, eax
    call getPlayerMove ; eax = returned input

    ; Calculate row and column
    dec eax ; num - 1

    ; Unlike C, the division with remainder commands are easier
    mov ecx, COLS
    
    ; Clear EDX because DIV uses EDX:EAX as the dividend
    xor edx, edx 
    div ecx ; eax = row, edx = column

    ; Calculate grid offset
    ; index in 1d array = row * 3 + column
    imul eax, COLS
    add eax, edx

    ; Store current player in gameGrid
    mov ecx, [currentPlayer]
    
    lea r8, [gameGrid] ; Grab address of gameGrid
    mov [r8 + rax], cl ; Increment address and place player into array

    ; Restore stack
    add rsp, 40
    ret