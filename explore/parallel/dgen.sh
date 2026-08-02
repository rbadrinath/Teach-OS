# A small script to generate a possibly large input for ms.c
LIM=50
echo $LIM
echo 
for ((i=1; i<=LIM; i++)); do echo $(($RANDOM % 100)); done

