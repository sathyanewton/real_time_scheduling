#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <algorithm>
#include <vector> 
#include <cmath>
#include <pthread.h>
#include <semaphore.h>

#define NUMBER_OF_TICKS 30
#define UP 11
#define DOWN 99
#define ACTIVE 1
#define DEAD 0

using namespace std;


class task_set
{
		public:
			int task_number;
			int period;
			int time_stash;
			int execution_time;
			int relative_deadline;
			int arrival_delay;
			int status;
			int number_of_schedules_RMS;
			int number_of_schedules_EDF;

			int RMS_priority;

			int EDF_deadline;
			int EDF_deadline_stash;
			int EDF_priority;

};

class gant_chart
{
		public:	
			int arrow_head;
//			int status;
//			int task_number;
//			int time_remaining;
//			int RMS_priority;

};

vector <int> result_array_RMS;
vector <int> result_array_EDF;


/* void print functions */

void print_task_table(task_set *task,int line_count)
{
	int i=0;
	cout<<"task_number || period || execution_time || relative_deadline || arrival_delay || status || time_stash || RMS_priority || EDF_deadline || EDF_deadline_stash || EDF_priority\n";
	for(i=0;i<line_count;i++)
	{
		
		cout<<task[i].task_number<<" --> "<<task[i].period<<" "<<task[i].execution_time<<" "<<task[i].relative_deadline<<" "<<task[i].arrival_delay<<" "<<task[i].status<<" "<<task[i].time_stash<<" "<<task[i].RMS_priority<<" "<<task[i].EDF_deadline<<" "<<task[i].EDF_deadline_stash<<" "<<task[i].EDF_priority<<" "<<task[i].number_of_schedules_RMS<<" "<<task[i].number_of_schedules_EDF<<"\n";
	}
}

void print_gant_chart(gant_chart **gant, int line_count)
{
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=0;j<NUMBER_OF_TICKS;j++)
		{
			cout<<gant[j][i].arrow_head<<" ";
		}
		cout<<"\n";
	}		

}
void print_priority(task_set *task,gant_chart **gant,int line_count)
{
	int i=0;
	for(i=0;i<line_count;i++)
	{
		cout<<"Task number --> "<<task[i].task_number<<"Priotiy -- >"<<task[i].RMS_priority<<"\n";
	}
}

void print_EDF_priority(task_set *task,gant_chart **gant,int line_count)
{
	int i=0;
	for(i=0;i<line_count;i++)
	{
		cout<<"Task number --> "<<task[i].task_number<<"EDF Priotiy -- >"<<task[i].EDF_priority<<"\n";
	}
}

void print_result_RMS()
{
	int i=0;
	cout<<"printing results for RMS scheulabling\n";
	for(i=0;i<result_array_RMS.size();i++)
	{
		cout<<"time slice --> "<<i<<"task --> "<<result_array_RMS[i]<<"\n";
	}
}

void print_result_EDF()
{
	int i=0;
	cout<<"printing results for EDF scheulabling\n";
	for(i=0;i<result_array_EDF.size();i++)
	{
		cout<<"time slice --> "<<i<<"task --> "<<result_array_EDF[i]<<"\n";
	}
}

void print_refereces(task_set *task, int line_count)
{
	int i=0;
	cout<<"print references \n";
	cout<<"RMS first \n";
	for(i=0;i<line_count;i++)
	{
		cout<<"Task number : "<<i<<" number of references : "<<task[i].number_of_schedules_RMS<<"\n";
	}
	cout<<"EDF first \n";
	for(i=0;i<line_count;i++)
	{
		cout<<"Task number : "<<i<<" number of references : "<<task[i].number_of_schedules_EDF<<"\n";
	}
}

/* Compute functions : */

/* Schedulability Test : */

