#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/time.h>
asmlinkage void get_process_switch_info(struct process_switch_info * psi, int pid1, int pid2){
	struct process_switch_info* ps_info = NULL;
	ps_info = psi;
	struct task_struct *tasks = NULL;
	struct task_struct *process1 = NULL;
	struct task_struct *process2 = NULL;
	
	for_each_process(tasks){
		if(tasks->pid == pid1){
			process1 = tasks;
		}
		else if(tasks->pid == pid2){
			process2 = tasks;
		}
	}

	if(process1 && process2){
		ps_info->counter_cpu = process1->context_switch_count;
		ps_info->counter_IO = process2->context_switch_count;
		ps_info->time_cpu.tv_sec = process1->idle_time.tv_sec;
		ps_info->time_cpu.tv_usec = process1->idle_time.tv_usec;
		ps_info->time_IO.tv_sec = process2->idle_time.tv_sec;
		ps_info->time_IO.tv_usec = process2->idle_time.tv_usec;
	}
	else{
		printk("\nprocess1(PID:%d) or process2(PID:%d) not found\n", pid1, pid2);
		return 0;
	}
	return 0;
}
