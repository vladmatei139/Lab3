#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "led_matrix.h"


void pointless_calculation() {
        int amount = 100000000;
	int x = 0;
	for (int i = 0; i < amount; i++)
		x += i;
}

void run_child(int n){
	for (int i = 0; i < 8; i++){
		pointless_calculation();
		set_led(n,i,RGB565_WHITE);
	}
}

int main(){

        if (open_led_matrix() == -1) {
                printf("Failed to initialize LED matrix\n");
                return -1;
        }

        clear_leds();
	
        for (int num_children = 1; num_children <= 8; num_children++){
            for (int n = 0; n < num_children; n++){
                pid_t pid = fork();
                if (pid == 0){
                    nice(n);
                    run_child(n);
					exit(0);
                }
            }
			for (int n = 0; n < num_children; n++)
						wait(NULL);
			pointless_calculation();
			clear_leds();
					
        }

        if (close_led_matrix() == -1) {
                printf("Could not properly close LED matrix\n");
                return -1;
        }

        return 0;
}

