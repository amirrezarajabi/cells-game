#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

int map[200][200];
int cell_map[200][200];
int energy_map[200][200];
int color_map_arr[600][601];
char print_map_arr[600][601];
int start_variable , size_map;
char *mapbi= "map.bin";
struct cell {
    int energy;
    int x;
    int y;
    char name[10];
    struct cell *next;
};

HANDLE hConsole;
void color_initialize(void)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}
void set_color(int f, int b)
{
    SetConsoleTextAttribute(hConsole, b*16+f);
}

void free_map()
{
    for(int i = 0; i < 600; i++)
    {
        for(int j = 0; j < 601; j++)
        {
            print_map_arr[i][j] = ' ';
        }
    }
}

int load_energy_map(char *file)
{
    int i , j = 1 , h , x , y;
    FILE *fp;
    fp = fopen(file , "r");
    fscanf(fp , "%d" , &i);
    while (j < 401)
    {
        fscanf(fp , "%d" , &h);
        x = j - 1;
        x = x / 200;
        y = j - 1;
        y = y % 200;
        energy_map[x][y] = h;
        j = j + 1;
    }
    fclose(fp);
    return i;
}

void buid_energy_map(char *file , int type_game)
{
    int i = 0 , j = 0;
    FILE *fp;
    fp = fopen(file , "w");
    fprintf(fp , "%d\n" , type_game);
    while (i < 200)
    {
        j = 0;
        while (j < 200)
        {
            fprintf(fp , "%d\n" , energy_map[i][j]);
            j = j + 1;
        }
        i = i + 1;
    }
    fclose(fp);
}

void build_cells_map_of_map()
{
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            if (map[i][j] == 3)
                cell_map[i][j] = -1;
            if (map[i][j]== 1)
                energy_map[i][j] = 100;
        }
    }
}

struct cell * newcell(int energy , int x , int y , char *name)
{
    struct cell * new_cell = (struct cell *) malloc(sizeof(struct cell));
    new_cell->energy = energy;
    new_cell->x = x;
    new_cell->y = y;
    strcpy(new_cell->name , name);
    new_cell->next = NULL;
    return new_cell;
}

void pushBack(struct cell ** head , int energy , int x , int y , char *name)
{
    if(*head == NULL)
    {
        *head = newcell(energy , x , y , name);
        return;
    }
    struct cell * curr = *head;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = newcell(energy , x , y , name);
}

void turn_menu()
{
    printf("[1]Move\n[2]Split a cell\n[3]Boost energy\n[4]Save\n[5]Exit\n");
}

void deletecell(struct cell ** head , char name[])
{
    if(*head == NULL){
        return ;
    }
    if(strcmp((*head)->name , name)== 0){
        struct cell* temp=*head;
        *head=(*head)->next;
        free(temp);
        return;
    }
    struct cell* current=*head;
    struct cell* last = NULL;
    while(current!=NULL)
    {
        if (strcmp(current->name , name) == 0)
        {
            last->next = current->next;
            free(current);
            return 1;
        }
        last = current;
        current = current->next;
    }
    return ;
}

char stoc (char *s)
{
    return s;
}

int sum_energy(struct cell * cell1)
{
    struct cell * cell2 = cell1;
    int tmp = 0;
    while (cell2 != NULL)
    {
        tmp = tmp + cell2->energy , cell2 = cell2->next;
    }
    return tmp;
}

void start_menu ( )
{
    printf("[1]Load\n[2]New single player game\n[3]New Multiplayer game\n[4]Exit\n");
    scanf("%d" , &start_variable);
}

int map_size (char *file)
{
    FILE *fp;
    int *i;
    fp = fopen(file, "rb");
    if(fp == NULL)
    {
        printf("Cannot read from file\n");
        return -1;
    }
    fread(&i , sizeof(int) , 1 , fp);
    fclose(fp);
    return i;
}

int type_to_color(int n)
{
    if(n == 1)
        return 14;
    if(n == 2)
        return 10;
    if(n == 3)
        return 12;
    if(n == 4)
        return 15;
}

void build_map (char *file)
{
    FILE *fp;
    char *s ;
    int *i , n = map_size(file);
    fp = fopen(file, "rb");
    fread(&i , sizeof(int) , 1 , fp);
    if(fp == NULL)
    {
        printf("Cannot read from file\n");
        return ;
    }
    for (int j = 1;j < n * n  + 1; j++)
    {
        fread(&s , sizeof(char) , 1 , fp);
        if (j % n == 0)
            map[j - (int)(j / n) * n - 1 + n][n - 1 - (j - 1) / n] = stoc(s) -'0';
        if (j % n != 0)
            map[j - (int)(j / n) * n - 1][n - 1 - (j - 1) / n] = stoc(s) -'0';
    }
    fclose(fp);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i % 2 == 0)
            {
                color_map_arr[3 * i][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i][3 * j + 2] = type_to_color(map[i][j]);
                color_map_arr[3 * i][3 * j + 3] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j + 2] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j + 3] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j + 2] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j + 3]= type_to_color(map[i][j]);
            }
            if(i % 2 == 1)
            {
                color_map_arr[3 * i][3 * j] = type_to_color(map[i][j]);
                color_map_arr[3 * i][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i][3 * j + 2] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 1][3 * j + 2] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j + 1] = type_to_color(map[i][j]);
                color_map_arr[3 * i + 2][3 * j + 2]= type_to_color(map[i][j]);
            }
        }
    }
}
void update_map()
{
    free_map();
    for(int i = 0; i < 200; i++)
    {
        for(int j = 0; j < 200; j++)
        {
            if(i % 2 == 0)
            {
                if(cell_map[i][j] == 1)
                    print_map_arr[3 * i + 1][3 * j + 2] = 'x';
                if(cell_map[i][j] == 2)
                    print_map_arr[3 * i + 1][3 * j + 2] = 'y';
            }
            if(i % 2 == 1)
            {
                if(cell_map[i][j] == 1)
                    print_map_arr[3 * i + 1][3 * j + 1] = 'x';
                if(cell_map[i][j] == 2)
                    print_map_arr[3 * i + 1][3 * j + 1] = 'y';
            }
        }
    }
}

