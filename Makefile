main : main.cpp win.cpp employee.cpp 
	g++ main.cpp win.cpp employee.cpp -o main -I./inc -g -lncurses -lmenu -lform

clean: 
	rm main
