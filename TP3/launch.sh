#mpirun -n "$1" -hostfile machine_file ./"$2"
mpirun --mca btl_tcp_if_exclude docker0  -n "$1" -hostfile machine_file ./"$2"
