#include <stdlib.h>
#include <stdio.h> //biblioteca padrão da linguagem C
#include <stdint.h>
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "pico/time.h" //biblioteca para gerenciamento de tempo
#include "hardware/irq.h" //biblioteca para gerenciamento de interrupções
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"   
#include "pico/bootrom.h"  
#include <hardware/pio.h>
#include "ws2818b.pio.h"
#include "hardware/clocks.h"
#include "inc/numeros.h"

// Definição Pinos I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definição dos Botões e RGB
#define BUTTON_A 5  // Pino do botão A
#define BUTTON_B 6  // Pino do botão B
#define BUZZER_A 21  // Pino do buzzer A
#define BUZZER_B 10  // Pino do buzzer A
#define LED_VERMELHO 13    // Pino do LED Vermelho
#define LED_VERDE 11    // Pino do LED Verde
#define LED_AZUL 12    // Pino do LED Azul
#define JOYSTICK_Y_PIN 26  // GPIO para eixo X
#define JOYSTICK_X_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick

// Configuração Frequência do PWM
const uint16_t WRAP_PERIOD = 25000; //valor máximo do contador - WRAP
const float PWM_DIVISER = 100.0; //divisor do clock para o PWM

// Variáveis globais
static volatile uint a = 1;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
volatile bool JB = false;  // Estado do JB Verde
volatile bool pwm_state = true;   // Estado do PWM Azul
volatile bool matriz_ligada = true;  // Estado inicial da matriz de LEDs (ligada)
volatile bool modo_emergencia = false;  // Estado de emergência (ativado pelo botão B)
// Variáveis para o menu
const char *menu_items[] = {
    "AGUA", "COMIDA", "BANHEIRO", "REMEDIO", "DOR", "CAMINHADA","ROUPA" ,"AJUDA", "SONO"
};
const int num_menu_items = sizeof(menu_items) / sizeof(menu_items[0]);
int selected_index = 0; // Índice da opção selecionada
int scroll_offset = 0;  // Deslocamento do menu (quantas opções estão "fora" da tela)


//Variáveis para o PIO
// Definição do número de LEDs e pino.
#define LED_COUNT 25   
#define LED_PIN 7
int Contador = 0; // Índice da opção selecionada
char numero;

// Definição de pixel GRB
struct pixel_t {
    uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
   };
   typedef struct pixel_t pixel_t;
   typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.
   
   // Declaração do buffer de pixels que formam a matriz.
   npLED_t leds[LED_COUNT];
   
   // Variáveis para uso da máquina PIO.
   PIO np_pio;
   uint sm;

// Declaração de funções
void inoutput_init();
static void gpio_irq_handler(uint gpio, uint32_t events);
void pwm_setup();
void draw_menu(ssd1306_t *ssd, int selected_index, int scroll_offset, int contador);

void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int getIndex(int x, int y);
void npSetPattern(const int matriz[5][5][3]);
void piscar_leds_e_sirene();

