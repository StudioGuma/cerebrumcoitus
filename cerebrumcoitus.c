/*
cerebrumcoitus — Brainfuck interpreter
Copyright (C) 2024 StudioGuma

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <signal.h>

#define TAPE_SIZE 0x10000

#define FREE_AND_EXIT(n) \
free(tape); \
tape = NULL; \
exit(n);

volatile sig_atomic_t interrupt = 0;

void sig_handler(int sig)
{	
	if (sig == SIGINT)
		interrupt = 1;
	putchar('\n');
}

off_t match_brackets(off_t offset, int fd, const char TARGET)
{
	char buf[1];
	int8_t pbyte_read;
	uint32_t num_pairs = 0;

	while ((pbyte_read = pread(fd, buf, 1, offset)) > 0) {
		switch (TARGET) {
		case ']':
			switch (*buf) {
			case '[':
				num_pairs++;
				break;

			case ']':
				if (num_pairs > 0)
					num_pairs--;
				else
					return offset;
			}

			offset++;
			break;

		case '[':
			switch (*buf) {
			case '[':
				if (num_pairs > 0)
					num_pairs--;
				else
					return offset;
				break;

			case ']':
				num_pairs++;
			}

			offset--;
		}
	}

	return (off_t)pbyte_read;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: cerebrumsex <file>\n");
		exit(EXIT_FAILURE);
	}

	struct sigaction action = {0};
	action.sa_handler = sig_handler;
	if (sigaction(SIGINT, &action, NULL) < 0) {
		perror("sigaction()");
		exit(EXIT_FAILURE);
	}

	int bf_file = open(argv[1], O_RDONLY);
	if (bf_file < 0) {
		perror("open()");
		exit(EXIT_FAILURE);
	}

	uint8_t *tape = calloc(TAPE_SIZE, 1);
	if (tape == NULL) {
		perror("calloc()");
		close(bf_file);
		exit(EXIT_FAILURE);
	}
	uint8_t *data_ptr = tape;
	char inst_ptr[1];
	int8_t byte_read;
	off_t offset = 0;

	while ((byte_read = read(bf_file, inst_ptr, 1)) > 0) {
		switch (*inst_ptr) {
		case '>':
			if (data_ptr == tape + (TAPE_SIZE - 1)) {
				fprintf(stderr, "Error: data pointer incremented out of bounds\n");
				close(bf_file);
				FREE_AND_EXIT(EXIT_FAILURE)
			}

			data_ptr++;
			break;

		case '<':
			if (data_ptr == tape) {
				fprintf(stderr, "Error: data pointer decremented out of bounds\n");
				close(bf_file);
				FREE_AND_EXIT(EXIT_FAILURE)
			}

			data_ptr--;
			break;

		case '+':
			(*data_ptr)++;
			break;

		case '-':
			(*data_ptr)--;
			break;

		case '.':
			if (putchar(*data_ptr) == EOF) {
				perror("putchar()");
				close(bf_file);
				FREE_AND_EXIT(EXIT_FAILURE)
			}
			break;

		case ',':
			*data_ptr = (uint8_t)getchar();
			break;

		case '[':
			if ((*data_ptr) == 0) {
				offset = match_brackets(++offset, bf_file, ']');
				if (offset == 0) {
					fprintf(stderr, "Error: unmatched '['\n");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}
				if (offset < 0) {
					perror("pread()");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}

				if (lseek(bf_file, ++offset, SEEK_SET) < 0) {
					perror("lseek()");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}
			}
			break;

		case ']':
			if ((*data_ptr) != 0) {
				offset = match_brackets(--offset, bf_file, '[');
				if (offset == 0) {
					fprintf(stderr, "Error: unmatched ']'\n");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}
				if (offset < 0) {
					perror("pread()");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}

				if (lseek(bf_file, ++offset, SEEK_SET) < 0) {
					perror("lseek()");
					close(bf_file);
					FREE_AND_EXIT(EXIT_FAILURE)
				}
			}
		}

		offset = lseek(bf_file, 0, SEEK_CUR);
		if (offset < 0) {
			perror("lseek()");
			close(bf_file);
			FREE_AND_EXIT(EXIT_FAILURE)
		}

		if (interrupt) {
			close(bf_file);
			FREE_AND_EXIT(EXIT_SUCCESS)
		}
	}

	if (byte_read < 0) {
		perror("read()");
		close(bf_file);
		FREE_AND_EXIT(EXIT_FAILURE)
	}

	if (close(bf_file) < 0) {
		perror("close()");
		FREE_AND_EXIT(EXIT_FAILURE)
	}
	FREE_AND_EXIT(EXIT_SUCCESS)
}
