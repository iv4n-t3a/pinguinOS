#include "core_lib/boot_params.h"

void kmain(boot_params_t params);

int main() {
  boot_params_t params;
  kmain(params);
}
