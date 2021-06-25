python3 create_test.py factorial.s
bash cpu-user.sh
cd my_tests/circ_files
python binary_to_hex.py ./output/CPU-factorial.out > ../../outout.out
cd ..
cd ..