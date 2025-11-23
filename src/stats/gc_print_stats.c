#include "internal_collector.h"
#include "unistd.h"

/*print single stat line (helper to avoid printf in multiple places)*/
static void gc_print_line(const char *label,size_t value)
{
	size_t	i;
	size_t	temp;
	char	buffer[32];
	int		len;

	i = 0;
	while(label[i])
		write(1,&label[i++],1);
	write(1,": ",2);
	len = 0;
	temp = value;
	if(temp == 0)
		buffer[len++] = '0';
	while (temp > 0)
	{
		buffer[len++] = (temp % 10) + '0';
		temp /= 10;
	}
	while(--len >= 0)
		write(1,&buffer[len],1);	
	write(1,"bytes\n", 7);
}

/*
	*print gc stat.st.cs to stdout
	*shows allocation/free counts and memory usage
*/
void gc_print_stats(t_gc_context *contex)
{
	if(!contex)
		return ;
	write(1,"=== GC Statistics ===\n",22);
	
	gc_print_line("Total Allocated", contex->total_allocated);
	gc_print_line("Total Freed", contex->total_freed);
	gc_print_line("Current Usage", contex->current_usage);
	gc_print_line("Peak Usage", contex->peak_usage);
	write(1, "Allocation Count: ", 18);
	gc_print_line("", contex->allocation_count);
	write(1, "Free Count: ", 12);
	gc_print_line("", contex->free_count);
	write(1, "=====================\n", 22);
}