int main()
{
    // Inicializa comunicação USB CDC para monitor serial
    stdio_init_all(); 
    // Inicializa entradas e saídas.
    inoutput_init();
    // Inicializa o PWM 
    pwm_setup();
    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();
    npWrite();

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  
    // Variável de Leitura do adc
    uint16_t adc_value_y;
    uint16_t adc_value_x;  

    //Controle de envio
    bool Controle_Envio = JB;

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    while (true)
    {
        // Atualiza o conteúdo do display com animações
        adc_select_input(0); // Seleciona o ADC para eixo y. O pino 26 como entrada analógica
        adc_value_y = adc_read();
        adc_select_input(1); // Seleciona o ADC para eixo x. O pino 27 como entrada analógica
        adc_value_x = adc_read();

        // Verifica se o joystick foi movido para cima ou para baixo
        if (adc_value_y < 1000) { // Movimento para baixo
            if (selected_index < num_menu_items - 1) {
                selected_index++;
                if (selected_index >= scroll_offset + 4) { // Rola o menu para baixo
                    scroll_offset++;
                }
            }
        } else if (adc_value_y > 3000) { // Movimento para cima
            if (selected_index > 0) {
                selected_index--;
                if (selected_index < scroll_offset) { // Rola o menu para cima
                    scroll_offset--;
                }
            }
        }


                // Verifica se o joystick foi movido para esquerda ou para direita
                if (adc_value_x < 1000) { // Movimento para esquerda
                    if (Contador > 0) {
                        Contador--; // Diminui o número
                    }
                } else if (adc_value_x > 3000) { // Movimento para Direita
                    if (Contador < 9) {
                        Contador++; // Aumenta o número
                    }
                }

        // Desenha o menu com a opção selecionada
        draw_menu(&ssd, selected_index, scroll_offset, Contador);

        // Atualiza o display
        ssd1306_send_data(&ssd);

        if (Controle_Envio!=JB && modo_emergencia==false)
        {
        // Imprime a opção selecionada no monitor serial ao pressionar o botão do joystick
        printf("Opcao confirmada: %s | Nivel de urgencia: %d\n", menu_items[selected_index], Contador);
        Controle_Envio = JB;
        gpio_put(LED_VERDE, true);
        sleep_ms(1000);
        gpio_put(LED_VERDE, false);
        }

        // Verifica se o modo de emergência está ativo
    if (modo_emergencia) {
        piscar_leds_e_sirene();  // Pisca os LEDs e ativa a sirene
        printf("URGENTE 10 !!!!\n" );
    } else {
        // Desliga os LEDs e buzzers quando o modo de emergência é desativado
        gpio_put(LED_VERMELHO, false);
        gpio_put(LED_AZUL, false);
        pwm_set_gpio_level(BUZZER_A, 0);  // Desliga o buzzer A
        pwm_set_gpio_level(BUZZER_B, 0);  // Desliga o buzzer B
    }



// Verifica se a matriz de LEDs está ligada ou desligada
if (matriz_ligada) {
    // Escolha o padrão a ser exibido com base no índice
    switch (Contador) {
        case 0:
            npSetPattern(ZERO);  // Exibe o padrão ZERO
            break;
        case 1:
            npSetPattern(UM);  // Exibe o padrão UM
            break;
        case 2:
            npSetPattern(DOIS);  // Exibe o padrão DOIS
            break;
        case 3:
            npSetPattern(TRES);  // Exibe o padrão TRES
            break;
        case 4:
            npSetPattern(QUATRO);  // Exibe o padrão QUATRO
            break;
        case 5:
            npSetPattern(CINCO);  // Exibe o padrão CINCO
            break;
        case 6:
            npSetPattern(SEIS);  // Exibe o padrão SEIS
            break;
        case 7:
            npSetPattern(SETE);  // Exibe o padrão SETE
            break;
        case 8:
            npSetPattern(OITO);  // Exibe o padrão OITO
            break;
        case 9:
            npSetPattern(NOVE);  // Exibe o padrão NOVE
            break;
        default:
            npClear();
            npWrite();
            break;
    }
} else {
    // Se a matriz estiver desligada, limpa os LEDs
    npClear();
    npWrite();
}

        // Delay para evitar leituras muito rápidas do joystick
        sleep_ms(100);
    }
}

// Função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) // 200 ms de debounce
    {
        last_time = current_time; // Atualiza o tempo do último evento

        if (gpio == JOYSTICK_PB) { 
            JB = !JB;  
        }

        if (gpio == BUTTON_A) { 
            matriz_ligada = !matriz_ligada;  // Alterna o estado da matriz de LEDs
        }
        if (gpio == BUTTON_B) { 
            modo_emergencia = !modo_emergencia;  // Alterna o estado de emergência
        }
    }
}

