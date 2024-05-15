#include <stdlib.h>
#include <stdio.h>
#include <string.h>	
#include <time.h>
#include <math.h>


int *n;
int *m;
int *k;
int log_limit;
char manual_word[4096];
int mode;

int two_power_of(double num){
    int toReturn = (int)pow(2.0,num);
    return toReturn;
}

int checkPowerOfTwo (int num){
    int flag = 1;
    while(num != 1 && flag){
        if(num % 2 != 0){
            flag = 0;
            fprintf(stdout,"Error: number isn't power of 2.\n");
        }
        num >>= 1;
    }
    return flag;
}

int option_insert_n_value(){
    int num;
    fprintf(stdout,"Please enter size of 'n'\n");
    scanf("%d" , &num);
    return num;
}

int option_insert_n_by_power(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter power for 'n'\n");
        scanf("%d" , &num);
        if (num > 30)
            fprintf(stdout,"Error: 'n' is too big (cannot be more ther 2^30)\n");
        else
            got_right_number = 1;
    }
    return two_power_of(num);
}

int get_n_value (){
    if (log_limit == 1)
        return option_insert_n_by_power(); 
    else {
        return option_insert_n_value();
    }
}

int option_insert_m_value(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter size of 'm'\n");
        scanf("%d" , &num);
        if (num <= (*n))
            got_right_number = 1;
        else
            fprintf(stdout,"Error: 'm' that is larger than 'n'\n");
    }
    return num;
}

int option_insert_m_by_power(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter power for 'm'\n");
        scanf("%d" , &num);
        if (num > 30)
            fprintf(stdout,"Error: 'm' is too big (cannot be more ther 2^30)\n");
        else if (two_power_of(num)>(*n))
            fprintf(stdout,"Error: 'm' that is larger than 'n'\n");
        else
            got_right_number = 1;
    }
    return two_power_of(num);
}

int get_m_value (){
    if (log_limit == 1)
        return option_insert_m_by_power();
    else {
        return option_insert_m_value();
    }
}

int calc_log(int num){
    int counter = 0;
    while(num != 1){
        num >>= 1;
        counter++;
    }
    return counter;
}

double get_a_value(int logm){
    double num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter size of 'a' (must be an integer and a>=1)\n");
        scanf("%lf" , &num);
        if (num < 1)
            fprintf(stdout,"Error: 'a' that is smaller than '1'\n");
        else if (num*logm > (*m))
            fprintf(stdout,"Error: 'k' that is bigger than 'm'\n");
        else 
            got_right_number = 1;
    }
    return num;
}

int option_insert_k(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter size of 'k'\n");
        scanf("%d" , &num);
        if (num > (*m))
            fprintf(stdout,"Error: 'k' that is bigger than 'm'\n");
        else 
            got_right_number = 1;
    }
    return num; 
}

int get_k_value (){
    double a;
    int option;
    int logm = calc_log(*m);
    fprintf(stdout,"Select which number do you want to insert:\n1)'k'\n2)'a'\n");
    scanf("%d" , &option);
    if (option == 1)
        return option_insert_k();
    else {
        a = get_a_value(logm);
        return a*logm;
    }
}

int get_k_value_without_m(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter size of 'k' smaller than 'n' (be advised not to enter k to big... will effect the running time)\n");
        scanf("%d" , &num);
        if (num > (*n))
            fprintf(stdout,"Error: 'k' that is bigger than 'n'\n");
        else 
            got_right_number = 1;
    }
    return num; 
}

int get_number_of_iterations(){
    int num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter number of iterations (must be an integer and bigger than  0)\n");
        scanf("%d" , &num);
        if (num < 1)
            fprintf(stdout,"Error: number of iterations must be bigger then '0'\n");
        else 
            got_right_number = 1;
    }
    return num;
}

char create_random_bit(){
    char bit_toReturn;
    int random_number = rand();
    if (random_number % 2 == 0)
        bit_toReturn = '0';
    else
        bit_toReturn = '1';
    return bit_toReturn;
}

void create_random_word (char* word){
    for (int i = 0;i<(*n);i++){
        char bit = create_random_bit();
        word[i] = bit;
    }
}

void set_memory_to_initial_value(int* mem , int size ,int value ){
    for (int i = 0;i<size;i++){
        mem[i] = value;
    }
}

