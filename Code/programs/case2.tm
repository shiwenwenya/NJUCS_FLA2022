; This example program checks if the length of a string of 1's is N^2
; Input: a string of 1's, e.g. '111111'

; the finite set of states
#Q = {0,s1,s2,s3,s4,accept,accept1,accept2,accept3,accept4,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 __ __ l* accept
0 1_ 11 ** s1

; s1
s1 11 11 rr s1
s1 __ __ l* accept
s1 _1 _1 l* reject
s1 1_ 1_ ** s2

; s2
s2 1_ 11 *r s3

; s3
s3 1_ 11 ** s4

; s4
s4 11 11 *l s4
s4 1_ 1_ *r s1

; State accept*: write 'true' on 1st tape
accept 11 __ l* accept
accept 1_ __ l* accept
accept __ __ r* accept1
accept _1 __ r* accept1
accept1 __ t_ r* accept2
accept1 _1 t_ r* accept2
accept2 __ r_ r* accept3
accept2 _1 r_ r* accept3
accept3 __ u_ r* accept4
accept3 _1 u_ r* accept4
accept4 __ e_ ** halt_accept
accept4 _1 e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject 11 __ l* reject
reject 1_ __ l* reject
reject _1 __ r* reject1
reject __ __ r* reject1
reject1 __ f_ r* reject2
reject1 _1 f_ r* reject2
reject2 __ a_ r* reject3
reject2 _1 a_ r* reject3
reject3 __ l_ r* reject4
reject3 _1 l_ r* reject4
reject4 __ s_ r* reject5
reject4 _1 s_ r* reject5
reject5 __ e_ ** halt_reject
reject5 _1 e_ ** halt_reject