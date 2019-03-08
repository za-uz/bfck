#include <stdlib.h>
//#include <stddef.h>
#include <stdio.h>

typedef unsigned char uchar;

struct Data {
	uchar *cur;
	ptrdiff_t curi;
	uchar *pos;
	size_t spos;
	uchar *neg;
	size_t sneg;
};

struct Stack {
	const char **bot;
	size_t size;
};

void
push(struct Stack *stack, const char *item)
{
	stack->bot = realloc(stack->bot, ++stack->size);
	stack->bot[stack->size - 1] = item;
}

const char *
pop(struct Stack *stack)
{
	const char *item = stack->bot[stack->size - 1];
	stack->bot = realloc(stack->bot, --stack->size);
	return item;
}

void
init_data(struct Data *data) {
	data->curi = 0;
	data->pos = malloc(1);
	data->spos = 1;
	data->neg = malloc(0);
	data->sneg = 0;
	data->cur = data->pos;
}

void
dinc(struct Data *data) {
	data->curi++;
	if (data->curi == 0) {
		data->cur = data->pos;
	} else if (data->curi >= (ptrdiff_t) data->spos) {
		data->pos = realloc(data->pos, ++data->spos);
		data->cur = &data->pos[data->curi];
	} else {
		data->cur++;
	}
}

void
ddec(struct Data *data)
{
	data->curi--;
	if (data->curi == -1) {
		data->cur = data->neg;
	} else if (-data->curi >= (ptrdiff_t) data->sneg) {
		data->neg = realloc(data->neg, ++data->sneg);
		data->cur = &data->neg[-data->curi];
	} else {
		data->cur--;
	}
}

/*
 * runs the code
 * reads input from in
 * writes output to out
 * 
 * code has to be correct (don't close a loop without opening it,
 * TERRIBLE things will happen)
 * code is null terminated
 * 
 * returns pointer to the null terminator of code on success
 * writes memory to all sorts of terrible places on error
 */

const char *
run(const char *code, FILE *in, FILE *out)
{
	/* stack for nested loops */
	struct Stack loop_stack = {NULL, 0};
	/* data, see get(uchar *) and put(uchar *, uchar) */
	struct Data data;
	init_data(&data);

	for (;;) {
		switch (*code) {
		case '>':
			dinc(&data);
			break;
		case '<':
			ddec(&data);
			break;
		case '+':
			(*data.cur)++;
			break;
		case '-':
			(*data.cur)--;
			break;
		case '.':
			putc((int) *data.cur, out);
			break;
		case ',':
			*data.cur = (char) getc(in);
			break;
		case '[':
			// TODO jump to matching ] if 0
			push(&loop_stack, code);
			break;
		case ']':
			if (*data.cur == 0)
				break;
			code = pop(&loop_stack);
			continue;
		case '\0':
			return code;
		}
		code++;
	}
}