int check_RMS_schedulability(task_set *task,int number_of_tasks)
{
	int scheulable = 0;
	float utilization_bound = 0;
	float stash = 0;
	float goma = 0;
	float sum_task_util=0;
	int i=0,j=0;

	float* task_util = new float[number_of_tasks];

	goma = (float) ((float)1/(float)number_of_tasks);

	//	cout << number_of_tasks<<"\n";

	// printf("%lf\n",goma);

	stash = pow(2,goma);

	// printf("%lf\n",stash);

	utilization_bound = (float)(number_of_tasks*(stash-1));

	printf("utilization bound --> %lf\n",utilization_bound);

	for(i=0;i<number_of_tasks;i++)
	{
		task_util[i] = (float)((float)task[i].execution_time / (float)task[i].period);
		sum_task_util = sum_task_util + task_util[i];
		printf("util of task %d --> %lf\n",task[i].task_number,task_util[i]);
	}
	printf("sum util --> %lf\n",sum_task_util);

	if(sum_task_util < utilization_bound)
	{
		delete[] task_util;
		return 1;
	}
	else 
	{
		delete[] task_util;
		return 0;
	}

}

int check_EDF_schedulability(task_set *task,int number_of_tasks)
{

	int scheulable = 0;
	float utilization_bound = 0;
	float stash = 0;
	float goma = 0;
	float sum_task_util=0;
	int i=0,j=0;

	float* task_util = new float[number_of_tasks];

	for(i=0;i<number_of_tasks;i++)
	{
		task_util[i] = (float)((float)task[i].execution_time / (float)task[i].period);
		sum_task_util = sum_task_util + task_util[i];
		printf("util of task %d --> %lf\n",task[i].task_number,task_util[i]);
	}
	printf("sum util --> %lf\n",sum_task_util);

	if(sum_task_util < 1)
	{
		delete[] task_util;
		return 1;
	}
	else 
	{
		delete[] task_util;
		return 0;
	}
}

/* gant functions */

int initalize_gant(gant_chart **gant, int line_count)
{
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=0;j<NUMBER_OF_TICKS;j++)
		{
			gant[i][j].arrow_head = 0;
		//	gant[i][j].status = DEAD;
		}
	}
	return 1;	
}

int populate_gant(task_set *task,gant_chart **gant,int line_count)
{
	int i=0,j=0;

	/* mark UPs */

	for(i=0;i<line_count;i++)
	{
		// cout<<"goma1"<<"\n";
		for(j=0;j<NUMBER_OF_TICKS;j++)
		{
		//	gant[j][i].task_number = i;
			//cout<<"goma2"<<"\n";
			if(task[i].arrival_delay == 0)
			{
					if((j % task[i].period ) == 0)
					{
						gant[j][i].arrow_head = UP;
						if(((j + task[i].relative_deadline) < NUMBER_OF_TICKS) && task[i].relative_deadline != 0)
						{
							gant[j + task[i].relative_deadline][i].arrow_head = DOWN;
						}
					}
			}
			else if(task[i].arrival_delay != 0)
			{
				if((j % task[i].period) == 0)
				{
					if(j<(NUMBER_OF_TICKS-task[i].arrival_delay))
					{
						gant[j+(task[i].arrival_delay)][i].arrow_head = UP;
						if(((j + task[i].arrival_delay + task[i].relative_deadline) < NUMBER_OF_TICKS) && task[i].relative_deadline != 0)
						{
							gant[j + task[i].arrival_delay + task[i].relative_deadline][i].arrow_head = DOWN;
						}
					}
				}
			}
		}
	}

return 1;
}



int update_gant_priority(task_set *task,gant_chart **gant,int line_count)
{
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=0;j<NUMBER_OF_TICKS;j++)
		{
		//	gant[j][i].RMS_priority = task[i].RMS_priority;
		//	gant[j][i].task_number = task[i].task_number;
		}
	}

	return 1;
}


/* Scheduler functions */

/* EDF functions */

