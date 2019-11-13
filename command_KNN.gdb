set remotetimeout 50000000
target remote localhost:3333
load
b classifyAPoint
cont
b 103
cont
x /1wx &answer
quit
