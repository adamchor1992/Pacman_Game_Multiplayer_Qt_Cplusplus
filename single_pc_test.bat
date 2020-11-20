cd Server/executable
start Pacman_Server.exe

timeout 1

cd ../../Client/executable

start Pacman_Client.exe client1.txt
start Pacman_Client.exe client2.txt