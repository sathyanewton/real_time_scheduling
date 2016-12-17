			/* if arrival delay is non zero */
			/*
			else if(task[i].arrival_delay != 0)
			{
				if(j!=0)
				{
					if(j == task[i].arrival_delay)
					{
						gant[j][i].arrow_head = UP;
					}
					else if(((task[i].period / j)- task[i].arrival_delay) == 0)
					{
						gant[j][i].arrow_head = 1;
					}
				}
			}
			*/

/*********************************** RMS scheduling *************************************************************/


/*
	for(j=0;j<NUMBER_OF_TICKS;j++)	// for every time slice 
	{

		for(i=0;i<line_count;i++)	// bubble compare
		{
			if(gant[j][i].arrow_head == UP)
			{

				for(k=(i+1);k<line_count;k++)
				{	
					if((i+1)<=line_count)
					{

						if(gant[j][k].arrow_head == UP)
						{

							if(task[i].RMS_priority > task[k].RMS_priority)
							{

							}
							else if((task[i].RMS_priority < task[k].RMS_priority))
							{


							} 

						}
					}

				}
			}
		}

	}

}

*/

/**************************************************** per thread function ***************************************************/

/*

	for(i=0;i<priority_buffer.size();i++)
	{
	// cout<<"size :"<<priority_buffer.size()<<"\n";	
		cout<<"priority buffer : "<<priority_buffer[i]<<"\n";
		// find max element and find it's location/index, find the respective task and schedule it. take care of running times.  
		for(j=0;j<=priority_buffer.size();j++)
		{
			if(priority_buffer[i] >= priority_buffer[j])
			{
			//	cout<<"time slice"<<time_slice_number<<"in here goma";
				maxval = priority_buffer[i];
				
			}
		}

	}
	cout<<"priority maxval -- > "<<maxval<<"Time slice numeber : " <<time_slice_number<<"\n";

	// evaluvate position of the max value 
	
	for(i=0;i<priority_buffer.size();i++)
	{
	if(task[i].status == ACTIVE)
	{
		if(maxval == priority_buffer[i])
		{
			cout<<"in here goma i value "<<i<<"\n\n\n\n\n\n\n\n";
			result_task = task_buffer[i];
			task[i].time_stash--;
		
			if(task[i].time_stash == 0)
			{
				task[i].status = DEAD;
			}
		}
	}
	else
	{
		//cout<< "time_slice_number : "<<time_slice_number<<" result_task : "<<44<< " " <<"\n";
		return 44;
	}

	}

*/




