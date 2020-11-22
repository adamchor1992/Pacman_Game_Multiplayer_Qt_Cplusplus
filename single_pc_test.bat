cd Server/executable
start Pacman_Server.exe

timeout 1

cd ../../Client/executable

start Pacman_Client.exe single_pc_test_client1.txt
start Pacman_Client.exe single_pc_test_client2.txt