int compute_EDF_priority(task_set *task,gant_chart **gant,int line_count)
{
	int i=0,j=0;
	int sanity_check=0;
	for(i=0;i<line_count;i++)
	{
		task[i].EDF_priority=line_count;
		for(j=0;j<line_count;j++)
		{
			if(task[i].EDF_deadline_stash > task[j].EDF_deadline_stash)
			{
				task[i].EDF_priority--;
			}
		}
	}
 // print_EDF_priority(task,gant,line_count);
	return 1;
}

int per_cycle_EDF_operation(task_set *task,gant_chart **gant,int line_count)
{
	int i=0,j=0;
	vector <int> task_buffer;
	vector <int> priority_buffer;
	int maxval = 0;
	int index = 0;
	int result_task=0;
	int number_of_active_tasks=0;
	int time_slice_number=0;

	for(i=0;i<line_count;i++)
	{
		if(task[i].status == ACTIVE)
		{
			number_of_active_tasks++;
			task_buffer.push_back(task[i].task_number);
			priority_buffer.push_back(task[i].EDF_priority);
		//==> important print 
		//	cout<<"Time slice : "<<time_slice_number << " ACTIVE task number : " << task[i].task_number<<"\n"; 
		}
	}
	// Find's maxvalue in priority vector

	if(number_of_active_tasks > 0)
	{
		maxval = priority_buffer[0];
		for(i=0;i<priority_buffer.size();i++)
		{
			// cout<<"priQ --> "<<priority_buffer[i]<<"\n";
			if(maxval < priority_buffer[i])
			{
				maxval = priority_buffer[i];

				index=i;
			}
		}
		// cout<<"maxval --> "<<maxval<<"\n";	
		if(task[task_buffer[index]].status == ACTIVE)
		{
			result_task = task[task_buffer[index]].task_number;
			task[task_buffer[index]].number_of_schedules_EDF++;
			task[task_buffer[index]].time_stash--;
			task[task_buffer[index]].EDF_deadline_stash--;
			if(task[task_buffer[index]].time_stash == 0)
			{
				// cout<<"in here "<<task[task_buffer[index]].task_number<<"\n\n";
				task[task_buffer[index]].status = DEAD;
				// cout<<"status : "<<task[task_buffer[index]].status<<"\n";
				
			}
		}
	}
	else 
	{
		result_task = 44;
	}
	//print_task_table(task,line_count);
	// cout<< "time_slice_number : "<<time_slice_number<<" result_task : "<<result_task<<"\n";
	return result_task;

}

int EDF_schedule(task_set *task,gant_chart **gant,int line_count)
{
	int sanity_check = 0;
	int i=0,j=0,k=0;
	int result_stash=0;


	for(i=0;i<NUMBER_OF_TICKS;i++)
	{
		for(j=0;j<line_count;j++)
		{
			if(gant[i][j].arrow_head == UP)
			{
				// cout<<"in here mamu\n\n\n\n";
				task[j].status = ACTIVE;
				task[j].time_stash = task[j].execution_time;
				task[j].EDF_deadline_stash = task[j].EDF_deadline;
			}
			else if(gant[i][j].arrow_head == DOWN)
			{
				//gant[i][j].status = DEAD;
				task[j].status = DEAD;
				if(task[j].time_stash != 0)
				{
					cout<<"something fishy\n";
					cout<<"missed deadline and not schdulable hahahahah!!\n";
				}
			}
		}

		sanity_check = compute_EDF_priority(task,gant,line_count);
		if(sanity_check==0)
		{
			cout<<"issue with EDF computing priority\n";
		}
		// print_EDF_priority(task,gant,line_count);
		// break;
		result_stash=per_cycle_EDF_operation(task,gant,line_count);
		result_array_EDF.push_back(result_stash);

	}

	
	return 1;
}

/* RMS functions */