// Função para desenhar o menu
void draw_menu(ssd1306_t *ssd, int selected_index, int scroll_offset, int contador) {
    ssd1306_fill(ssd, false); // Limpa o display
    char numero = Contador + '0';  // Converte o inteiro para o caractere correspondente
    ssd1306_draw_string(ssd, "URGENCIA:", 30, 0); // Desenha uma string
    ssd1306_draw_char(ssd, numero, 100, 0); // Desenha uma string     

    // Desenha as opções visíveis do menu
    for (int i = 0; i < 4; i++) {
        int item_index = scroll_offset + i;
        if (item_index < num_menu_items) {
            ssd1306_draw_string(ssd, menu_items[item_index], 30, 12 + (i * 12));
        }
    }

    // Desenha o seletor "X" ao lado da opção selecionada
    ssd1306_draw_string(ssd, "X", 15, 12 + ((selected_index - scroll_offset) * 12));
}

// Inicializa os leds e botões
void inoutput_init()
{
    // Input
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AZUL);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERMELHO, false);
    gpio_put(LED_AZUL, false);
    gpio_put(LED_VERDE, false);

    // Configura os pinos dos botões como entrada
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);  // Ativa o resistor pull-up interno para o botão A

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);  // Ativa o resistor pull-up interno para o botão B

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB); // Ativa o resistor pull-up interno para o botão do JOYSTICK

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);  

    //Inicializa os Buzzers
    gpio_init(BUZZER_A);
    gpio_set_dir(BUZZER_A, GPIO_OUT);
    gpio_put(BUZZER_A, false);
    gpio_init(BUZZER_B);
    gpio_set_dir(BUZZER_B, GPIO_OUT);
    gpio_put(BUZZER_B, false);
}



// Função para configurar o módulo PWM
void pwm_setup()
{
    gpio_set_function(BUZZER_A, GPIO_FUNC_PWM); // Configura o pino do buzzer A para função PWM
    gpio_set_function(BUZZER_B, GPIO_FUNC_PWM); // Configura o pino do buzzer B para função PWM


    uint slice_num_buzzera = pwm_gpio_to_slice_num(BUZZER_A); // Obtém o número do slice PWM do pino do buzzer A
    uint slice_num_buzzerb = pwm_gpio_to_slice_num(BUZZER_B); // Obtém o número do slice PWM do pino do buzzer B


    pwm_set_clkdiv(slice_num_buzzera, PWM_DIVISER); //define o divisor de clock do PWM
    pwm_set_clkdiv(slice_num_buzzerb, PWM_DIVISER); //define o divisor de clock do PWM

 
    pwm_set_wrap(slice_num_buzzera, WRAP_PERIOD); //definir o valor de wrap
    pwm_set_wrap(slice_num_buzzerb, WRAP_PERIOD); //definir o valor de wrap


    pwm_set_gpio_level(BUZZER_A, 0); //definir o cico de trabalho (duty cycle) do pwm
    pwm_set_gpio_level(BUZZER_B, 0); //definir o cico de trabalho (duty cycle) do pwm


    pwm_set_enabled(slice_num_buzzera, true); //habilita o pwm no slice correspondente
    pwm_set_enabled(slice_num_buzzerb, true); //habilita o pwm no slice correspondente
}