int find_first_k_tuple(char* word){
    int toReturn = 0;
    for(int i = 0;i < (*k);i++){
        toReturn<<=1;
        if (word[i] == '1')
            toReturn++;
    }
    return toReturn;
}
int create_mask(){
    int toReturn = 0;
    for(int i =0; i<(*k);i++){
        toReturn <<= 1;
        toReturn = toReturn | 1;
    }
    return toReturn;
}

int find_next_tuple(char* word, int last_k_tuple, int new_k_start_index){
    int mask = create_mask();
    int new_k_tuple = last_k_tuple << 1;
    int newbit;
    new_k_tuple  = new_k_tuple & mask;
    if(word[new_k_start_index+((*k)-1)] == '1')
        newbit = 1;
    else
        newbit = 0;
    new_k_tuple = new_k_tuple | newbit;
    //fprintf(stdout,"the new k tuple is : %d\n",new_k_tuple);
    return new_k_tuple;        

}

int check_word_k_hashmap_method(char* word){
    int is_in_Lkmn = 1;
    int size_of_hashmap = two_power_of((*k));
    int* k_hashmap = malloc(size_of_hashmap*sizeof(int));
    int k_tuple;
    set_memory_to_initial_value(k_hashmap , size_of_hashmap , -1 );
    k_tuple = find_first_k_tuple(word);
    k_hashmap[k_tuple] = 0;
    for(int i = 1; (i <= ((*n)-(*k)))&&(is_in_Lkmn == 1);i++){
        k_tuple = find_next_tuple(word,k_tuple,i);
        if(k_hashmap[k_tuple] == -1)
            k_hashmap[k_tuple] = i;
        else if (i-k_hashmap[k_tuple] <= ((*m)-(*k))){
            is_in_Lkmn = 0;
        //    fprintf(stdout , "found reapet in indexes %d and %d\n" , k_hashmap[k_tuple] , i);
        }
        else     
             k_hashmap[k_tuple] = i;             
    }
    free(k_hashmap);
    return is_in_Lkmn;
}

int check_k_is_not_in_hashmap(char** m_hashmap, int index){
    for (int i=0;i<(*m);i++){
        if(i!=index){
            if(strcmp(m_hashmap[i],m_hashmap[index])==0){
                return 0;
            }
        }
    }
    return 1;
}

int check_word_m_hashmap_method(char* word){
    int is_in_Lkmn = 1;
    char **m_hashmap = malloc((*m) * sizeof(char *));
    for (int i = 0; i < (*m); i++) {
        m_hashmap[i] = malloc( ((*k) + 1) * sizeof(char)); // +1 for null terminator
    }
    for (int i = 0; i <= ((*n)-(*k)) && is_in_Lkmn; i++){
        strncpy(m_hashmap[i%(*m)], word + i, *k);
        m_hashmap[i%(*m)][*k] = '\0'; //adding null terminator;
        if (check_k_is_not_in_hashmap(m_hashmap ,i%(*m))==0)
            is_in_Lkmn = 0;
    }
    for (int i = 0; i < (*m); i++) {
        free(m_hashmap[i]);
    }
    free(m_hashmap);
    return is_in_Lkmn;
}

int check_word(char* word){
    if ((*k)<25)
        return check_word_k_hashmap_method(word);
    else
        return check_word_m_hashmap_method(word);
}

int create_word_and_check(){
    int word_in_Lkmn;
    char* word = malloc((*n)*sizeof(char));
    create_random_word(word);
    if (check_word(word))
        word_in_Lkmn = 1;
    else 
        word_in_Lkmn = 0;
    //fprintf(stdout,"The word we created is:\n%s\n",word);
    free(word);
    return word_in_Lkmn;
}

int log_limit_question(){
    int option;
    fprintf(stdout,"Do you want to limit yourself to 'n' & 'm' that are only power of 2?\n1)Yes\n2)No\n");
    scanf("%d" , &option);
    if (option == 1)
        return 1;
    else
        return 0;
}

int choose_mode(){
    int option;
    fprintf(stdout,"Please choose mode for the program:\n1)Check manual entered word\n2)Choose parameters for random sequence\n3)Automated simulation\n");
    scanf("%d" , &option);
    if (option == 1)
        return 1;
    else if (option == 2)
        return 2;
    else 
        return 3;
}

