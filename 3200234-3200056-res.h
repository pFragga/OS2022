#define Ntel 3
#define Ncash 2
#define Nseat 10
#define NzoneA 10
#define NzoneB 20
#define PzoneA 30
#define CzoneA 30
#define CzoneB 20
#define Nseatlow 1
#define Nseathigh 5
#define t_reslow 1
#define t_reshigh 5
#define t_seatlow 5
#define t_seathigh 13
#define t_cashlow 4
#define t_cashhigh 8
#define Pcardsuccess 90

/* each customer keeps one of these */
typedef struct info {
    int tickets, seira, cost, status;
    int theseis[Nseathigh];
    char *Zone;
} INFORMATION;

/* initialization of theatre layout */
void gen_theatre();

/* RNG related */
int gen_random(int a, int b);
char *rand_zone();
int rand_tickets();
int try_to_pay();

/* math */
int calc_cost(char *Zone, int tickets);
float calc_percent(int x);

/* modification of the theatre layout */
int check_avail(int threadID, char *Zone, int tickets, INFORMATION *cust_info);
void cancel_seats(INFORMATION *cust_info);

/* multithreading */
void tameio(int threadID, INFORMATION *cust_info);
void thlefwnhma(int threadID, INFORMATION *cust_info);
void *krathsh(void *tid);
void *create_threads(void *Ncust);
