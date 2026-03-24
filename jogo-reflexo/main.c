#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define btn_r 15
#define btn_g 14
#define btn_y 12

#define led_r 18
#define led_g 19
#define led_y 21

volatile int flg_r=0;
volatile int flg_g=0;
volatile int flg_y=0;
volatile int flg_inicio =0;

volatile int tamanho =0;

const int leds[3]={led_r, led_g, led_y};
const int sequencia [10] ={led_y, led_g, led_r, led_y, led_g, led_y, led_r, led_y, led_g, led_y};



void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    if (gpio == btn_r){
        flg_r=1;
    }if (gpio == btn_g){
        flg_inicio=1;
        flg_g=1;
    }if (gpio == btn_y){
        flg_y=1;
    }
  
  }
}
void acender_led(int cor) {
    gpio_put(cor, 1);
    sleep_ms(300);
    gpio_put(cor, 0);
    sleep_ms(300);
}

int main() {
    stdio_init_all();

    gpio_init(btn_r);
    gpio_set_dir(btn_r, GPIO_IN);
    gpio_pull_up(btn_r);

    gpio_init(btn_g);
    gpio_set_dir(btn_g, GPIO_IN);
    gpio_pull_up(btn_g);

    gpio_init(btn_y);
    gpio_set_dir(btn_y, GPIO_IN);
    gpio_pull_up(btn_y);

    gpio_init(led_r);
    gpio_set_dir(led_r, GPIO_OUT);
    gpio_init(led_g);
    gpio_set_dir(led_g, GPIO_OUT);
    gpio_init(led_y);
    gpio_set_dir(led_y, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(btn_r, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(btn_g, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(btn_y, GPIO_IRQ_EDGE_FALL, true);


    while (true) {
        if (flg_inicio) {
            flg_inicio = 0;
            tamanho = 0;
            int pnts = 0;
            bool erro = false;

            //  10 rodadas
            for (int r = 1; r <= 10; r++) {
                //  Mostrar sequência
                for (int i = 0; i < r; i++) {
                    acender_led(sequencia[i]);
                }

                // 2. Esperar o jogador repetir
                for (int i = 0; i < r; i++) {
                    flg_r = 0; 
                    flg_g = 0; 
                    flg_y = 0;
                    int btn_clicado = -1;


                    

                    while (flg_r == 0 && flg_g == 0 && flg_y == 0);

                    if (flg_r) {
                        btn_clicado = led_r;
                    }
                    else if (flg_g){
                        btn_clicado = led_g;
                    }
                    else if (flg_y) {
                        btn_clicado = led_y;
                    }
                    if (btn_clicado == sequencia[i]) {
                        gpio_put(btn_clicado, 1);
                        sleep_ms(300);
                        gpio_put(btn_clicado, 0);

                    } else {
                        erro = true;
                        break;
                    }
                    sleep_ms(100); 
                }

                if (erro) break;
                pnts = r; 
                sleep_ms(500); 
            }

           
            printf("Points %d\n", pnts);
        }
    }
}
