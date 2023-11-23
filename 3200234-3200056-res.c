#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "3200234-3200056-res.h"

/* mutexes & condition variables */
pthread_mutex_t mutex_thl, mutex_tam, mutex_thes, mutex_account, mutex_trans,
				mutex_print;
pthread_cond_t cond_thl, cond_tam;

/* counters */
int dia8esimoi_thl = Ntel;
int dia8esimoi_tam = Ncash;

/* plan */
int theatre[NzoneA + NzoneB][Nseat];

/* company account */
int company_account = 0;

/* waiting times */
int thl_wait_time = 0;
int tam_wait_time = 0;
int total_wait_time = 0;

/* successful & unsuccessful transactions */
int s_transactions, f_transactions_thl, f_transactions_tam;

/* command line arguments */
unsigned int seed;
int Ncust;

void
gen_theatre()
{
	for (int i = 0; i < NzoneA + NzoneB; i++) {
		for (int j = 0; j < Nseat; j++) {
			theatre[i][j] = 0;
		}
	}
}

int
gen_random(int a, int b) {
	return (rand_r(&seed) % (b - a + 1) + a);
}

char *
rand_zone()
{
	if (gen_random(0, 100) <= PzoneA) {
		return "A";
	}
	return "B";
}

int
rand_tickets()
{
	return gen_random(Nseatlow, Nseathigh);
}

int
try_to_pay()
{
	if (gen_random(0, 100) <= Pcardsuccess) {
		return 0;
	}
	return 1;
}

int
calc_cost(char *Zone, int tickets)
{
	if (Zone == "A") {
		return tickets * CzoneA;
	}
	return tickets * CzoneB;
}

float
calc_percent(int x)
{
	return (float) x / Ncust * 100.0;
}

/* this modifies the theatre layout */
int
check_avail(int threadID, char *Zone, int tickets, INFORMATION *cust_info)
{
	pthread_mutex_lock(&mutex_thes);
	int synexomenes = 0;
	int first_seat;
	int seira;
	int theseis[Nseathigh];

	int start_of_seats;
	int end_of_seats;
	if(Zone == "A") {
		start_of_seats = 0;
		end_of_seats = NzoneA;
	} else {
		start_of_seats = NzoneA;
		end_of_seats = NzoneA + NzoneB;
	}

	for (int i = start_of_seats; i < end_of_seats; i++) {
		synexomenes = 0;
		for (int j = 0; j < Nseat; j++) {
			if (theatre[i][j] == 0) {
				synexomenes++;
				if (synexomenes == 1) {
					first_seat = j;
				}
			} else {
				if (synexomenes < tickets) {
					synexomenes = 0;
				}
			}
		}

		// an oi synexomenes theseis einai arketes desmeuse tis
		if (synexomenes >= tickets) {
			seira = i;
			for (int j = first_seat; j < first_seat+tickets; j++) {
				theatre[i][j] = threadID;
				theseis[j-first_seat] = j;
			}
			break;
		}
	}

	pthread_mutex_unlock(&mutex_thes);

	if (synexomenes < tickets) {
		return -1;
	} else {
		cust_info->seira = seira;
		for (int i = 0; i < Nseathigh; i++) {
			cust_info->theseis[i] = theseis[i];
		}
		return 0;
	}
}

void
cancel_seats(INFORMATION *cust_info)
{
	pthread_mutex_lock(&mutex_thes);
	for (int j = cust_info->theseis[0]; j < cust_info->theseis[0] + cust_info->tickets; j++) {
		theatre[cust_info->seira][j] = 0;
	}
	pthread_mutex_unlock(&mutex_thes);
}


void
tameio(int threadID, INFORMATION *cust_info)
{
	dia8esimoi_tam--;
	pthread_mutex_unlock(&mutex_tam);

	// O tameias prwta perimenei
	unsigned int anamoni = gen_random(t_cashlow, t_cashhigh);
	pthread_mutex_lock(&mutex_print);
	printf("Pelaths %d: Dokimase na plhrwseis...\n", threadID);
	pthread_mutex_unlock(&mutex_print);
	sleep(anamoni);

	// Meta dokimazei na kanei thn plhwmh, kai an ginei ta lefta phgainoun sthn etairia
	if (try_to_pay() == 0) {
		/* successful payment */
		pthread_mutex_lock(&mutex_account);
		company_account += cust_info->cost;
		pthread_mutex_unlock(&mutex_account);
	} else {
		/* payment unsuccessful, cancel the seats */
		cancel_seats(cust_info);
		cust_info->status = 1;
		pthread_mutex_lock(&mutex_trans);
		s_transactions--;
		f_transactions_tam++;
		pthread_mutex_unlock(&mutex_trans);
	}

	pthread_mutex_lock(&mutex_tam);
	dia8esimoi_tam++;
	pthread_mutex_unlock(&mutex_tam);
	pthread_cond_broadcast(&cond_tam);
}

