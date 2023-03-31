FILES="./actual_data/*"
# utils=(0.8 0.82 0.84 0.86 0.88 0.9 0.92 0.94 0.96 0.98 1)
# dists=(2 5 10 20 50 100 200)
for f in $FILES
do
  ./random "$f" >> outpf.txt
  ./nearest "$f" >> outpf.txt
  ./baruah "$f" >> outpf.txt
  ./heuristic "$f" 10 0.8 >> outpf.txt
  ./heuristic "$f" 20 0.8 >> outpf.txt
  ./heuristic "$f" 10 0.9 >> outpf.txt
  ./heuristic "$f" 20 0.9 >> outpf.txt
  echo "hello" >> outpf.txt
  echo "processed $f at all algos"
done