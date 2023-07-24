#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define QUEUE_SZ 1000000
#define ARRLEN(x) (int)(sizeof(x) / sizeof(__typeof__(*x)))

typedef struct
{
    char id;
    int pos_x;
    int pos_y;
    int type;
    int length;
    int exited;
} car;

typedef struct
{
    int id;
    car cars[20];
    int nr_car;
    int parent_id;
} park;

typedef struct
{
    park parks[QUEUE_SZ];
    int front;
    int rear;
} ParkQueue;

enum
{
    vertical,
    horizontal
};

#define TRUE 1
#define FALSE 0

#define TABLE_SZ 100000

char map[20][20];
park park_init;
int x_dim;
int y_dim;
int show = 0;

static ParkQueue queue;
static park park_processed[TABLE_SZ];

void parse_map(char *filename);
void parse_park();
void validate_map();
void queue_init(ParkQueue *q);
void queue_push(ParkQueue *q, park item);
int queue_empty(ParkQueue *q);
park queue_pop(ParkQueue *q);
park queue_peek(ParkQueue *q);
park queue_popback(ParkQueue *q);
void pretty_print(char thismap[][20]);
void draw_map(char thismap[][20], park p);
int compare_park(park p1, park p2);
int compare_car(car c1, car c2);
park array_find_byid(park *array, int size, int id);
int is_duplicate(park *array, int size, park p);
int is_park_empty(park p);

// util

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        if (strncmp(argv[1], "-show", 5) == 0)
        {
            show = 1;
            parse_map(argv[2]);
        }
    }
    else
    {
        parse_map(argv[1]);
    }

    parse_park();
    validate_map();

    int processed_count = 0;

    queue_init(&queue);
    queue_push(&queue, park_init);

    park last_park;
    park_init.id = 0;
    while (queue_empty(&queue) == FALSE)
    {
        // note that in C , function that returns a struct , returns by value
        // the following p is a copy of the queue peek
        char this_map[20][20];
        park p = queue_peek(&queue);
        draw_map(this_map, p);
        //int duplicate = FALSE;

        if (is_duplicate(park_processed, processed_count, p))
        {
            queue_pop(&queue);
        }
        else
        {
            p.id = processed_count;
            // pretty_print(this_map);
            // printf("id:%d parent:%d\n", p.id, p.parent_id);

            if (is_park_empty(p))
            {
                last_park = p;
                break;
            }
            // draw map
            // try all possible moves , and push to queue
            for (int i = 0; i < 20; i++)
            {
                car *c = &p.cars[i];
                if (c->id != 0 && c->exited == FALSE)
                {
                    // if the car is horizontal
                    if (c->type == horizontal)
                    {
                        if (this_map[c->pos_y][c->pos_x + c->length] == '.')
                        {
                            int id = p.parent_id;
                            c->pos_x++;
                            c->exited = (c->pos_x == 0 || c->pos_x + c->length == x_dim) ? TRUE : FALSE;
                            p.parent_id = processed_count;
                            queue_push(&queue, p);
                            c->pos_x--;
                            c->exited = FALSE;
                            p.parent_id = id;
                        }
                        if (this_map[c->pos_y][c->pos_x - 1] == '.')
                        {
                            int id = p.parent_id;
                            c->pos_x--;
                            c->exited = (c->pos_x == 0 || c->pos_x == x_dim) ? TRUE : FALSE;
                            p.parent_id = processed_count;
                            queue_push(&queue, p);
                            c->pos_x++;
                            c->exited = FALSE;
                            p.parent_id = id;
                        }
                    }
                    else if (c->type == vertical)
                    {
                        if (this_map[c->pos_y + c->length][c->pos_x] == '.')
                        {
                            int id = p.parent_id;
                            c->pos_y++;
                            c->exited = (c->pos_y == 0 || c->pos_y + c->length == y_dim) ? TRUE : FALSE;
                            p.parent_id = processed_count;
                            queue_push(&queue, p);
                            c->pos_y--;
                            c->exited = FALSE;
                            p.parent_id = id;
                        }
                        if (this_map[c->pos_y - 1][c->pos_x] == '.')
                        {
                            int id = p.parent_id;
                            c->pos_y--;
                            c->exited = (c->pos_y == 0 || c->pos_y == y_dim) ? TRUE : FALSE;
                            p.parent_id = processed_count;
                            queue_push(&queue, p);
                            c->pos_y++;
                            c->exited = FALSE;
                            p.parent_id = id;
                        }
                    }
                }
            }
            queue_pop(&queue);
            memcpy(park_processed + processed_count, &p, sizeof(park));
            processed_count++;
            last_park = p;
        }
    }

    int move = 0;
    park curr = last_park;
    if(is_park_empty(last_park) == FALSE)
    {
        printf("No Solution?\n");
        return 0;
    }
    queue_init(&queue);
    queue_push(&queue, last_park);
    while (curr.parent_id != 0)
    {
        //int id = curr.parent_id;
        park parent = park_processed[curr.parent_id];
        queue_push(&queue, parent);
        curr = parent;
    }
    queue_push(&queue, park_processed[curr.parent_id]);

    char res[20][20];

    while (queue_empty(&queue) == FALSE)
    {
        draw_map(res, queue_popback(&queue));
        move++;
        if (show)
        {
            pretty_print(res);
            printf("\n");
        }
    }
    if (move < 30000)
        printf("%d moves\n", move - 1);
    else
        printf("no solution\n");

    return 0;
}