void
thlefwnhma(int threadID, INFORMATION *cust_info)
{
	dia8esimoi_thl--;
	pthread_mutex_unlock(&mutex_thl);

	// O thlefwnhths prwta perimenei
	unsigned int anamoni = gen_random(t_seatlow, t_seathigh);
	pthread_mutex_lock(&mutex_print);
	printf("Pelaths %d: Psakse na mou breis 8eseis...\n", threadID);
	pthread_mutex_unlock(&mutex_print);
	sleep(anamoni);

	// Meta koitaei gia 8eseis kai an brei, ypologizei to kostos
	cust_info->tickets = rand_tickets();
	cust_info->Zone = rand_zone();
	if (check_avail(threadID, cust_info->Zone, cust_info->tickets, cust_info) != 0) {
		// apotyxia stis 8eseis
		cust_info->status = -1;
		pthread_mutex_lock(&mutex_trans);
		s_transactions--;
		f_transactions_thl++;
		pthread_mutex_unlock(&mutex_trans);
	} else {
		// epityxia stis 8eseis
		cust_info->cost = calc_cost(cust_info->Zone, cust_info->tickets);
	}

	pthread_mutex_lock(&mutex_thl);
	dia8esimoi_thl++;
	pthread_mutex_unlock(&mutex_thl);
	pthread_cond_broadcast(&cond_thl);
}

void *
krathsh(void *tid)
{
	int threadID = *(int *) tid;
	INFORMATION cust_info;

	pthread_mutex_lock(&mutex_thl);
	while (dia8esimoi_thl == 0) {
		pthread_mutex_lock(&mutex_print);
		printf("Pelaths %d: 0 thlefwnhtes dia8esimoi. Perimenw...\n", threadID);
		pthread_mutex_unlock(&mutex_print);
		pthread_cond_wait(&cond_thl, &mutex_thl);
	}

	thlefwnhma(threadID, &cust_info);

	if (cust_info.status == -1) {
		pthread_mutex_lock(&mutex_print);
		printf("\n------------ Pelaths %d ------------\nH krathsh apetyxe epeidh den yparxoun arketes 8eseis sthn Zwnh %s.\n------------------------------------\n\n", threadID, cust_info.Zone);
		pthread_mutex_unlock(&mutex_print);
	} else {
		pthread_mutex_lock(&mutex_tam);
		while (dia8esimoi_tam == 0) {
			pthread_mutex_lock(&mutex_print);
			printf("Pelaths %d: 0 tameies dia8esimoi. Perimenw...\n", threadID);
			pthread_mutex_unlock(&mutex_print);
			pthread_cond_wait(&cond_tam, &mutex_tam);
		}

		tameio(threadID, &cust_info);

		pthread_mutex_lock(&mutex_print);
		if (cust_info.status == 1) {
			printf("\n------------ Pelaths %d ------------\nH krathsh apetyxe giati h synallagi me tin karta den egine apodekth.\n", threadID);
		} else {
			printf("\n------------ Pelaths %d ------------\nH krathsh oloklhrw8hke epityxws. Oi 8eseis sou einai:\n", threadID);
			for (int i = 0; i < cust_info.tickets; i++) {
				if (cust_info.Zone == "A") {
					printf("Zwnh %s, Seira %d, 8esh %d\n", cust_info.Zone, cust_info.seira+1, cust_info.theseis[i]+1);
				} else {
					printf("Zwnh %s, Seira %d, 8esh %d\n", cust_info.Zone, cust_info.seira-9, cust_info.theseis[i]+1);
				}
			}
			printf("Kostizei(oun) %d$ synolika.\n", cust_info.cost);
		}
		printf("------------------------------------\n\n"); pthread_mutex_unlock(&mutex_print);
	}
}

