//Full test
@R1
D=M
@i
M=0
(LOOP)
@R0
M=D
@END
D;JGE
@i
D=M-D
@LOOP
0;JMP
(END)
@END
0;JMP