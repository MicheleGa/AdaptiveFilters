#include "pmsis.h"

#define STACK_SIZE      1024

void cluster_fn();

// Cluster entry point
static void cluster_entry(void *arg)
{
  // Bare metal parallelism
  pi_cl_team_fork(NUM_CORES, cluster_fn, (void *)0x0);

}


int main()
{

  printf("\n");
  printf("**********************\n");
  printf("*  ADAPTIVE FILTERS  *\n");
  printf("**********************\n");
  printf("\n");

  struct pi_device cluster_dev = {0};
  struct pi_cluster_conf conf;
  struct pi_cluster_task cluster_task = {0};

  // task parameters allocation
  pi_cluster_task(&cluster_task, cluster_entry, NULL);

  // [OPTIONAL] specify the stack size for the task
  cluster_task.stack_size = STACK_SIZE;
  cluster_task.slave_stack_size = STACK_SIZE;

  // open the cluster
  pi_cluster_conf_init(&conf);
  pi_open_from_conf(&cluster_dev, &conf);
  if (pi_cluster_open(&cluster_dev))
  {
    printf("ERROR: Cluster not working\n");
    return -1;
  }

  // send a task to the cluster
  pi_cluster_send_task_to_cl(&cluster_dev, &cluster_task);

  // closing the cluster
  pi_cluster_close(&cluster_dev);

  return 0;
 
}