int prioritize(task_set *task,gant_chart **gant,int line_count)
{
	int i=0,j=0;
	int sanity_check=0;
	for(i=0;i<line_count;i++)
	{
		task[i].RMS_priority=line_count;
		for(j=0;j<line_count;j++)
		{
			if(task[i].period < task[j].period)
			{
				task[i].RMS_priority--;
			}
		}
	}
	print_priority(task,gant,line_count);

	//sanity_check = update_gant_priority(task,gant,line_count);

	return 1;
}

int per_cycle_operation(task_set *task,gant_chart **gant,int time_slice_number,int line_count)
{
	//returns task_number that is scheduled in this time slice

	// cout<<"\n\n\n\n\n\n\n";
	int i=0,j=0;
	vector <int> task_buffer;
	vector <int> priority_buffer;
	int maxval = 0;
	int index = 0;
	//vector<int>::iterator maxval;
	int result_task=0;
	int number_of_active_tasks=0;

	for(i=0;i<line_count;i++)
	{
		if(task[i].status == ACTIVE)
		{
			number_of_active_tasks++;
			task_buffer.push_back(task[i].task_number);
			priority_buffer.push_back(task[i].RMS_priority);
	//		cout<<"Time slice : "<<time_slice_number << " ACTIVE task number : " << task[i].task_number<<"\n"; 
			// cout<< "Time Slice Number : "<<time_slice_number<<"\n";
			// cout<< "priority : "<<gant[time_slice_number][i].RMS_priority<<"\n";
		}
	}
	// Find's maxvalue in priority vector

	if(number_of_active_tasks > 0)
	{
		maxval = priority_buffer[0];
		for(i=0;i<priority_buffer.size();i++)
		{
	//		cout<<"priQ --> "<<priority_buffer[i]<<"\n";
			if(maxval < priority_buffer[i])
			{
				maxval = priority_buffer[i];

				index=i;
			}
		}
	//	cout<<"maxval --> "<<maxval<<"\n";	
		


		if(task[task_buffer[index]].status == ACTIVE)
		{
			result_task = task[task_buffer[index]].task_number;
			task[task_buffer[index]].number_of_schedules_RMS++;
			task[task_buffer[index]].time_stash--;
			if(task[task_buffer[index]].time_stash == 0)
			{
				//	cout<<"in here "<<task[task_buffer[index]].task_number<<"\n\n";
				task[task_buffer[index]].status = DEAD;
				// cout<<"status : "<<task[task_buffer[index]].status<<"\n";
				
			}
		}
	}
	else 
	{
		result_task = 44;
	}

	//print_task_table(task,line_count);

//	cout<< "time_slice_number : "<<time_slice_number<<" result_task : "<<result_task<<"\n";
	return result_task;

}

int RMS_schedule(task_set *task,gant_chart **gant,int line_count)
{
	int sanity_check = 0;
	int i=0,j=0,k=0;
	int result_stash=0;


	sanity_check = prioritize(task,gant,line_count);
	if(sanity_check == 1)
	{
		cout<<"done assigning priorities\n"; 
	}

	for(i=0;i<NUMBER_OF_TICKS;i++)
	{
		for(j=0;j<line_count;j++)
		{
			//break this logic down
			if(gant[i][j].arrow_head == UP)
			{
				// gant[i][j].status = ACTIVE;
			//	cout<<"in here mamu\n\n\n\n";
				task[j].status = ACTIVE;
				task[j].time_stash = task[j].execution_time;
			}
			else if(gant[i][j].arrow_head == DOWN)
			{
				//gant[i][j].status = DEAD;
				task[j].status = DEAD;
				if(task[j].time_stash != 0)
				{
					cout<<"something fishy\n";
					cout<<"missed deadline and not schdulable hahahahah!!\n";
				}
			}
		}
		result_stash = per_cycle_operation(task,gant,i,line_count);
		result_array_RMS.push_back(result_stash);
		// break;
	}

	return 1;
}





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> pthread stuff <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


