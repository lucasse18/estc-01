#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "dbg.h"
#include "bernoulli.h"


void print_usage();

int main(int argc, char *argv[]) {

  static struct option opts[] = {
    {"help",       no_argument,       0, 'h'},
    {"iterations", required_argument, 0, 'm'},//default 5
    {"samples",    required_argument, 0, 'n'},//default 10
    {"seed",       required_argument, 0, 's'},//default 0
    {"outfile",    required_argument, 0, 'o'},//default stdout
    {0,            0,                 0,   0}
  };

  FILE *outfile = stdout;
  size_t sample_count = 10, mean_count = 5;
  unsigned long s = 0;

  int c;
  int index = 0;

  while((c = getopt_long(argc, argv, "hm:n:s:o:", opts, &index)) != -1) {
    switch(c) {
    case 0:
      break;

    case 'h':
      print_usage();
      exit(0);

    case 'o':
      check(outfile == stdout, "duplicate option -o.");
      check(strcmp("stdout", optarg), "reopening stdout is unsupported");
      check(strcmp("stderr", optarg), "reopening stderr is unsupported");
      check(strcmp("stdin",  optarg), "trying to set outfile as stdin");
      outfile = fopen(optarg, "w");
      check(outfile != NULL, "could not open file for writing.");
      break;

    case 'n':
      sample_count = (size_t) atol(optarg);
      break;

    case 'm':
      mean_count = (size_t) atol(optarg);
      break;

    case 's':
      s = atol(optarg);
      break;

    case '?':
      // user passed an unsupported option
      // getopt_long already printed an error message
      exit(1);

    default:
      abort();
    }
  }

  if (!argv[optind]) {
    fprintf(stderr, "bernoulli: no probability specified, stopping.\n");
    goto error;
  }

  double p = atof(argv[optind]);

  srand48(s);

  fprintf(outfile, "# sample_mean_for_P=%f\n", p);
  for(size_t i = 0; i < mean_count; i++) {
    double s_mean = 0;
    for(size_t j = 0; j < sample_count; j++)
      s_mean += get_bernoulli_value(&p);
    fprintf(outfile, "%.8f\n",  s_mean/sample_count);
  }

  return 0;

error:
  exit(-1);

}

void print_usage() {
  printf("Usage: bernoulli [options] P\n");
  printf("Generate a file with <i> sample means for a bernoulli distribution \
with <n> samples and probability P.\n");
  printf("Options:\n");
  printf("  -h, --help             \tDisplay this help message and exit.\n");
  printf("  -m, --means <arg>      \tNumber of sample means (default 5).\n");
  printf("  -n, --samples <arg>    \tNumber of ramdom samples to generate for each mean (default 10).\n");
  printf("  -s, --seed <arg>       \tSeed to feed the RNG algorithm (default %d).\n", 0);
  printf("  -o, --outfile <file>   \tWrite output to <file>.\n");
}
