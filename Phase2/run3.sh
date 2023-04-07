FILES="./actual_data4/*"
# utils=(0.8 0.82 0.84 0.86 0.88 0.9 0.92 0.94 0.96 0.98 1)
# dists=(2 5 10 20 50 100 200)
for f in $FILES
do
  ./random "$f" >> outpf2.txt
  ./nearest "$f" >> outpf2.txt
  ./baruah "$f" >> outpf2.txt
  ./heuristic "$f" 10 0.8 >> outpf2.txt
  ./heuristic "$f" 20 0.8 >> outpf2.txt
  ./heuristic "$f" 10 0.9 >> outpf2.txt
  ./heuristic "$f" 20 0.9 >> outpf2.txt
  echo "hello" >> outpf2.txt
  echo "processed $f at all algos"
done