void worker_function(int input)
{

	cout<<"printing from thread "<<input<<"\n";

}

/*	Thread functions ==> the idea is to spawn threads and schedule them explicitly */

	pthread_mutex_t lock;
	pthread_t *t;
  	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//  	pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;  
//    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; 
//    pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER; 
//    pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER; 
    sem_t *sem;
    int global_thread_id_variable_RMS=0;
    int global_thread_id_variable_EDF=0;
    int *tp=0;



void *worker_thread_RMS(void *tp)
{

	cout<<"inside thread function\n";
	int thread_id=0;
	int sanity_check=0;
	pthread_mutex_lock(&mutex);
	thread_id = global_thread_id_variable_RMS++;
	pthread_mutex_unlock(&mutex);
	int number_of_executions=0;
	task_set *task;

	task = (task_set *)tp;
	

	while(1)
	{
		if(number_of_executions == task[thread_id].number_of_schedules_RMS)
		{
			break;
		}
		sem_wait(&sem[thread_id]);
		worker_function(thread_id);
		number_of_executions++;
		cout<<"inside while of "<<thread_id<<" and number of execution number = "<<number_of_executions<<"\n";
	}

pthread_exit(NULL);
}

void *worker_thread_EDF(void *tp)
{

	cout<<"inside thread function\n";
	int thread_id=0;
	int sanity_check=0;
	pthread_mutex_lock(&mutex);
	thread_id = global_thread_id_variable_EDF++;
	pthread_mutex_unlock(&mutex);
	int number_of_executions=0;
	task_set *task;

	task = (task_set *)tp;
	

	while(1)
	{
		if(number_of_executions == task[thread_id].number_of_schedules_EDF)
		{
			break;
		}
		sem_wait(&sem[thread_id]);
		worker_function(thread_id);
		number_of_executions++;
		cout<<"inside while of "<<thread_id<<" and number of execution number = "<<number_of_executions<<"\n";
	}

pthread_exit(NULL);
}


/*`>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> main function <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



int main()
{

	/* local variable declaration */
	int period=0,execution_time=0,relative_deadline=0,arrival_delay=0;
	string task_name_string,period_string,execution_time_string,relative_deadline_string,arrival_delay_string;
	string line;
	int line_count=0;
	int index=0;
	int sanity_check = 0;			// if--> 1 then ok, else error
	int uivariable=0,uivariable1=0; 
	task_set *task;
	// int *number_of_cycles_tasks_were_active;

	/* iterator */
	int i=0,j=0;

	/* program logic */

	ifstream file( "input6.txt" );
	ifstream file1( "input6.txt" );
	if(file1)
	{
		while(getline(file1,line))
		{
			line_count++;
		}
	}


	/*	Allocating memory for thread variable */

	t = (pthread_t*)malloc(sizeof(pthread_t)*line_count);
	sem = (sem_t*)malloc((sizeof(sem_t))*(line_count));
	// number_of_cycles_tasks_were_active = (int)malloc(sizeof(int)*line_count);
	for(i=0;i<line_count;i++)
	{
		sem_init(&sem[i], 0, 0);
	}

	task = new task_set[line_count];
	gant_chart **gant = new gant_chart*[NUMBER_OF_TICKS];
	for(i=0;i<NUMBER_OF_TICKS;i++)
	{
		gant[i] = new gant_chart[line_count];
	}

	cout<<"Finished memory allocation :"<<"\n";


	if(file)
	{
		while(getline(file,line))
		{
			//cout<<line<<"\n";

			
			replace(line.begin(), line.end(), ',', ' ');

			istringstream iss(line);

			iss >> task_name_string;
			iss >> period_string;
			iss >> execution_time_string;
			iss >> relative_deadline_string;
			iss >> arrival_delay_string;
			period = atoi(period_string.c_str());
			execution_time = atoi(execution_time_string.c_str());
			relative_deadline = atoi(relative_deadline_string.c_str());
			arrival_delay = atoi(arrival_delay_string.c_str());

			// cout<<task_name_string<<" --> "<<period<<" "<<execution_time<<" "<<relative_deadline<<" "<<arrival_delay<<"\n";

			task[index].task_number=index;
			task[index].period=(period);
			task[index].execution_time=execution_time;
			task[index].relative_deadline=relative_deadline;
			task[index].arrival_delay=arrival_delay;

			if(task[index].relative_deadline != 0)
			{
			task[index].EDF_deadline = task[index].relative_deadline;
			}
			else
			{
				task[index].EDF_deadline = task[index].period;
			}

			index++;
		}
		file.close();
	}


	print_task_table(task,line_count);

	// cout<<"printed and done :"<<"\n";



	sanity_check = initalize_gant(gant,line_count);
	// cout<<"after intialize gant"<<"\n";

	//print_gant_chart(gant,line_count);
	sanity_check = populate_gant(task,gant,line_count);

	print_gant_chart(gant,line_count);

