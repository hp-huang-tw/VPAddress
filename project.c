#include <linux/printk.h>
#include <linux/syscalls.h>
#include <asm/syscalls.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <asm/pgtable.h>
asmlinkage int sys_project(long pid)
{
	struct task_struct *task = NULL;
	struct mm_struct *mm = NULL;	//descript process's memory description
	struct vm_area_struct *first_vma = NULL;
	struct vm_area_struct *vma = NULL;
	struct page *page = NULL;
	unsigned long vm_address;

	for_each_process(task)
	{
		if(task->pid == pid)
		{
			printk(KERN_INFO "The image name of the process(PID: %i) is %s.\n", task->pid, task->comm);
			mm = task->mm;
			if(mm != NULL && mm->pgd != NULL && mm->mmap != NULL)
			{
				vma = mm->mmap;
				while(vma->vm_next != NULL)
				{
					if(vma->vm_file != NULL)
						printk(KERN_INFO "start( 0x%08lx ):end( 0x%08lx )\t%s\n", vma->vm_start, vma->vm_end, vma->vm_file->f_path.dentry->d_iname);                                      
					else
						printk(KERN_INFO "start( 0x%08lx ):end( 0x%08lx )\n", vma->vm_start, vma->vm_end);		
					
					printk("Physical Address:\n");
					/* Physical Address */
					for( vm_address = vma->vm_start; vm_address < vma->vm_end; vm_address +=0x1000)
					{
						page = follow_page(vma, vm_address, FOLL_GET);
						if(page == NULL) continue;
						printk("0x%08lx\t", (unsigned long) page_to_pfn(page));
					}
					printk("\n");
					vma = vma->vm_next;
				}				
			}
		}
	}
	return 0;
}
