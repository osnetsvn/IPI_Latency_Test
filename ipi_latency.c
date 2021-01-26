#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#define IPI_COUNT 100000
static unsigned long ipi_count=IPI_COUNT;
module_param_named(ipi_count, ipi_count, ulong, S_IRUGO | S_IWUSR);
static void __init handle_ipi(void *t)
{
	ktime_t *time=(ktime_t *) t;
	*time=ktime_sub(ktime_get(),*time);
	trace_printk("Latency:\t%llu\n",*time);

}
static void __init send_ipi(void){
	ktime_t time;
	unsigned int src_cpu=get_cpu();
	unsigned int des_cpu;
	//Send IPI to ANY OTHER CORE ONLINE BUT SELF	
	//des_cpu=cpumask_any_but(cpu_online_mask,src_cpu);
	time=ktime_get();
	smp_call_function_single(3,handle_ipi,&time,1);
	put_cpu();
}

static int __init init_ipi_latency(void){
	while(ipi_count --){
		send_ipi();
		udelay(200);
	}
	return 0;
}
static void __exit exit_module(void){
	printk("Module removed\n");
}
module_init(init_ipi_latency);
module_exit(exit_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ROJA ESWARAN");