/**
* Inicializa a máquina PIO para controle da matriz de LEDs.
*/
void npInit(uint pin) {

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
   
    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
      np_pio = pio1;
      sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }
   
    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
   
    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i) {
      leds[i].R = 0;
      leds[i].G = 0;
      leds[i].B = 0;
    }
   }
   
   /**
   * Atribui uma cor RGB a um LED.
   */
   void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
   }
   
   /**
   * Limpa o buffer de pixels.
   */
   void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 0, 0, 0);
   }
   
   /**
   * Escreve os dados do buffer nos LEDs.
   */
   void npWrite() {
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
      pio_sm_put_blocking(np_pio, sm, leds[i].G);
      pio_sm_put_blocking(np_pio, sm, leds[i].R);
      pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
   }
   
   int getIndex(int x, int y) {
       // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
       // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
       if (y % 2 == 0) {
           return y * 5 + x; // Linha par (esquerda para direita).
       } else {
           return y * 5 + (4 - x); // Linha ímpar (direita para esquerda).
       }
   }
   
   /**
    * Função para acender os LEDs com base nas matrizes de desenho.
    * LEDs com valor 1 no vetor serão acesos com a cor especificada.
    */
   void npSetPattern(const int matriz[5][5][3]) {
       int index = 0; // Índice para mapear a matriz 2D para o vetor linear de LEDs (0 a 24)
   int conversao[5][5][3]=
   {
       {{matriz[4][4][0],matriz[4][4][1], matriz[4][4][2]},{matriz[4][3][0],matriz[4][3][1], matriz[4][3][2]}, {matriz[4][2][0],matriz[4][2][1], matriz[4][2][2]}, {matriz[4][1][0],matriz[4][1][1], matriz[4][1][2]}, {matriz[4][0][0],matriz[4][0][1], matriz[4][0][2]}},
       {{matriz[3][0][0],matriz[3][0][1], matriz[3][0][2]}, {matriz[3][1][0],matriz[3][1][1], matriz[3][1][2]}, {matriz[3][2][0],matriz[3][2][1], matriz[3][2][2]}, {matriz[3][3][0],matriz[3][3][1], matriz[3][3][2]}, {matriz[3][4][0],matriz[3][4][1], matriz[3][4][2]}},
       {{matriz[2][4][0],matriz[2][4][1], matriz[2][4][2]}, {matriz[2][3][0],matriz[2][3][1], matriz[2][3][2]}, {matriz[2][2][0],matriz[2][2][1], matriz[2][2][2]}, {matriz[2][1][0],matriz[2][1][1], matriz[2][1][2]}, {matriz[2][0][0],matriz[2][0][1], matriz[2][0][2]}},
       {{matriz[1][0][0],matriz[1][0][1], matriz[1][0][2]},{matriz[1][1][0],matriz[1][1][1], matriz[1][1][2]}, {matriz[1][2][0],matriz[1][2][1], matriz[1][2][2]}, {matriz[1][3][0],matriz[1][3][1], matriz[1][3][2]}, {matriz[1][4][0],matriz[1][4][1], matriz[1][4][2]}},
       {{matriz[0][4][0],matriz[0][4][1], matriz[0][4][2]}, {matriz[0][3][0],matriz[0][3][1], matriz[0][3][2]}, {matriz[0][2][0],matriz[0][2][1], matriz[0][2][2]}, {matriz[0][1][0],matriz[0][1][1], matriz[0][1][2]}, {matriz[0][0][0],matriz[0][0][1], matriz[0][0][2]}}
   };
   
   
       for (int linha = 0; linha < 5; linha++) {
           for (int coluna = 0; coluna < 5; coluna++) {
               int r = conversao[linha][coluna][0]; // Componente R
               int g = conversao[linha][coluna][1]; // Componente G
               int b = conversao[linha][coluna][2]; // Componente B
   
               // Acende o LED na posição correspondente
               npSetLED(index, r, g, b);
               index++;
           }
       }
       npWrite(); // Envia os dados para a matriz de LEDs
   }

   void piscar_leds_e_sirene() {
    static bool estado_leds = false;  // Estado atual dos LEDs (ligado/desligado)
    static uint32_t ultimo_tempo = 0;  // Último tempo de alternância
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    // Alterna os LEDs e buzzers a cada 100 ms
    if (tempo_atual - ultimo_tempo > 100000) {  // 100 ms
        ultimo_tempo = tempo_atual;

        estado_leds = !estado_leds;  // Inverte o estado dos LEDs

        // Alterna os LEDs vermelho e azul
        gpio_put(LED_VERMELHO, !estado_leds);
        gpio_put(LED_AZUL, estado_leds);

        // Gera o som de sirene nos buzzers
        if (estado_leds) {
            pwm_set_gpio_level(BUZZER_A, WRAP_PERIOD / 2);  // 50% de duty cycle (som alto)
            pwm_set_gpio_level(BUZZER_B, WRAP_PERIOD / 2);  // 50% de duty cycle (som alto)
        } else {
            pwm_set_gpio_level(BUZZER_A, 0);  // Desliga o buzzer A
            pwm_set_gpio_level(BUZZER_B, 0);  // Desliga o buzzer B
        }
    }
}