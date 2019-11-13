set remotetimeout 50000000
target remote localhost:3333
load
b viete
cont
b 166
cont
x /1wx &pi
print pi
quit