void random_sequence(){
    int num_of_iter;
    int counter_words_in_Lkmn = 0;
    double success_rate;
    log_limit = log_limit_question();
    (*n) = get_n_value();
    fprintf(stdout,"\n");
    (*m) = get_m_value();
    fprintf(stdout,"\n");
    (*k) = get_k_value();
    fprintf(stdout,"\n");
    num_of_iter = get_number_of_iterations();
    fprintf(stdout,"The parameters of the run are:\nn : %d\nm : %d\nk : %d\nnumber of iretetions : %d\n\n",(*n) , (*m) , (*k) , num_of_iter );
    srand(time(NULL));
    for (int i=0; i<num_of_iter; i++){
        if (create_word_and_check() == 1)
            counter_words_in_Lkmn++;
    }
    success_rate = ((double)counter_words_in_Lkmn/(double)num_of_iter)*100;
    fprintf(stdout,"The success rate is:\n%f%%\n",success_rate );
}

int check_valid_word(){
    for (int i=0;i<strlen(manual_word);i++){
        if (manual_word[i] != '0'&& manual_word[i] != '1')
            return 0;
    }
    return 1;
}

void check_manual_entered_word(){
    int got_valid_word  = 0;
    log_limit = 0;
    while(got_valid_word == 0){
        fprintf(stdout,"Please enter valid word to check (for now we limit the length of the word to 4096 bits)\n"); 
        scanf("%s",manual_word); 
        if (check_valid_word()==0)
            fprintf(stdout,"Error: The word is not valid, try again\n");
        else
            got_valid_word = 1;
    }
    (*n) = strlen(manual_word);
    (*m) = get_m_value();
    fprintf(stdout,"\n");
    (*k) = get_k_value();
    fprintf(stdout,"\n");
    fprintf(stdout,"The parameters of the run are:\nn : %d\nm : %d\nk : %d\n\n\n",(*n) , (*m) , (*k));
    if (check_word(manual_word) == 1)
        fprintf(stdout,"The word you entered is in Lmkn\n");
    else
        fprintf(stdout,"The word you entered is *NOT* in Lmkn\n");
}

void display_graph(const char* filename){
    FILE* gnuplotPipe = popen("gnuplot -p","w");
    fprintf(gnuplotPipe , "plot 'dataTodisplay'\n");
    fflush(gnuplotPipe);
    fclose(gnuplotPipe);
}

void make_graph(int point_for_graph[][2], int number_of_points){  
    const char* filename =  "./dataTodisplay";
    FILE* data = fopen(filename,"w+");
    for (int i = 0; i<number_of_points;i++){
        fprintf(data , "%d %d\n",point_for_graph[i][0],point_for_graph[i][1]);
    }
    fflush(data); 
    fclose(data);
    display_graph(filename);
    remove(filename);
}


void choose_n_m_option(){
    int num_of_iter;
    int counter_words_in_Lkmn;
    double success_rate;
    int logm;
    log_limit = log_limit_question();
    (*n) = get_n_value();
    fprintf(stdout,"\n");
    (*m) = get_m_value();
    fprintf(stdout,"\n");
    logm = calc_log(*m);
    int points_for_graph[25-logm][2];
    int number_of_points = 25-logm;
    fprintf(stdout,"The parameters of the run are:\nn : %d\nm : %d\n\nk | rate\n",(*n) , (*m));
    for (int i = logm;i<25;i++){
        if (i<21)
            num_of_iter = 1000;
        else   
            num_of_iter = 100;
        counter_words_in_Lkmn = 0;
        success_rate = 0;
        (*k) = i;
        srand(time(NULL));
        for (int i=0; i<num_of_iter; i++){
            if (create_word_and_check() == 1)
                counter_words_in_Lkmn++;
        }
        success_rate = ((double)counter_words_in_Lkmn/(double)num_of_iter)*100;
        fprintf(stdout,"%d | %f%%\n",*k ,success_rate );
        points_for_graph[i-logm][0] = i;
        points_for_graph[i-logm][1] = (int)success_rate;
    }
    make_graph(points_for_graph , number_of_points);
}

int find_number_of_iteration(){
    if ((*k)<20 && (*n)<500000){
        return 1000;
    }
    else if ((*k)<25 && (*n)<5000000)
        return 100;
    else
        return 10;
}

int get_min (int x,int y){ //if we get 0 we return other value - Be advised
    if (x == 0)
        return y;
    else if (x<y || y == 0)
        return x;
    else
        return y;
}

