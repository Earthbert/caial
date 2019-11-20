set remotetimeout 50000000
target remote localhost:3333
load
b main
cont
b 302
cont
x /1wx &result
print result
quit