void pretty_print(char thismap[][20])
{
    for (int y = 0; y < y_dim; y++)
        for (int x = 0; x < x_dim + 1; x++)
        {
            printf("%c", thismap[y][x]);
        }
}

int compare_car(car c1, car c2)
{
    int result1 = c1.id == c2.id ? TRUE : FALSE;
    int result2 = c1.length == c2.length ? TRUE : FALSE;
    int result3 = c1.pos_x == c2.pos_x ? TRUE : FALSE;
    int result4 = c1.pos_y == c2.pos_y ? TRUE : FALSE;
    int result5 = c1.type == c2.type ? TRUE : FALSE;
    int result6 = c1.exited == c2.exited ? TRUE : FALSE;
    return result1 && result2 && result3 && result4 && result5 && result6;
}

int compare_park(park p1, park p2)
{
    for (int i = 0; i < 20; i++)
    {
        if (compare_car(p1.cars[i], p2.cars[i]) == FALSE)
            return FALSE;
    }

    return TRUE;
}

park array_find_byid(park *array, int size, int id)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i].id == id)
            return array[i];
    }
}

void draw_map(char thismap[][20], park p)
{
    // clear the map , reserve boundries
    memcpy(thismap, map, 20 * 20);
    for (int y = 1; y < y_dim - 1; y++)
        for (int x = 1; x < x_dim - 1; x++)
            thismap[y][x] = '.';

    for (int i = 0; i < 20; i++)
    {
        car car = p.cars[i];
        if (car.id != 0 && car.exited == FALSE)
        {
            if (car.type == horizontal)
            {
                for (int x = 0; x < car.length; x++)
                {
                    thismap[car.pos_y][car.pos_x + x] = car.id;
                }
            }
            else if (car.type == vertical)
            {
                for (int y = 0; y < car.length; y++)
                {
                    thismap[car.pos_y + y][car.pos_x] = car.id;
                }
            }
        }
    }
}

/* parser */


