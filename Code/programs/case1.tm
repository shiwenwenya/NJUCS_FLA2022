; This example program realizes the circular right shift operation of binary numbers
; Input: a string of 0's and 1's, e.g. '1001001'

; the finite set of states
#Q = {0,s1,s2,s3,s4,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

; the transition functions
0 0 _ r s1
0 1 _ r s2

s1 0 0 r s1
s1 1 0 r s2
s1 _ _ l s3

s2 0 1 r s1
s2 1 1 r s2
s2 _ _ l s4

s3 0 0 l s3
s3 1 1 l s3
s3 _ 0 * halt

s4 0 0 l s4
s4 1 1 l s4
s4 _ 1 * halt