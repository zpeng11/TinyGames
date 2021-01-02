work_path=$(dirname $0)
cd ${work_path}
work_path=$(pwd)
cd ${work_path}
g++ tetromino.cpp map.cpp main.cpp -g -pthread -o main && ./main