void validate_map()
{
    // check extra char
    for (int y = 0; y < y_dim; y++)
    {
        if (map[y][x_dim] != '\n')
        {
            printf("extra char on line %d\n", y);
            exit(EXIT_FAILURE);
        }
    }

    if (park_init.nr_car == 0)
    {
        printf("no car in parking lot\n");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    for (int x = 0; x < x_dim; x++)
    {
        if (map[0][x] == '.')
            count++;
    }
    for (int x = 0; x < x_dim; x++)
    {
        if (map[y_dim][x] == '.')
            count++;
    }
    for (int y = 0; y < y_dim; y++)
    {
        if (map[y][0] == '.')
            count++;
    }
    for (int y = 0; y < y_dim; y++)
    {
        if (map[y][x_dim - 1] == '.')
            count++;
    }

    if (count == 0)
    {
        printf("No Solution?\n");
        exit(EXIT_FAILURE);
    }
}

int array_contain(char *array, char c)
{
    for (int i = 0; i < ARRLEN(array); i++)
        if (array[i] == c)
            return 1;

    return 0;
}

void parse_park()
{
    char alphas[20];
    int count = 0;
    int length;

    memset(alphas, 0, 20);
    park_init.nr_car = 0;
    for (int i = 0; i < 20; i++)
    {
        park_init.cars->id = 0;
        park_init.cars->exited = 0;
    }

    for (int y = 1; y < y_dim - 1; y++)
    {
        for (int x = 1; x < x_dim - 1; x++)
        {
            int local_x = x;
            int local_y = y;
            char c = map[local_y][local_x];
            if (isalpha(c))
            {
                if (array_contain(alphas, map[local_y][local_x]))
                    continue;

                // check horizontal
                if ((local_x < x_dim - 1) && (c == map[local_y][local_x + 1]))
                {
                    char alpha = map[y][local_x];
                    length = 2;
                    local_x += 2;
                    while (local_x < x_dim)
                        length += alpha == map[local_y][local_x++] ? 1 : 0;

                    car *car = &park_init.cars[count];
                    car->length = length;
                    car->pos_x = x;
                    car->pos_y = y;
                    car->type = horizontal;
                    car->id = map[y][x];
                    alphas[count] = map[y][x];
                    park_init.nr_car++;
                    count++;
                }

                // check vertical
                else if ((local_y < y_dim - 1) && (c == map[local_y + 1][local_x]))
                {
                    char alpha = map[local_y][local_x];
                    length = 2;
                    local_y += 2;
                    while (local_y < y_dim)
                        length += alpha == map[local_y++][local_x] ? 1 : 0;

                    car *car = &park_init.cars[count];
                    car->length = length;
                    car->pos_x = x;
                    car->pos_y = y;
                    car->type = vertical;
                    car->id = map[y][x];
                    alphas[count] = map[y][x];
                    park_init.nr_car++;
                    count++;
                }
            }
        }
    }
}

void parse_map(char *filename)
{
    FILE *f = fopen(filename, "r");
    char desc[10];
    char argx[10];
    char argy[10];

    memset(desc, 0, 10);
    memset(argx, 0, 10);
    memset(argy, 0, 10);
    // memset(map, 0, 20);
    fgets(desc, 10, f);
    char *p = desc;
    while (*p != 'x')
        p++;
    memcpy(argy, desc, p - desc);
    p++;

    int i = 0;
    while (*p != '\n')
    {
        argx[i] = *p++;
        i++;
    }

    x_dim = atoi(argx);
    y_dim = atoi(argy);

    i = 0;
    while (fgets(map[i++], 15, f))
        ;
}

/*  queue  */
void queue_init(ParkQueue *q)
{
    q->front = 0;
    q->rear = 0;
}

park queue_peek(ParkQueue *q)
{
    park result;
    memcpy(&result, &q->parks[q->front], sizeof(park));
    return result;
}

void queue_push(ParkQueue *q, park item)
{
    park *p = &q->parks[q->rear];
    memcpy(p, &item, sizeof(park));
    q->rear = (q->rear + 1) % QUEUE_SZ;
}

park queue_pop(ParkQueue *q)
{
    park val = q->parks[q->front];
    q->front++;
    return val;
}

park queue_popback(ParkQueue *q)
{
    park val = q->parks[q->rear - 1];
    q->rear = (q->rear - 1) < 0 ? (QUEUE_SZ - 1) : (q->rear - 1);
    return val;
}

int queue_empty(ParkQueue *q)
{
    return q->front == q->rear;
}

/*  carpark   */

int is_duplicate(park *array, int size, park p)
{
    for (int i = 0; i < size; i++)
    {
        if (compare_park(p, array[i]) == TRUE)
        {
            return TRUE;
        }
    }

    return FALSE;
}

int is_park_empty(park p)
{
    for (int i = 0; i < 20; i++)
    {
        if (p.cars[i].id != 0 && p.cars[i].exited == FALSE)
            return FALSE;
    }

    return TRUE;
}