void printcell(struct cell * head)
{
    struct cell * curr = head;
    int i = 1;
    printf("sum of your energy: %d\n" , sum_energy(head));
    printf("Please choose one of your cells:\n");
    while (curr!=NULL)
    {
        printf("[%d]",i);
        printf(" %s (%d , %d) E: %d", curr->name , curr->x , curr->y , curr->energy);
        curr = curr->next;
        if (curr != NULL)
            printf("\n");
        i = i + 1;
    }
    printf("\n");
}

int num_of_cell(struct cell * cells)
{
    int i = 0;
    struct cell * cellss = cells;
    while(cellss != NULL)
    {
        cellss = cellss->next;
        i = i + 1;
    }
    return i;
}

void savecell(struct cell * head , char * file)
{
    struct cell * curr = head;
    FILE *fp;
    fp = fopen(file,"w");
    if (fp == NULL)
    {
        printf(":(");
        return ;
    }
    fprintf(fp,"%d\n",num_of_cell(head));
    while (curr!=NULL)
    {
        fprintf(fp ,"%s\n",curr->name);
        fprintf(fp ,"%d\n",curr->energy);
        fprintf(fp ,"%d\n",curr->x);
        fprintf(fp ,"%d\n",curr->y);
        curr = curr->next;
    }
    fclose(fp);
}

int check_be (int x , int y)
{
    if (cell_map[x][y] == 0 && y < size_map && y > -1 && x < size_map && x > -1)
        return 1;
    return 0;
}

int x_cell(struct cell *head , char *name)
{
    struct cell *c = head;
    while (strcmp(c->name , name) != 0)
        c = c->next;
    return c->x;
}

int y_cell(struct cell *head , char *name)
{
    struct cell *c = head;
    while (strcmp(c->name , name) != 0)
        c = c->next;
    return c->y;
}

int e_cell(struct cell *head , char *name)
{
    struct cell *c = head;
    while (strcmp(c->name , name) != 0)
        c = c->next;
    return c->energy;
}

char * n_cell(struct cell *head , int n)
{
    struct cell *c = head;
    for (int i = 1; i < n ; i++)
        c = c->next;
    return c->name;
}

