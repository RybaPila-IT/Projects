CC = gcc
CFlags = -Wall -Wextra -O2 #-D PLAYER
err_src = src/err.c src/err.h
fl_mn_src = src/file_manager.c src/file_manager.h
bo_sv_src = src/board_saver.h src/board_saver.c
run_handle_src = src/run_handle.h src/run_handle.c
graph_pen_src = src/graphic_penguin.h src/graphic_penguin.c
ai_move_src = src/ai_move.h src/ai_move.c



all: to_execute

to_execute: main.o file_manager.o err.o board_saver.o run_handle.o graphic_penguin.o ai_move.o
	$(CC) $^ -o PROJECT_PENGUIN -lncurses
	mv -f *.o bin
	mv PROJECT_PENGUIN bin

main.o: $(err_src) $(fl_mn_src) src/main.c
	$(CC) -c $(CFlags) src/main.c

file_manager.o: $(fl_mn_src)
	$(CC) -c $(CFlags) src/file_manager.c

board_saver.o: $(bo_sv_src)
	$(CC) -c $(CFlags) src/board_saver.c

err.o: $(err_src)
	$(CC) -c $(CFlags) src/err.c

run_handle.o: $(run_handle_src)
	$(CC) -c $(CFlags) src/run_handle.c

graphic_penguin.o: $(graph_pen_src)
	$(CC) -c $(CFlags) src/graphic_penguin.c -lncurses

ai_move.o: $(ai_move_src)
	$(CC) -c $(CFlags) src/ai_move.c

clean:
	rm -f bin/*.o bin/PROJECT_PENGUIN

run: bin/PROJECT_PENGUIN
	(cd bin; ./PROJECT_PENGUIN phase=placement penguins=3 Pliki/3.txt Pliki/out.txt)