void choose_n_k_option(){
    int number_of_points;
    int num_of_iter;
    int counter_words_in_Lkmn;
    double success_rate;
    int jumps;
    log_limit = log_limit_question();
    (*n) = get_n_value();
    fprintf(stdout,"\n");
    (*k) = get_k_value_without_m();
    fprintf(stdout,"\n");
    int max_m = get_min((*n),two_power_of(*k));
    if ((max_m-(*k))<201){
        jumps = 1;
        number_of_points = (max_m-(*k));
    }
    else{
        jumps = (max_m-(*k))/200;
        number_of_points = 200;
    }
    int points_for_graph[number_of_points][2];
    num_of_iter = find_number_of_iteration();
    fprintf(stdout,"The parameters of the run are:\nn : %d\nk : %d\nnumber of iretetions : %d\n\nm | rate\n",(*n) , (*k) , num_of_iter );
    for (int i = 0;i<number_of_points;i++){
        counter_words_in_Lkmn = 0;
        success_rate = 0;
        (*m) = (*k)+(i*jumps);
        srand(time(NULL));
        for (int i=0; i<num_of_iter; i++){
            if (create_word_and_check() == 1)
                counter_words_in_Lkmn++;
        }
        success_rate = ((double)counter_words_in_Lkmn/(double)num_of_iter)*100;
        fprintf(stdout,"%d | %f%%\n",*m ,success_rate );
        points_for_graph[i][0] = (*m);
        points_for_graph[i][1] = (int)success_rate;
    }
    make_graph(points_for_graph , number_of_points);
}

double get_a_value_for_simulation(){
    double num;
    int got_right_number  = 0;
    while(got_right_number == 0){
        fprintf(stdout,"Please enter size of 'a' (must be an integer and a>=1), and remember that in any case k will not be bigger than 22\n");
        scanf("%lf" , &num);
        if (num < 1)
            fprintf(stdout,"Error: 'a' that is smaller than '1'\n");
        else 
            got_right_number = 1;
    }
    return num;
}

void choose_n_a_option(){
    int counter_words_in_Lkmn;
    double success_rate;
    int number_of_iterations = 1000;
    int max_val_for_k;
    double a;
    log_limit = log_limit_question();
    (*n) = get_n_value();
    fprintf(stdout,"\n");
    a = get_a_value_for_simulation();
    if ((*n)>two_power_of((int)(22/a)))
        max_val_for_k=22;
    else
        max_val_for_k = (int)(calc_log(*n)*a);
    int points_for_graph[max_val_for_k][2];
    fprintf(stdout,"The parameters of the run are:\nn : %d\na : %lf\nnumber of iretetions : %d\n\nk | m | rate\n",(*n) , a , number_of_iterations );
    for (int i = 0;i<max_val_for_k;i++){
        counter_words_in_Lkmn = 0;
        success_rate = 0;
        (*k) = i+1;
        (*m) = two_power_of((double)((*k)/a));
        srand(time(NULL));
        for (int i=0; i<number_of_iterations; i++){
            if (create_word_and_check() == 1)
                counter_words_in_Lkmn++;
        }
        success_rate = ((double)counter_words_in_Lkmn/(double)number_of_iterations)*100;
        fprintf(stdout,"%d | %d | %f%%\n",*k, *m, success_rate);
        points_for_graph[i][0] = (*k);
        points_for_graph[i][1] = (int)success_rate;
    }
    make_graph(points_for_graph , max_val_for_k);
    
    
}

void choose_n_run_interesting_cases(){
    fprintf(stdout,"Not implemented yet");
}

void automated_simulation(){
    int option;
    fprintf(stdout,"Please choose type of simulation:\n1)Choose 'n' & 'm' and run for 'k' (k will go from logm to 21)\n2)Choose 'n' & 'k' and run for 'm'\n3)Choose 'n' & 'a' and run for 'm' & 'k' while preserving the ratio k=a*log(m)\n4)Choose 'n' and run for intresting 'm' & 'k'\n");
    scanf("%d" , &option);
    if (option == 1)
        choose_n_m_option();
    else if (option == 2)
        choose_n_k_option();
    else if (option == 3)
        choose_n_a_option();
    else //(option == 4)
        choose_n_run_interesting_cases(); 
}

int main(int argc, char **argv){
	n = malloc(sizeof(int));
    m = malloc(sizeof(int));
    k = malloc(sizeof(int));
    mode = choose_mode();
    if (mode == 1)
        check_manual_entered_word();
    else if (mode ==2)
        random_sequence();
    else{ //mode == 3
        automated_simulation();
    }
	free(n);
    free(m);
    free(k);
	exit(0);
}

