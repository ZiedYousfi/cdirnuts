#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Dossier courant lors de l'appel : %s\n", cwd);
  } else {
    perror("getcwd() error");
    return 1;
  }

  printf("Hello, World!\n");

  // Return 0 to indicate successful execution
  return 0;
}
