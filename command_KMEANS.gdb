set remotetimeout 50000000
target remote localhost:3333
load
b classify_kmeans
cont
b 233
cont
x /1wx &answer
print answer
quit
