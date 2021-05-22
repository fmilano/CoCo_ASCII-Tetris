#ifndef TETRIS_H
# define TETRIS_H

struct tetris;

void tetris_cleanup_io();

void tetris_signal_quit(int);

void tetris_set_ioconfig();

void tetris_init();

void tetris_clean();

void tetris_print();

void tetris_run();

void tetris_new_block();

void tetris_new_block();

void tetris_print_block();

void tetris_rotate();

void tetris_gravity();

void tetris_fall(int l);

void tetris_check_lines();

int tetris_level();



#endif //TETRIS_H
