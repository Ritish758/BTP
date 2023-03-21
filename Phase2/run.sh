FILES="./test_files/*"
utils=(0.8 0.82 0.84 0.86 0.88 0.9 0.92 0.94 0.96 0.98 1)
# dists=(2 5 10 20 50 100 200)
for f in $FILES
do
  for i in "${utils[@]}"
    do
        ./a.out "$f" 10000 "$i" >> outpf.txt
    done  
  echo "hello" >> outpf.txt
  echo "processed $f at all utils"
done