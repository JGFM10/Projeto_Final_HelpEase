# HelpEase: Sistema de Comunicação Assistiva

Este projeto é um sistema de comunicação assistiva desenvolvido para ajudar pessoas em situações de vulnerabilidade, como idosos ou pacientes com dificuldades de locomoção, a solicitarem ajuda de forma simples e intuitiva. O sistema utiliza um display OLED para exibir um menu de opções, um joystick para navegação e uma matriz de LEDs para indicar o nível de urgência.

Vídeo de Demonstração:https://youtu.be/YXceuPVkP-U

Vídeo de apresentação:https://youtu.be/9cm-BgUJBuU

## Funcionamento Básico do Código

### 1. **Inicialização**
- O código começa configurando os pinos GPIO, ADC, PWM, I2C e PIO.
- O display OLED é inicializado e exibe o menu inicial.
- A matriz de LEDs é configurada e limpa.

### 2. **Menu Interativo**
- O menu é exibido no display OLED, com opções como "Água", "Comida", "Banheiro", etc.
- O joystick é usado para navegar entre as opções:
  - Movimento vertical: Navega entre as opções.
  - Movimento horizontal: Ajusta o nível de urgência (1 a 9).
  - Botão do joystick: Confirma a seleção.

### 3. **Nível de Urgência**
- O nível de urgência selecionado é exibido na matriz de LEDs.
- O valor é atualizado em tempo real conforme o movimento do joystick.

### 4. **Modo de Emergência**
- O botão B ativa o modo de emergência:
  - A sirene é acionada (buzzer).
  - Os LEDs piscam de forma intensa.
  - Uma mensagem de emergência é enviada ao monitor serial.

### 5. **Controle da Matriz de LEDs**
- A matriz de LEDs é controlada via protocolo WS2812B.
- Padrões pré-definidos são usados para exibir números de 0 a 9, representando o nível de urgência.

### 6. **Interrupções**
- As interrupções são usadas para tratar eventos dos botões e joystick.
- Debouncing é implementado para evitar leituras incorretas.

### 7. **Loop Principal**
- O loop principal do código:
  - Lê os valores do joystick (eixos X e Y).
  - Atualiza o menu e o nível de urgência.
  - Verifica se o modo de emergência foi ativado.
  - Controla a matriz de LEDs e o buzzer.

## Como Usar
1. Conecte o hardware conforme descrito na pinagem.
2. Compile e carregue o código no Raspberry Pi Pico W.
3. Utilize o joystick para navegar no menu e selecionar opções.
4. Pressione o botão B para ativar o modo de emergência.

## Dependências
- Biblioteca SSD1306 para o display OLED.
- Biblioteca PIO para controle da matriz de LEDs WS2812B.
- SDK do Raspberry Pi Pico.