lbl1:

	cout<<"tell what scheduling you want to do : \n";
	cout<<"1.RMS scheduling : \n";
	cout<<"2.EDF scheduling : \n";
	cout<<"3.print results\n";
	cout<<"4.Run_task_simulation\n";
	cout<<"5.Exit : \n";
	cout<<"6.print references : \n";
	cin>>uivariable;


	if(uivariable==1)
	{
		sanity_check = check_RMS_schedulability(task,line_count);
		if(sanity_check == 0)
		{
			cout<<"not schedulable --> enter new task task_set"<<"\n";
		//	goto lbl1;
		// 	return 0;
		}

		sanity_check = RMS_schedule(task,gant,line_count);

		

	}
  	else if(uivariable == 2)
	{
		//EDF stuff 
		sanity_check = check_EDF_schedulability(task,line_count);
		if(sanity_check == 0)
		{
			cout<<"not schedulable --> enter new task task_set"<<"\n";
			goto lbl1;
		 	return 0;
		}
		sanity_check = EDF_schedule(task,gant,line_count);

	}
	else if(uivariable == 3)
	{
		print_result_RMS();
		print_result_EDF();
	}
	else if(uivariable == 4)
	{
		lbl2:
		cout<<"----> 1.schedule by RMS\n";
		cout<<"----> 2.schedule by EDF\n";
		cin>>uivariable1;
		/*	Thread create */
		
		if(uivariable1==1)
		{
			for(i=0;i<line_count;i++)
			{
				sanity_check = pthread_create(&t[i], NULL,worker_thread_RMS,(void*)task);
			}
			for(i=0;i<result_array_RMS.size();i++)
			{
				if(result_array_RMS[i]!=44)
				{
					sem_post(&sem[result_array_RMS[i]]);
				}
				else
				{
					cout<<"time_slice_empty\n";
				}
			}
			for(i=0; i<line_count; i++)
			{
			    pthread_join(t[i], NULL);
			}
		}
		else if(uivariable1==2)
		{
			for(i=0;i<line_count;i++)
			{
				sanity_check = pthread_create(&t[i], NULL,worker_thread_EDF,(void*)task);
			}
			for(i=0;i<result_array_EDF.size();i++)
			{
				if(result_array_EDF[i]!=44)
				{
					sem_post(&sem[result_array_EDF[i]]);
				}
				else
				{
					cout<<"time_slice_empty\n";
				}
			}
			for(i=0; i<line_count; i++)
			{
			    pthread_join(t[i], NULL);
			}
		}
		else
		{
			cout<<"----> wrong input machi\n";
			goto lbl2;
		}

	}
	else if(uivariable == 5)
	{
		return 0;
	}
	else if(uivariable == 6)
	{
		print_refereces(task,line_count);
	}
	else 
	{
		cout<<"wrong input machi\n ";
	}
goto lbl1;
}