void *
create_threads(void *Ncust)
{
	int customers = *((int *) Ncust);
	int tIDs[customers];

	/* dynamically allocate memory for customer threads */
	pthread_t *th;
	th = malloc(sizeof(pthread_t) * customers);
	if (th == NULL) {
		printf("ERROR: Not enough memory to create customer threads\n");
		exit(-1);
	}

	for (int i = 0; i < customers; i++) {
		tIDs[i] = i+1;

		if (tIDs[i] != 1) {
			unsigned int anamoni = gen_random(t_reslow, t_reshigh);
			// printf("Pelaths %d: %d seconds until start.\n", tIDs[i], anamoni);
			sleep(anamoni);
		}

		if (pthread_create(&th[i], NULL, &krathsh, &tIDs[i]) != 0) {
			printf("Failed to create thread.\n");
			exit(-1);
		}
	}

	for (int i = 0; i < customers; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			printf("Failed to join thread.\n");
			exit(-1);
		}
	}

	/* always remember to deallocate memory afterwards */
	free(th);

	printf("\n------------ Printing theater layout... ------------\n");
	for (int i = 0; i < NzoneA + NzoneB; i++) {
		for (int j = 0; j < Nseat; j++) {
			if (i < NzoneA) {
				if (theatre[i][j] == 0) {
					printf("Zwnh: A, Seira: %d, 8esh: %d -> EMPTY\n", i+1, j+1);
				} else {
					printf("Zwnh: A, Seira: %d, 8esh: %d -> Pelaths %d\n", i+1, j+1, theatre[i][j]);
				}
			} else {
				if (theatre[i][j] == 0) {
					printf("Zwnh: B, Seira: %d, 8esh: %d -> EMPTY\n", i-9, j+1);
				} else {
					printf("Zwnh: B, Seira: %d, 8esh: %d -> Pelaths %d\n", i-9, j+1, theatre[i][j]);
				}
			}
		}
	}
	printf("----------------------------------------------------\n\n------------ Stats ------------\n");

	printf("Esoda apo tis pwlhseis: %d$.\n", company_account);
	printf("Krathseis me epityxia: %.2f%%\n", calc_percent(s_transactions));
	printf("Krathseis me apotyxia logw 8esewn: %.2f%%\n", calc_percent(f_transactions_thl));
	printf("Krathseis me apotyxia logw pistothkhs: %.2f%%\n", calc_percent(f_transactions_tam));
	printf("Mesos xronos anamonhs pelatwn: N/A\n"); // TODO
	printf("Mesos xronos eksyphrethshs pelatwn: N/A\n"); // TODO
	printf("-------------------------------\n");
}

int
main(int argc, char const **argv)
{
	/* ensure 2 arguments were given */
	if (argc != 3) {
		printf("test-res: illegal number of arguments\n");
		exit(-1);
	}

	/* ensure first argument is a positive integer */
	if (atoi(argv[1]) < 0) {
		printf("test-res: illegal argument for RNG seed (must be integer)\n");
		exit(-1);
	}

	Ncust = atoi(argv[1]);
	s_transactions = Ncust;
	seed = atoi(argv[2]);
	gen_theatre();

	printf("To programma 8a eksyphrethsei %d pelates.\n", Ncust);

	/* dynamically allocate memory for initial thread only */
	pthread_t *t1;
	t1 = malloc(sizeof(pthread_t));
	if (t1 == NULL) {
		printf("ERROR: Not enough memory to create initial thread!\n");
		return -1;
	}

	pthread_mutex_init(&mutex_thl, NULL);
	pthread_mutex_init(&mutex_tam, NULL);
	pthread_mutex_init(&mutex_thes, NULL);
	pthread_mutex_init(&mutex_account, NULL);
	pthread_mutex_init(&mutex_trans, NULL);
	pthread_mutex_init(&mutex_print, NULL);
	pthread_cond_init(&cond_thl, NULL);
	pthread_cond_init(&cond_tam, NULL);

	if (pthread_create(&(*t1), NULL, &create_threads, &Ncust) != 0) {
		perror("Failed to create initial thread.\n");
	}

	if (pthread_join(*t1, NULL) != 0) {
		perror("Failed to join initial thread.");
	}

	/* mutexed and condition variables must be destroyed afterwards */
	pthread_mutex_destroy(&mutex_thl);
	pthread_mutex_destroy(&mutex_tam);
	pthread_mutex_destroy(&mutex_thes);
	pthread_mutex_destroy(&mutex_account);
	pthread_mutex_destroy(&mutex_trans);
	pthread_cond_destroy(&cond_thl);
	pthread_cond_destroy(&cond_tam);
	pthread_mutex_destroy(&mutex_print);

	free(t1);

	return 0;
}
