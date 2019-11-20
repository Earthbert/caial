set remotetimeout 50000000
target remote localhost:3333
load
b main
cont
b 71
cont
x /1wx &e
print e
quit