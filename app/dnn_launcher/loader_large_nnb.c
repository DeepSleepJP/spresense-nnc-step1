/****************************************************************************
 * dnnrt_lenet/loader_learge_nnb.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dnnrt/runtime.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int get_nnb_size(const char *nnb_path, uint32_t * size_ptr)
{
  int ret;
  struct stat nnb_stat;

  if (nnb_path == NULL || size_ptr == NULL)
    {
      return -EINVAL;
    }
  ret = stat(nnb_path, &nnb_stat);
  if (ret == 0)
    {
      *size_ptr = nnb_stat.st_size;
    }
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
nn_network_t *load_nnb_network(const char *nnb_path, void *target)
{
  int ret;
  FILE *file = NULL;
  uint32_t exp_data_bsize, act_data_bsize;
  nn_network_t *network = target;

  if (nnb_path == NULL)
    {
      goto file_open_err;
    }
  printf("nnb_path = %s\n", nnb_path);
  fflush(stdout);
  file = fopen(nnb_path, "r");
  if (file == NULL)
    {
      printf("ERROR: fopen is failed in load_nnb_network\n");
      goto file_open_err;
    }
  
  fflush(stdout);
  /* get size of nnb_file in units of bytes */
  ret = get_nnb_size(nnb_path, &exp_data_bsize);
  if (ret)
    {
      goto get_size_error;
    }
  printf("INFO: NNB size is %dbyte\n", exp_data_bsize);
  fflush(stdout);

  act_data_bsize = fread(network, 1, exp_data_bsize, file);
  if (exp_data_bsize != act_data_bsize)
    {
      printf("ERROR: fread is failed in load_nnb_network (req=%dbyte, result=%dbyte)\n", exp_data_bsize, act_data_bsize);
      free(network);
      network = NULL;
    }
    
  fflush(stdout);

get_size_error:
  fclose(file);
file_open_err:
  return network;
}