void move_menu ()
{
        printf("[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
}

int x_change(int x , int direction)
{
    if (direction == 1)
    {
        return x;
    }
    if (direction == 2)
    {
        return x;
    }
    if (direction == 3)
    {
        return x + 1;
    }
    if (direction == 4)
    {
        return x - 1;
    }
    if (direction == 5)
    {
        return x + 1;
    }
    if (direction == 6)
    {
        return x - 1;
    }
}

int y_change(int y , int direction , int x)
{
    if (direction == 1)
    {
        return y + 1;
    }
    if (direction == 2)
    {
        return y - 1;
    }
    if (direction == 3 && x % 2 == 1)
    {
        return y;
    }
    if (direction == 4 && x % 2 == 1)
    {
        return y;
    }
    if (direction == 5 && x % 2 == 1)
    {
        return y - 1;
    }
    if (direction == 6 && x % 2 == 1)
    {
        return y - 1;
    }
    if (direction == 3 && x % 2 == 0)
    {
        return y + 1;
    }
    if (direction == 4 && x % 2 == 0)
    {
        return y + 1;
    }
    if (direction == 5 && x % 2 == 0)
    {
        return y;
    }
    if (direction == 6 && x % 2 == 0)
    {
        return y;
    }
}

int check_be_mitosis(int x , int y , int energy)
{
    if (map[x][y] == 2)
    {
        if (energy > 79)
        {
            if (x % 2 == 0)
            {
                if (check_be(x - 1 , y) + check_be(x - 1 , y + 1) + check_be(x , y + 1) + check_be(x + 1 , y + 1) + check_be(x + 1 , y) + check_be(x , y - 1) > 0)
                {
                    return 1;
                }
            }
            if (x % 2 == 1)
            {
                if (check_be(x - 1 , y - 1) + check_be(x - 1 , y) + check_be(x , y + 1) + check_be(x + 1 , y) + check_be(x + 1 , y - 1) + check_be(x , y - 1) > 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int check_be_boost_energy(int x , int y)
{
    if (energy_map[x][y] > 10){
        return 1;
    }
    return 0;
}

int main ()
{
    struct cell * cell1 = NULL;
    struct cell * cell2 = NULL;
    srand(time(NULL));
    color_initialize();
    start_menu();
    int which_cell , which_cell2 , flag = 0 , turn;
    char *file1 = "cell1.txt" , *game = "game.txt" , *file2 = "cell2.txt" , *mapbin = "map.bin" , *which_name , *which_name2;
    load_energy_map(game);
    if(start_variable == 4)
        return 0;
    if(start_variable == 1)
    {
        size_map = map_size(mapbin);
        build_map(mapbin);
        build_cells_map_of_map();
        if (load_energy_map(game) == 1)
        {
            load_energy_map(game);
            FILE * fp;
            fp = fopen(file1 , "r");
            char name[10] ;
            int x , y , energy , i;
            fscanf(fp,"%d" ,&i);
            while(i > 0)
            {
                fscanf(fp,"%s" ,&name);
                fscanf(fp,"%d" ,&energy);
                fscanf(fp,"%d" ,&x);
                fscanf(fp,"%d" ,&y);
                pushBack(&cell1,energy,x,y,name);
                cell_map[x][y] = 1;
                i = i - 1;
            }
            fclose(fp);
            while(flag == 0)
            {
                update_map();
                for(int jj = 3 * size_map; jj > -1; jj--)
                {
                    for(int ii = 0; ii < 3 * size_map; ii++)
                    {
                        set_color(0 , color_map_arr[ii][jj]);
                        printf("%c" , print_map_arr[ii][jj]);
                    }
                    printf("\n");
                }
                set_color(15 , 0);
                int direct ;
                printcell(cell1);
                scanf("%d" , &which_cell);
                which_name = n_cell(cell1 , which_cell);
                turn_menu();
                scanf("%d" , &turn);
                if(turn == 5)
                    return 0;
                if(turn == 4)
                {
                    buid_energy_map(game , 1);
                    savecell(cell1 , file1);
                    return 0;
                }
                if(turn == 1)
                {
                    move_menu();
                    scanf("%d" , &direct);
                    if(check_be(x_change(x_cell(cell1 , which_name) , direct) , y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name))) == 1)
                    {
                        cell_map[x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = 0 ;
                        int xxx = x_change(x_cell(cell1 , which_name) , direct) , yyy = y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name)) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee , xxx , yyy , which_name);
                        cell_map[xxx][yyy] = 1;
                    }
                }
                if(turn == 3)
                {
                    if(check_be_boost_energy(x_cell(cell1 , which_name) , y_cell(cell1 , which_name)) == 1 && e_cell(cell1,which_name) < 86)
                    {
                        energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] - 15;
                        int xxx = x_cell(cell1 , which_name) , yyy = y_cell(cell1 , which_name) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee + 15 , xxx , yyy , which_name);
                    }
                }
                if (turn == 2)
                {
                    if(check_be_mitosis(x_cell(cell1 , which_name),y_cell(cell1 , which_name) ,e_cell(cell1 , which_name)) == 1)
                    {
                        char name1[10] , name2[10] ;
                        int xxxx = x_cell(cell1 , which_name) ,  yyyy = y_cell(cell1 , which_name) ;
                        printf("choose names of cells:\n");
                        scanf("%s" , &name1);
                        scanf("%s" , &name2);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , 40 , xxxx , yyyy , name1);
                        if (xxxx % 2 == 0)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy + 1, name2);
                                cell_map[xxxx + 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy + 1, name2);
                                cell_map[xxxx - 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy , name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                        if (xxxx % 2 == 1)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy ) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy, name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy - 1 , name2);
                                cell_map[xxxx + 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy - 1, name2);
                                cell_map[xxxx - 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                    }
                }
            }
        }
        if (load_energy_map(game) == 2)
        {
            load_energy_map(game);
            FILE * fp , *fq;
            fp = fopen(file1 , "r");
            fq = fopen(file2 , "r");
            char name[10] ;
            int x , y , energy , i;
            fscanf(fp,"%d" ,&i);
            while(i > 0)
            {
                fscanf(fp,"%s" ,&name);
                fscanf(fp,"%d" ,&energy);
                fscanf(fp,"%d" ,&x);
                fscanf(fp,"%d" ,&y);
                pushBack(&cell1,energy,x,y,name);
                cell_map[x][y] = 1;
                i = i - 1;
            }
            fclose(fp);
            fscanf(fq,"%d" ,&i);
            while(i > 0)
            {
                fscanf(fq,"%s" ,&name);
                fscanf(fq,"%d" ,&energy);
                fscanf(fq,"%d" ,&x);
                fscanf(fq,"%d" ,&y);
                pushBack(&cell2,energy,x,y,name);
                cell_map[x][y] = 2;
                i = i - 1;
            }
            fclose(fq);
            while (flag == 0)
            {
                update_map();
                for(int jj = 3 * size_map; jj > -1; jj--)
                {
                    for(int ii = 0; ii < 3 * size_map; ii++)
                    {
                        set_color(0 , color_map_arr[ii][jj]);
                        printf("%c" , print_map_arr[ii][jj]);
                    }
                    printf("\n");
                }
                set_color(15 , 0);
                int direct ;
                printcell(cell1);
                scanf("%d" , &which_cell);
                which_name = n_cell(cell1 , which_cell);
                turn_menu();
                scanf("%d" , &turn);
                if(turn == 5)
                    return 0;
                if(turn == 4)
                {
                    buid_energy_map(game , 2);
                    savecell(cell1 , file1);
                    savecell(cell2 , file2);
                    return 0;
                }
                if(turn == 1)
                {
                    move_menu();
                    scanf("%d" , &direct);
                    if(check_be(x_change(x_cell(cell1 , which_name) , direct) , y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name))) == 1)
                    {
                        cell_map[x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = 0 ;
                        int xxx = x_change(x_cell(cell1 , which_name) , direct) , yyy = y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name)) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee , xxx , yyy , which_name);
                        cell_map[xxx][yyy] = 1;
                    }
                }
                if(turn == 3)
                {
                    if(check_be_boost_energy(x_cell(cell1 , which_name) , y_cell(cell1 , which_name)) == 1 && e_cell(cell1,which_name) < 86)
                    {
                        energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] - 15;
                        int xxx = x_cell(cell1 , which_name) , yyy = y_cell(cell1 , which_name) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee + 15 , xxx , yyy , which_name);
                    }
                }
                if (turn == 2)
                {
                    if(check_be_mitosis(x_cell(cell1 , which_name),y_cell(cell1 , which_name) ,e_cell(cell1 , which_name)) == 1)
                    {
                        char name1[10] , name2[10] ;
                        int xxxx = x_cell(cell1 , which_name) ,  yyyy = y_cell(cell1 , which_name) ;
                        printf("choose names of cells:\n");
                        scanf("%s" , &name1);
                        scanf("%s" , &name2);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , 40 , xxxx , yyyy , name1);
                        if (xxxx % 2 == 0)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy + 1, name2);
                                cell_map[xxxx + 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy + 1, name2);
                                cell_map[xxxx - 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy , name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                        if (xxxx % 2 == 1)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy ) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy, name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy - 1 , name2);
                                cell_map[xxxx + 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy - 1, name2);
                                cell_map[xxxx - 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                    }
                }
                update_map();
                for(int jj = 3 * size_map; jj > -1; jj--)
                {
                    for(int ii = 0; ii < 3 * size_map; ii++)
                    {
                        set_color(0 , color_map_arr[ii][jj]);
                        printf("%c" , print_map_arr[ii][jj]);
                    }
                    printf("\n");
                }
                set_color(15 , 0);
                int direct2 ;
                printcell(cell2);
                scanf("%d" , &which_cell2);
                which_name2 = n_cell(cell2 , which_cell2);
                turn_menu();
                scanf("%d" , &turn);
                if(turn == 5)
                    return 0;
                if(turn == 4)
                {
                    buid_energy_map(game , 3);
                    savecell(cell1 , file1);
                    savecell(cell2 , file2);
                    return 0;
                }
                if(turn == 1)
                {
                    move_menu();
                    scanf("%d" , &direct2);
                    if(check_be(x_change(x_cell(cell2 , which_name2) , direct2) , y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2))) == 1)
                    {
                        cell_map[x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = 0 ;
                        int xxx = x_change(x_cell(cell2 , which_name2) , direct2) , yyy = y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2)) , eee = e_cell(cell2 , which_name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , eee , xxx , yyy , which_name2);
                        cell_map[xxx][yyy] = 2;
                    }
                }
                if(turn == 3)
                {
                    if(check_be_boost_energy(x_cell(cell2 , which_name2) , y_cell(cell2 , which_name2)) == 1 && e_cell(cell2,which_name2) < 86)
                    {
                        energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] - 15;
                        int xxx = x_cell(cell2 , which_name2) , yyy = y_cell(cell2 , which_name2) , eee = e_cell(cell2 , which_name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , eee + 15 , xxx , yyy , which_name2);
                    }
                }
                if (turn == 2)
                {
                    if(check_be_mitosis(x_cell(cell2 , which_name2),y_cell(cell2 , which_name2) ,e_cell(cell2 , which_name2)) == 1)
                    {
                        char name1[10] , name2[10] ;
                        int xxxx = x_cell(cell2 , which_name2) ,  yyyy = y_cell(cell2 , which_name2) ;
                        printf("choose names of cells:\n");
                        scanf("%s" , &name1);
                        scanf("%s" , &name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , 40 , xxxx , yyyy , name1);
                        if (xxxx % 2 == 0)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1, yyyy + 1, name2);
                                cell_map[xxxx + 1][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx - 1, yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1 , yyyy + 1, name2);
                                cell_map[xxxx - 1][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1,yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1 , yyyy , name2);
                                cell_map[xxxx + 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx - 1,yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1, yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 2;
                            }
                        }
                        if (xxxx % 2 == 1)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1 ,yyyy ) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1, yyyy, name2);
                                cell_map[xxxx + 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx - 1, yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1 , yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx + 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1 , yyyy - 1 , name2);
                                cell_map[xxxx + 1][yyyy - 1] = 2;
                            }
                            else if (check_be(xxxx - 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1, yyyy - 1, name2);
                                cell_map[xxxx - 1][yyyy - 1] = 2;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 2;
                            }
                        }
                    }
                }
            }
        }
        if (load_energy_map(game) == 3)
        {
            load_energy_map(game);
            FILE *fp , *fq;
            fp = fopen(file1 , "r");
            fq = fopen(file2 , "r");
            char name[10] ;
            int x , y , energy , i;
            fscanf(fp,"%d" ,&i);
            while(i > 0)
            {
                fscanf(fp,"%s" ,&name);
                fscanf(fp,"%d" ,&energy);
                fscanf(fp,"%d" ,&x);
                fscanf(fp,"%d" ,&y);
                pushBack(&cell1,energy,x,y,name);
                cell_map[x][y] = 1;
                i = i - 1;
            }
            fclose(fp);
            fscanf(fq,"%d" ,&i);
            while(i > 0)
            {
                fscanf(fq,"%s" ,&name);
                fscanf(fq,"%d" ,&energy);
                fscanf(fq,"%d" ,&x);
                fscanf(fq,"%d" ,&y);
                pushBack(&cell2,energy,x,y,name);
                cell_map[x][y] = 2;
                i = i - 1;
            }
            fclose(fq);
            while (flag == 0)
            {
                update_map();
                for(int jj = 3 * size_map; jj > -1; jj--)
                {
                    for(int ii = 0; ii < 3 * size_map; ii++)
                    {
                        set_color(0 , color_map_arr[ii][jj]);
                        printf("%c" , print_map_arr[ii][jj]);
                    }
                    printf("\n");
                }
                set_color(15 , 0);
                int direct2 ;
                printcell(cell2);
                scanf("%d" , &which_cell2);
                which_name2 = n_cell(cell2 , which_cell2);
                turn_menu();
                scanf("%d" , &turn);
                if(turn == 5)
                    return 0;
                if(turn == 4)
                {
                    buid_energy_map(game , 3);
                    savecell(cell1 , file1);
                    savecell(cell2 , file2);
                    return 0;
                }
                if(turn == 1)
                {
                    move_menu();
                    scanf("%d" , &direct2);
                    if(check_be(x_change(x_cell(cell2 , which_name2) , direct2) , y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2))) == 1)
                    {
                        cell_map[x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = 0 ;
                        int xxx = x_change(x_cell(cell2 , which_name2) , direct2) , yyy = y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2)) , eee = e_cell(cell2 , which_name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , eee , xxx , yyy , which_name2);
                        cell_map[xxx][yyy] = 2;
                    }
                }
                if(turn == 3)
                {
                    if(check_be_boost_energy(x_cell(cell2 , which_name2) , y_cell(cell2 , which_name2)) == 1 && e_cell(cell2,which_name2) < 86)
                    {
                        energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] - 15;
                        int xxx = x_cell(cell2 , which_name2) , yyy = y_cell(cell2 , which_name2) , eee = e_cell(cell2 , which_name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , eee + 15 , xxx , yyy , which_name2);
                    }
                }
                if (turn == 2)
                {
                    if(check_be_mitosis(x_cell(cell2 , which_name2),y_cell(cell2 , which_name2) ,e_cell(cell2 , which_name2)) == 1)
                    {
                        char name1[10] , name2[10] ;
                        int xxxx = x_cell(cell2 , which_name2) ,  yyyy = y_cell(cell2 , which_name2) ;
                        printf("choose names of cells:\n");
                        scanf("%s" , &name1);
                        scanf("%s" , &name2);
                        deletecell(&cell2 , which_name2);
                        pushBack(&cell2 , 40 , xxxx , yyyy , name1);
                        if (xxxx % 2 == 0)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1, yyyy + 1, name2);
                                cell_map[xxxx + 1][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx - 1, yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1 , yyyy + 1, name2);
                                cell_map[xxxx - 1][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1,yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1 , yyyy , name2);
                                cell_map[xxxx + 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx - 1,yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1, yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 2;
                            }
                        }
                        if (xxxx % 2 == 1)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 2;
                            }
                            else if (check_be(xxxx + 1 ,yyyy ) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1, yyyy, name2);
                                cell_map[xxxx + 1][yyyy ] = 2;
                            }
                            else if (check_be(xxxx - 1, yyyy) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1 , yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 2;
                            }
                            else if (check_be(xxxx + 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx + 1 , yyyy - 1 , name2);
                                cell_map[xxxx + 1][yyyy - 1] = 2;
                            }
                            else if (check_be(xxxx - 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx - 1, yyyy - 1, name2);
                                cell_map[xxxx - 1][yyyy - 1] = 2;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 2;
                            }
                        }
                    }
                }
                update_map();
                for(int jj = 3 * size_map; jj > -1; jj--)
                {
                    for(int ii = 0; ii < 3 * size_map; ii++)
                    {
                        set_color(0 , color_map_arr[ii][jj]);
                        printf("%c" , print_map_arr[ii][jj]);
                    }
                    printf("\n");
                }
                set_color(15 , 0);
                int direct ;
                printcell(cell1);
                scanf("%d" , &which_cell);
                which_name = n_cell(cell1 , which_cell);
                turn_menu();
                scanf("%d" , &turn);
                if(turn == 5)
                    return 0;
                if(turn == 4)
                {
                    buid_energy_map(game , 2);
                    savecell(cell1 , file1);
                    savecell(cell2 , file2);
                    return 0;
                }
                if(turn == 1)
                {
                    move_menu();
                    scanf("%d" , &direct);
                    if(check_be(x_change(x_cell(cell1 , which_name) , direct) , y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name))) == 1)
                    {
                        cell_map[x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = 0 ;
                        int xxx = x_change(x_cell(cell1 , which_name) , direct) , yyy = y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name)) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee , xxx , yyy , which_name);
                        cell_map[xxx][yyy] = 1;
                    }
                }
                if(turn == 3)
                {
                    if(check_be_boost_energy(x_cell(cell1 , which_name) , y_cell(cell1 , which_name)) == 1 && e_cell(cell1,which_name) < 86)
                    {
                        energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] - 15;
                        int xxx = x_cell(cell1 , which_name) , yyy = y_cell(cell1 , which_name) , eee = e_cell(cell1 , which_name);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , eee + 15 , xxx , yyy , which_name);
                    }
                }
                if (turn == 2)
                {
                    if(check_be_mitosis(x_cell(cell1 , which_name),y_cell(cell1 , which_name) ,e_cell(cell1 , which_name)) == 1)
                    {
                        char name1[10] , name2[10] ;
                        int xxxx = x_cell(cell1 , which_name) ,  yyyy = y_cell(cell1 , which_name) ;
                        printf("choose names of cells:\n");
                        scanf("%s" , &name1);
                        scanf("%s" , &name2);
                        deletecell(&cell1 , which_name);
                        pushBack(&cell1 , 40 , xxxx , yyyy , name1);
                        if (xxxx % 2 == 0)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy + 1, name2);
                                cell_map[xxxx + 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy + 1, name2);
                                cell_map[xxxx - 1][yyyy + 1] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy , name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                        if (xxxx % 2 == 1)
                        {
                            if (check_be(xxxx ,yyyy + 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx + 1 ,yyyy ) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1, yyyy, name2);
                                cell_map[xxxx + 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx - 1, yyyy) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1 , yyyy , name2);
                                cell_map[xxxx - 1][yyyy] = 1;
                            }
                            else if (check_be(xxxx + 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx + 1 , yyyy - 1 , name2);
                                cell_map[xxxx + 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx - 1,yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx - 1, yyyy - 1, name2);
                                cell_map[xxxx - 1][yyyy - 1] = 1;
                            }
                            else if (check_be(xxxx , yyyy - 1) == 1)
                            {
                                pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                                cell_map[xxxx][yyyy - 1] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if(start_variable == 2)
    {
        build_map(mapbin);
        size_map = map_size(mapbin);
        build_cells_map_of_map();
        buid_energy_map(game , 1);
        printf("please write number of your cells: \n");
        int num ;
        scanf("%d" , &num);
        int x , y;
        printf("please write name of your cells: \n");
        while (num > 0)
        {
            scanf("%s" , which_name);
            flag = 0;
            while (flag == 0)
            {
                x = rand() % size_map;
                y = rand() % size_map;
                if ( check_be(x , y) == 1)
                {
                    pushBack(&cell1 , 0 , x , y , which_name);
                    flag = 1;
                    cell_map[x][y] = 1 ;
                }
            }
            num = num - 1;
        }
        flag = 0;
        while(flag == 0)
        {
            update_map();
            for(int jj = 3 * size_map; jj > -1; jj--)
            {
                for(int ii = 0; ii < 3 * size_map; ii++)
                {
                    set_color(0 , color_map_arr[ii][jj]);
                    printf("%c" , print_map_arr[ii][jj]);
                }
                printf("\n");
            }
            set_color(15 , 0);
            int direct ;
            printcell(cell1);
            scanf("%d" , &which_cell);
            which_name = n_cell(cell1 , which_cell);
            turn_menu();
            scanf("%d" , &turn);
            if(turn == 5)
                return 0;
            if(turn == 4)
            {
                buid_energy_map(game , 1);
                savecell(cell1 , file1);
                return 0;
            }
            if(turn == 1)
            {
                move_menu();
                scanf("%d" , &direct);
                if(check_be(x_change(x_cell(cell1 , which_name) , direct) , y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name))) == 1)
                {
                    cell_map[x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = 0 ;
                    int xxx = x_change(x_cell(cell1 , which_name) , direct) , yyy = y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name)) , eee = e_cell(cell1 , which_name);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , eee , xxx , yyy , which_name);
                    cell_map[xxx][yyy] = 1;
                }
            }
            if(turn == 3)
            {
                if(check_be_boost_energy(x_cell(cell1 , which_name) , y_cell(cell1 , which_name)) == 1 && e_cell(cell1,which_name) < 86)
                {
                    energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] - 15;
                    int xxx = x_cell(cell1 , which_name) , yyy = y_cell(cell1 , which_name) , eee = e_cell(cell1 , which_name);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , eee + 15 , xxx , yyy , which_name);
                }
            }
            if (turn == 2)
            {
                if(check_be_mitosis(x_cell(cell1 , which_name),y_cell(cell1 , which_name) ,e_cell(cell1 , which_name)) == 1)
                {
                    char name1[10] , name2[10] ;
                    int xxxx = x_cell(cell1 , which_name) ,  yyyy = y_cell(cell1 , which_name) ;
                    printf("choose names of cells:\n");
                    scanf("%s" , &name1);
                    scanf("%s" , &name2);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , 40 , xxxx , yyyy , name1);
                    if (xxxx % 2 == 0)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1, yyyy + 1, name2);
                            cell_map[xxxx + 1][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx - 1, yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1 , yyyy + 1, name2);
                            cell_map[xxxx - 1][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1,yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1 , yyyy , name2);
                            cell_map[xxxx + 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx - 1,yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1, yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 1;
                        }
                    }
                    if (xxxx % 2 == 1)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1 ,yyyy ) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1, yyyy, name2);
                            cell_map[xxxx + 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx - 1, yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1 , yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx + 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1 , yyyy - 1 , name2);
                            cell_map[xxxx + 1][yyyy - 1] = 1;
                        }
                        else if (check_be(xxxx - 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1, yyyy - 1, name2);
                            cell_map[xxxx - 1][yyyy - 1] = 1;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 1;
                        }
                    }
                }
            }
        }
    }
    if(start_variable == 3)
    {
        build_map(mapbin);
        size_map = map_size(mapbin);
        build_cells_map_of_map();
        printf("please write number of your cells: \n");
        int num ;
        scanf("%d" , &num);
        int x , y;
        printf("please write name of your cells: \n");
        while (num > 0)
        {
            scanf("%s" , which_name);
            flag = 0;
            while (flag == 0)
            {
                x = rand() % size_map;
                y = rand() % size_map;
                if ( check_be(x , y) == 1)
                {
                    pushBack(&cell1 , 0 , x , y , which_name);
                    flag = 1;
                    cell_map[x][y] = 1 ;
                }
            }
            num = num - 1;
        }
        printf("please write number of your cells: \n");
        scanf("%d" , &num);
        printf("please write name of your cells: \n");
        while (num > 0)
        {
            scanf("%s" , which_name);
            flag = 0;
            while (flag == 0)
            {
                x = rand() % size_map;
                y = rand() % size_map;
                if ( check_be(x , y) == 1)
                {
                    pushBack(&cell2 , 0 , x , y , which_name);
                    flag = 1;
                    cell_map[x][y] = 2 ;
                }
            }
            num = num - 1;
        }
        flag = 0;
        while (flag == 0)
        {
            update_map();
            for(int jj = 3 * size_map; jj > -1; jj--)
            {
                for(int ii = 0; ii < 3 * size_map; ii++)
                {
                    set_color(0 , color_map_arr[ii][jj]);
                    printf("%c" , print_map_arr[ii][jj]);
                }
                printf("\n");
            }
            set_color(15 , 0);
            int direct ;
            printcell(cell1);
            scanf("%d" , &which_cell);
            which_name = n_cell(cell1 , which_cell);
            turn_menu();
            scanf("%d" , &turn);
            if(turn == 5)
                return 0;
            if(turn == 4)
            {
                buid_energy_map(game , 2);
                savecell(cell1 , file1);
                savecell(cell2 , file2);
                return 0;
            }
            if(turn == 1)
            {
                move_menu();
                scanf("%d" , &direct);
                if(check_be(x_change(x_cell(cell1 , which_name) , direct) , y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name))) == 1)
                {
                    cell_map[x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = 0 ;
                    int xxx = x_change(x_cell(cell1 , which_name) , direct) , yyy = y_change(y_cell(cell1 , which_name),direct , x_cell(cell1 , which_name)) , eee = e_cell(cell1 , which_name);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , eee , xxx , yyy , which_name);
                    cell_map[xxx][yyy] = 1;
                }
            }
            if(turn == 3)
            {
                if(check_be_boost_energy(x_cell(cell1 , which_name) , y_cell(cell1 , which_name)) == 1 && e_cell(cell1,which_name) < 86)
                {
                    energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] = energy_map [x_cell(cell1 , which_name)][y_cell(cell1 , which_name)] - 15;
                    int xxx = x_cell(cell1 , which_name) , yyy = y_cell(cell1 , which_name) , eee = e_cell(cell1 , which_name);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , eee + 15 , xxx , yyy , which_name);
                }
            }
            if (turn == 2)
            {
                if(check_be_mitosis(x_cell(cell1 , which_name),y_cell(cell1 , which_name) ,e_cell(cell1 , which_name)) == 1)
                {
                    char name1[10] , name2[10] ;
                    int xxxx = x_cell(cell1 , which_name) ,  yyyy = y_cell(cell1 , which_name) ;
                    printf("choose names of cells:\n");
                    scanf("%s" , &name1);
                    scanf("%s" , &name2);
                    deletecell(&cell1 , which_name);
                    pushBack(&cell1 , 40 , xxxx , yyyy , name1);
                    if (xxxx % 2 == 0)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1, yyyy + 1, name2);
                            cell_map[xxxx + 1][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx - 1, yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1 , yyyy + 1, name2);
                            cell_map[xxxx - 1][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1,yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1 , yyyy , name2);
                            cell_map[xxxx + 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx - 1,yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1, yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 1;
                        }
                    }
                    if (xxxx % 2 == 1)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 1;
                        }
                        else if (check_be(xxxx + 1 ,yyyy ) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1, yyyy, name2);
                            cell_map[xxxx + 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx - 1, yyyy) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1 , yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 1;
                        }
                        else if (check_be(xxxx + 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx + 1 , yyyy - 1 , name2);
                            cell_map[xxxx + 1][yyyy - 1] = 1;
                        }
                        else if (check_be(xxxx - 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx - 1, yyyy - 1, name2);
                            cell_map[xxxx - 1][yyyy - 1] = 1;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell1 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 1;
                        }
                    }
                }
            }
            update_map();
            for(int jj = 3 * size_map; jj > -1; jj--)
            {
                for(int ii = 0; ii < 3 * size_map; ii++)
                {
                    set_color(0 , color_map_arr[ii][jj]);
                    printf("%c" , print_map_arr[ii][jj]);
                }
                printf("\n");
            }
            set_color(15 , 0);
            int direct2 ;
            printcell(cell2);
            scanf("%d" , &which_cell2);
            which_name2 = n_cell(cell2 , which_cell2);
            turn_menu();
            scanf("%d" , &turn);
            if(turn == 5)
                return 0;
            if(turn == 4)
            {
                buid_energy_map(game , 3);
                savecell(cell1 , file1);
                savecell(cell2 , file2);
                return 0;
            }
            if(turn == 1)
            {
                move_menu();
                scanf("%d" , &direct2);
                if(check_be(x_change(x_cell(cell2 , which_name2) , direct2) , y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2))) == 1)
                {
                    cell_map[x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = 0 ;
                    int xxx = x_change(x_cell(cell2 , which_name2) , direct2) , yyy = y_change(y_cell(cell2 , which_name2),direct2 , x_cell(cell2 , which_name2)) , eee = e_cell(cell2 , which_name2);
                    deletecell(&cell2 , which_name2);
                    pushBack(&cell2 , eee , xxx , yyy , which_name2);
                    cell_map[xxx][yyy] = 2;
                }
            }
            if(turn == 3)
            {
                if(check_be_boost_energy(x_cell(cell2 , which_name2) , y_cell(cell2 , which_name2)) == 1 && e_cell(cell2,which_name2) < 86)
                {
                    energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] = energy_map [x_cell(cell2 , which_name2)][y_cell(cell2 , which_name2)] - 15;
                    int xxx = x_cell(cell2 , which_name2) , yyy = y_cell(cell2 , which_name2) , eee = e_cell(cell2 , which_name2);
                    deletecell(&cell2 , which_name2);
                    pushBack(&cell2 , eee + 15 , xxx , yyy , which_name2);
                }
            }
            if (turn == 2)
            {
                if(check_be_mitosis(x_cell(cell2 , which_name2),y_cell(cell2 , which_name2) ,e_cell(cell2 , which_name2)) == 1)
                {
                    char name1[10] , name2[10] ;
                    int xxxx = x_cell(cell2 , which_name2) ,  yyyy = y_cell(cell2 , which_name2) ;
                    printf("choose names of cells:\n");
                    scanf("%s" , &name1);
                    scanf("%s" , &name2);
                    deletecell(&cell2 , which_name2);
                    pushBack(&cell2 , 40 , xxxx , yyyy , name1);
                    if (xxxx % 2 == 0)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 2;
                        }
                        else if (check_be(xxxx + 1 ,yyyy + 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx + 1, yyyy + 1, name2);
                            cell_map[xxxx + 1][yyyy + 1] = 2;
                        }
                        else if (check_be(xxxx - 1, yyyy + 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx - 1 , yyyy + 1, name2);
                            cell_map[xxxx - 1][yyyy + 1] = 2;
                        }
                        else if (check_be(xxxx + 1,yyyy) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx + 1 , yyyy , name2);
                            cell_map[xxxx + 1][yyyy] = 2;
                        }
                        else if (check_be(xxxx - 1,yyyy) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx - 1, yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 2;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 2;
                        }
                    }
                    if (xxxx % 2 == 1)
                    {
                        if (check_be(xxxx ,yyyy + 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx , yyyy + 1, name2);
                            cell_map[xxxx][yyyy + 1] = 2;
                        }
                        else if (check_be(xxxx + 1 ,yyyy ) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx + 1, yyyy, name2);
                            cell_map[xxxx + 1][yyyy] = 2;
                        }
                        else if (check_be(xxxx - 1, yyyy) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx - 1 , yyyy , name2);
                            cell_map[xxxx - 1][yyyy] = 2;
                        }
                        else if (check_be(xxxx + 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx + 1 , yyyy - 1 , name2);
                            cell_map[xxxx + 1][yyyy - 1] = 2;
                        }
                        else if (check_be(xxxx - 1,yyyy - 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx - 1, yyyy - 1, name2);
                            cell_map[xxxx - 1][yyyy - 1] = 2;
                        }
                        else if (check_be(xxxx , yyyy - 1) == 1)
                        {
                            pushBack(&cell2 , 40 , xxxx , yyyy - 1, name2);
                            cell_map[xxxx][yyyy - 1] = 2;
                        }
                    }
                }
            }
        }
    }
}

